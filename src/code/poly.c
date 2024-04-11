#include "common.h"

/* rodata */
extern char D_801103D0[];

extern f64 D_801104F8;
extern s32 D_80236974;
extern Collision gZoneCollisions[];
extern Collider D_80236980[128];
extern s32 D_8020D8F4;
extern f64 D_801106A0;
extern f64 D_801106A8;

extern Vec3f D_80108F9C;
extern Vec3f D_80108FA8;
extern Vec3f D_80108FB4;
extern Vec3f D_80108FC0;
extern f32 D_80108FCC;
extern s32 D_80108FD0;
extern s32 D_80108FD4;
extern f32 D_80108FD8;
extern f32 D_80108FDC;
extern s32 D_80108FE0;
extern f32 D_80108FE4;
extern s32 D_80108FE8;
extern s32 D_80108FEC;
extern Vec3f D_802489C8[8];
extern Poly sPolygonArray[1024];
extern s32 sNumPolygons;
extern Poly* sPolygons[1024];
extern s32 sNumListedPolygons;
extern Collider* D_80240898[];

typedef struct TouchedPolygon {
    /* 0x00 */ Poly* poly;
    /* 0x04 */ s32 isOutside; // probably has different meaning
    /* 0x08 */ f32 distance;
    /* 0x0C */ f32 height;
    /* 0x10 */ Vec3f point;
} TouchedPolygon; //sizeof 0x1C

/* Migrated BSS */
//TODO: type this data correctly
char gShadows[0xC00];
s32 gShadowCount;
Vec3f D_80248518;
TouchedPolygon sTouchedPolygons[32];
s32 sNumTouchedPolygons;
Vec3f D_802488B0;
char D_802488BC_pad[0x4];
char gHasShadow[0x100];
char D_802489C0[0x08];
Vec3f D_802489C8[8];
char D_80248A28[0x08];

Vec3f* RotateVector3D(Vec3f*, Vec3f, f32, s32);
void func_800D3854(PlayerActor*, Tongue*, Camera*, Vec3f*, Vec3f*, s32);
void func_800D5394(PlayerActor*, Tongue*, Camera*, Vec3f*, Vec3f*, s32);
void func_800D6864(PlayerActor*, Tongue*, Camera*, Vec3f*, Vec3f*);
void func_800D69D0(s32, PlayerActor*, Tongue*, Camera*, Vec3f*, Vec3f*, s32);
Poly* SearchPolyBelow(Vec3f, f32, f32);
Poly* SearchPolygonBetween(Vec3f, Vec3f, s32, s32, s32);
void OrderRectBounds(Rect3D*);
Vec3f* func_800CA734(Vec3f*, Vec3f, f32, s32);
void func_800CBC08(Actor*);
void func_800CC814(Actor*, Vec3f, s32);
Vec3f* LocalToWorld(Vec3f* outVec, Vec3f vec, Poly* poly);
Vec3f* WorldToLocal(Vec3f* outVec, Vec3f vec, Poly* poly);
s32 IsOnPolygon(Vec3f vec, Poly* poly);
s32 Vec3f_EqualsCopy(Vec3f vec1, Vec3f vec2);
s32 IsInsidePolygon(Vec3f vec, Poly* poly);
void Vec3f_Set(Vec3f* vec, f32 x, f32 y, f32 z);

void ClearPolygon(void) {
    sNumPolygons = 0;
}

const char D_80110180[] = "\n";

#ifdef NON_MATCHING
void LoadPolygonsFromAsset(Collider* f) {
    ColliderAsset* colliderAsset;
    Vec3wi* triangleData;
    s32 numPolygons;
    s32 i;
    Poly* poly;
    f32 sinAlpha, cosAlpha, sinBeta, cosBeta;
    s32 j;
    Vec3f* pos;

    colliderAsset = f->asset;
    triangleData = colliderAsset->triangleData;
    numPolygons = colliderAsset->numPolygons;
    poly = &sPolygonArray[sNumPolygons];

    if (sNumPolygons + numPolygons > 0x400) {
        DummiedPrintf3("Too Many Polygons: %d\n", sNumPolygons + numPolygons);
        return;
    }

    sNumPolygons += numPolygons;
    f->polygons = poly;

    if (f->typeMaybe == COLLIDER_TYPE_8 ||
        f->typeMaybe == COLLIDER_TYPE_27 ||
        f->typeMaybe == COLLIDER_TYPE_25 ||
        f->typeMaybe == COLLIDER_TYPE_29 ||
        f->typeMaybe == COLLIDER_TYPE_34 ||
        f->typeMaybe == COLLIDER_TYPE_35 ||
        f->typeMaybe == COLLIDER_TYPE_7) {
        sinAlpha = sinf(f->unk_angle_rad);
        cosAlpha = cosf(f->unk_angle_rad);
    } else if (f->typeMaybe == COLLIDER_TYPE_33) {
        sinAlpha = sinf(DEGREES_TO_RADIANS_PI(f->unk_8C));
        cosAlpha = cosf(DEGREES_TO_RADIANS_PI(f->unk_8C));
        sinBeta = sinf(DEGREES_TO_RADIANS_PI(f->unk_90));
        cosBeta = cosf(DEGREES_TO_RADIANS_PI(f->unk_90));
    }

    for (i = 0; i < numPolygons; i++, triangleData++, poly++) {
        poly->unk_00 = 0;
        poly->unk_04 = f->unk_00;
        pos = &f->pos;
        for (j = 0; j < 3; j++) {
            Vec3f* vtxLocal = &colliderAsset->vertArray[triangleData->vertIndexes[j]];
            if (f->typeMaybe == COLLIDER_TYPE_33) {
                // rotate oround two axes
                f32 vx = vtxLocal->x * f->scale.x;
                f32 vy = vtxLocal->y * f->scale.y;
                f32 vz = vtxLocal->z * f->scale.z;
                // rotate around X axis
                f32 y1 = cosAlpha * vy - sinAlpha * vz;
                f32 newZ = sinAlpha * vy + cosAlpha * vz;
                // rotate around Y axis
                f32 z1 = cosBeta * newZ - sinBeta * vx;
                f32 x1 = sinBeta * newZ + cosBeta * vx;
                
                poly->vertices[j].x = pos->x + x1;
                poly->vertices[j].y = pos->y + y1;
                poly->vertices[j].z = pos->z + z1;
            } else if (f->typeMaybe == COLLIDER_TYPE_8 ||
                       f->typeMaybe == COLLIDER_TYPE_27 ||
                       f->typeMaybe == COLLIDER_TYPE_25 ||
                       f->typeMaybe == COLLIDER_TYPE_29 ||
                       f->typeMaybe == COLLIDER_TYPE_34 ||
                       f->typeMaybe == COLLIDER_TYPE_35 ||
                       f->typeMaybe == COLLIDER_TYPE_7) {
                // rotate around one axis
                switch (f->rotationType) {
                    case ROTATION_X:
                        {
                            f32 vy = vtxLocal->y * f->scale.y;
                            f32 vz = vtxLocal->z * f->scale.z;
                            poly->vertices[j].x = pos->x + (vtxLocal->x * f->scale.x);
                            poly->vertices[j].y = pos->y + (cosAlpha * vy - sinAlpha * vz);
                            poly->vertices[j].z = pos->z + (sinAlpha * vy + cosAlpha * vz);
                        }
                        break;
                    case ROTATION_Y:
                        {
                            f32 vz = vtxLocal->z * f->scale.z;
                            f32 vx = vtxLocal->x * f->scale.x;                            
                            poly->vertices[j].x = pos->x + (sinAlpha * vz + cosAlpha * vx);
                            poly->vertices[j].y = pos->y + (vtxLocal->y * f->scale.y);
                            poly->vertices[j].z = pos->z + (cosAlpha * vz - sinAlpha * vx);
                        }
                        break;
                    case ROTATION_Z:
                        {
                            f32 vx = vtxLocal->x * f->scale.x;
                            f32 vy = vtxLocal->y * f->scale.y;
                            poly->vertices[j].x = pos->x + (cosAlpha * vx - sinAlpha * vy);
                            poly->vertices[j].y = pos->y + (sinAlpha * vx + cosAlpha * vy);
                            poly->vertices[j].z = pos->z + (vtxLocal->z * f->scale.z);
                        }
                        break;
                }
            } else {
                // no rotation
                if (func_800B3FFC(f, 3) == 0 && func_800B3FFC(f, 2) != 0 || gCurrentStage == STAGE_VS) {
                    poly->vertices[j].x = pos->x + vtxLocal->x * f->scale.x;
                    poly->vertices[j].y = pos->y + vtxLocal->y * f->scale.y;
                    poly->vertices[j].z = pos->z + vtxLocal->z * f->scale.z;
                } else {
                    // no scale
                    poly->vertices[j].x = pos->x + vtxLocal->x;
                    poly->vertices[j].y = pos->y + vtxLocal->y;
                    poly->vertices[j].z = pos->z + vtxLocal->z;
                }
            }
        }

        switch (f->disp_type) {
            case COLLIDER_DISP_TYPE_7:
                poly->disp_type = COLLIDER_DISP_TYPE_7;
                break;
            case COLLIDER_DISP_TYPE_70:
                poly->disp_type = COLLIDER_DISP_TYPE_70;
                break;
            default:
                DummiedPrintf3("Unknown f->disp_type\n");
                break;
        }
    }
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/LoadPolygonsFromAsset.s")
void LoadPolygonsFromAsset(Collider* f);
#endif

void RegisterCollider(Collider* f) {
    Poly** pointerArray;
    Poly* polygons;
    s32 numPolygons;
    s32 i;

    if (f->polygons == NULL) {
        LoadPolygonsFromAsset(f);
    }
    pointerArray = &sPolygons[sNumListedPolygons];
    polygons = f->polygons;    
    numPolygons = f->asset->numPolygons;
    

    if (sNumListedPolygons + numPolygons > 0x400) {
        DummiedPrintf3("Too Many Listed Polygons\n");
        // no return
    }

    for (i = 0; i < numPolygons; i++) {
        *pointerArray++ = polygons++;
    }

    sNumListedPolygons += numPolygons;
}

void RegisterCollider_Copy(Collider* f, s32 unused_arg) {
    Poly** pointerArray;
    Poly* polygons;
    s32 numPolygons;
    s32 i;

    if (f->polygons == NULL) {
        LoadPolygonsFromAsset(f);
    }
    pointerArray = &sPolygons[sNumListedPolygons];
    polygons = f->polygons;    
    numPolygons = f->asset->numPolygons;    

    if (sNumListedPolygons + numPolygons > 0x400) {
        DummiedPrintf3("Too Many Listed Polygons\n");
        // no return
    }

    for (i = 0; i < numPolygons; i++) {
        *pointerArray++ = polygons++;
    }

    sNumListedPolygons += numPolygons;
}

void RegisterFirstCollider(Collider* f) {
    sNumListedPolygons = 0;
    RegisterCollider(f);
}

void RegisterNextCollider(Collider* f) {
    RegisterCollider(f);
}

//#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/RegisterCollidersIntersectingRect.s")
s32 RegisterCollidersIntersectingRect(Rect3D* rect, s32 disp_mask, s32 unk_mask) {
    s32 i;
    Collider** ptr;
    s32 total = 0;

    sNumListedPolygons = 0;
    ptr = D_80240898;
    for (i = 0; i < gFeildCount; i++, ptr++) {
        Collider* c = *ptr;

        if ((c->unk_114 & unk_mask) && (c->disp_type & disp_mask)) {
            if (IfRectsIntersect(rect, &c->bbox) == 0) {
                continue;
            }

            RegisterCollider(c);
            total++;
        }
        if (1) {// TODO: fake match
        }
    }
    if (ptr) { // TODO: fake match
    } 
    return total;
}

s32 func_800C982C(Rect3D* rect, PlayerActor* player) {
    Collider** ptr;
    s32 i;    
    s32 total = 0;

    sNumListedPolygons = 0;
    ptr = D_80240898;
    for (i = 0; i < gFeildCount; i++, ptr++) {
        Collider* c = *ptr;

        if (c->typeMaybe == COLLIDER_TYPE_33 && player->squishTimer != 0) {
            continue;
        }
        if ((c->unk_114 & 2) && (c->disp_type & 0x77)) {
            if (IfRectsIntersect(rect, &c->bbox) == 0) {
                continue;
            }
            if (i && i && i) { // TODO: fake match
            }
            RegisterCollider(c);
            total++;
        }
    }
    return total;
}

s32 func_800C9928(Rect3D* rect, s32 disp_mask, s32 unk_mask) {
    Collider** ptr;
    s32 i;    
    s32 total = 0;

    sNumListedPolygons = 0;
    ptr = D_80240898;
    if (!total && !total) { // TODO: fake match
    }
    for (i = 0; i < gFeildCount; i++, ptr++) {
        Collider* c = *ptr;

        if ((c->unk_114 & unk_mask) && c->unk_124 != 1 && (c->disp_type & disp_mask)) {
            if (IfRectsIntersect(rect, &c->bbox) == 0) {
                continue;
            }
            RegisterCollider(c);
            total++;
        }
    }
    return total;
}

s32 func_800C9A24(Rect3D* rect, s32 unused_arg, s32 disp_mask) {
    Collider** ptr;
    s32 i;    
    s32 total = 0;
    s32 disp_type;

    sNumListedPolygons = 0;
    ptr = D_80240898;
    for (i = 0; i < gFeildCount; i++, ptr++) {
        Collider* c = *ptr;

        disp_type = 0;
        if (c->unk_118 != 0) {
            disp_type |= COLLIDER_DISP_TYPE_7;
            if (!i) { // TODO: fake match
            }
        }
        if (c->unk_114 & 1) {
            disp_type |= COLLIDER_DISP_TYPE_70;
        }

        if (disp_type & disp_mask) {
            if (IfRectsIntersect(rect, &c->bbox) == 0) {
                continue;
            }
            RegisterCollider(c);
            total++;
        }
    }
    return total;
}

// Checks if Poly's bounding box intersects with the given rectangle
s32 IfPolyBoundIntersectsRect(Poly* poly, Rect3D* rect) {
    if (poly->unk_00 < 0) {
        return 0;
    }

    func_800D79E4(poly, 1);

    if (IfRectsIntersect(rect, &poly->boundBox) == 0) {
        return 0;
    }
    return 1;
}

f32 DistanceWithPoint(Vec3f point3D, Vec2f point2D) {
    return NORM_3(point3D.x - point2D.x, point3D.y - point2D.y, point3D.z);
}

// it is assumed that the line lies in the plane z=0 
void DistanceWithLine(Vec3f point, Vec2f linePoint1, Vec2f linePoint2, f32* distance, Vec3f* worldPos, Poly* poly) {
    Vec2f line;
    f32 det;
    f32 closestPointAlpha;
    Vec2f vec1;
    Vec2f unused;
    Vec2f closestPoint;
    Vec3f closestPoint3D;
    
    vec1.x = point.x - linePoint1.x;
    vec1.y = point.y - linePoint1.y;

    line.x = linePoint2.x - linePoint1.x;
    line.y = linePoint2.y - linePoint1.y;

    det = SUM_OF_SQUARES(line.x, line.y);

    if (det == 0.0f) {
        DummiedPrintf3("DistanceWithLine(): determinant is 0\n");
    }

    closestPointAlpha = (line.x * vec1.x + line.y * vec1.y) / det;

    if (closestPointAlpha <= 0.0f) {
        closestPoint = linePoint1;
    } else if (closestPointAlpha >= 1.0f) {
        closestPoint = linePoint2;
    } else {
        closestPoint.x = (1.0f - closestPointAlpha) * linePoint1.x + closestPointAlpha * linePoint2.x;
        closestPoint.y = (1.0f - closestPointAlpha) * linePoint1.y + closestPointAlpha * linePoint2.y;
    }

    *distance = DistanceWithPoint(point, closestPoint);

    closestPoint3D.x = closestPoint.x;
    closestPoint3D.y = closestPoint.y;
    closestPoint3D.z = 0.0f;
    LocalToWorld(worldPos, closestPoint3D, poly);
}

void MinimunDistance(Vec3f point, Poly* poly, f32 unused, f32* distance, s32* isOutside, Vec3f* closestPoint) {
    s32 type;
    f32 x_0, y_0;

    WorldToLocal(&point, point, poly);

    type = 0;
    x_0 = poly->triangularCoorsMatrix[0][0] * point.x + poly->triangularCoorsMatrix[1][0] * point.y;
    y_0 = poly->triangularCoorsMatrix[0][1] * point.x + poly->triangularCoorsMatrix[1][1] * point.y;
    if (x_0 < 0.0f) {
        type += 1;
    }
    if (y_0 < 0.0f) {
        type += 2;
    }
    if (x_0 + y_0 > 1.0f) {
        type += 4;
    }

    switch (type) {
        case 0:
            *closestPoint = point;
            closestPoint->z = 0.0f;
            LocalToWorld(closestPoint, *closestPoint, poly);
            *distance = point.z;
            *isOutside = FALSE;
            break;
        case 3:
            *closestPoint = poly->vertices[0];
            *distance = DistanceWithPoint(point, poly->verticesLocal[0]);
            *isOutside = TRUE;
            break;
        case 6:
            *closestPoint = poly->vertices[1];
            *distance = DistanceWithPoint(point, poly->verticesLocal[1]);
            *isOutside = TRUE;
            break;
        case 5:
            *closestPoint = poly->vertices[2];
            *distance = DistanceWithPoint(point, poly->verticesLocal[2]);
            *isOutside = TRUE;
            break;
        case 2:
            DistanceWithLine(point, poly->verticesLocal[0], poly->verticesLocal[1], distance, closestPoint, poly);
            *isOutside = TRUE;
            break;
        case 4:
            DistanceWithLine(point, poly->verticesLocal[1], poly->verticesLocal[2], distance, closestPoint, poly);
            *isOutside = TRUE;
            break;
        case 1:
            DistanceWithLine(point, poly->verticesLocal[2], poly->verticesLocal[0], distance, closestPoint, poly);
            *isOutside = TRUE;
            break;
        default:
            DummiedPrintf3("MinimunDistance(): invalid case\n");
            break;
    }
}

void ListUpTouchedPolygon(Vec3f point, f32 radius, s32 unused) {
    Poly** ptr;
    s32 isOutside;
    f32 distance;
    Vec3f closestPoint;
    TouchedPolygon* touched;    
    s32 i;
    f32 height;
    f32 dx, dy, dz;
    Rect3D box;

    sNumTouchedPolygons = 0;
    touched = sTouchedPolygons;
    box.min.x = point.x - radius;
    box.max.x = point.x + radius;
    box.min.y = point.y - radius;
    box.max.y = point.y + radius;
    box.min.z = point.z - radius;
    box.max.z = point.z + radius;

    ptr = sPolygons;
    for (i = 0; i < sNumListedPolygons; ptr++, i++) {
        Poly* poly = *ptr;
        
        func_800D79E4(poly, 1);
        if (!IfPolyBoundIntersectsRect(poly, &box)) {
            continue;
        }

        func_800D79E4(poly, 2);
        if (i && i && i) { // TODO: fake match            
        }
        dx = point.x - poly->vertices[0].x;
        dy = point.y - poly->vertices[0].y;
        dz = point.z - poly->vertices[0].z;
        height = dz * poly->rotationMatrix.normal.z + (dx * poly->rotationMatrix.normal.x + dy * poly->rotationMatrix.normal.y);
        // polygons are always one-sided
        if (height < 0 || height > radius) {
            continue;
        }

        func_800D79E4(poly, 3);
        if (poly->unk_00 < 0) {
            continue;
        }
        MinimunDistance(point, poly, radius, &distance, &isOutside, &closestPoint);
        if (distance > radius) {
            continue;
        }

        if (sNumTouchedPolygons >= 32) {
            DummiedPrintf3("ListUpTouchedPolygon(): Too Many\n");
        }
        
        touched->poly = poly;
        touched->isOutside = isOutside;
        touched->distance = distance;
        touched->height = height;
        touched->point = closestPoint;
        touched++;
        sNumTouchedPolygons++;
    }
}

TouchedPolygon* GetClosestTouchedPolygon(void) {
    s32 i;
    TouchedPolygon* best;
    TouchedPolygon* curr;

    if (sNumTouchedPolygons == 0) {
        return NULL;
    }
    if (sNumTouchedPolygons == 1) {
        return &sTouchedPolygons[0];
    }

    curr = &sTouchedPolygons[1];
    best = &sTouchedPolygons[0];    
    for (i = 1; i < sNumTouchedPolygons; curr++, i++) {
        if (curr->distance > best->distance) {
            continue;
        }
        // if two points outside of triangle and have same distance, prefer the one which is farther from the plane
        // in all other cases prefer the last one no matter if it's inside or outside
        if (curr->distance == best->distance && curr->isOutside && best->isOutside && curr->height < best->height) {
            continue;
        }
        
        best = curr;
    }

    return best;
}

void func_800CA4BC(TouchedPolygon* touched) {
    s32 i;
    TouchedPolygon* curr;

    if (touched->isOutside == FALSE) {
        return;
    }

    curr = sTouchedPolygons;
    for (i = 0; i < sNumTouchedPolygons; curr++, i++) {
        if (curr == touched) {
            continue;
        }

        if (IsOnPolygon(touched->point, curr->poly) && Vec3f_EqualsCopy(curr->poly->rotationMatrix.normal, touched->poly->rotationMatrix.normal)) {
            touched->isOutside = FALSE;
            return;
        }
    }
}

Vec3f* GetCollisionPointAtDistance(Vec3f* outVec, Vec3f point, TouchedPolygon* touched, f32 distance) {
    Vec3f touchPoint;
    Vec3f vec;
    f32 coef;
    f32 unused;
    Vec3f sp34;

    if (touched->isOutside) {
        touchPoint = touched->point;
        vec.x = point.x - touchPoint.x;
        vec.y = point.y - touchPoint.y;
        vec.z = point.z - touchPoint.z;

        coef = distance / NORM_3(vec.x, vec.y, vec.z);

        vec.x *= coef;
        vec.y *= coef;
        vec.z *= coef;

        point.x = touchPoint.x + vec.x;
        point.y = touchPoint.y + vec.y;
        point.z = touchPoint.z + vec.z;
    } else {
        Poly* poly = touched->poly;
        WorldToLocal(&sp34, point, poly);
        sp34.z = distance;
        LocalToWorld(&point, sp34, poly);
    }

    *outVec = point;
    return outVec;
}

Vec3f* GetCollisionPoint(Vec3f* collisionPoint, Vec3f point, f32 radius, s32 arg5) {
    Poly* lastPoly;
    s32 i;
    TouchedPolygon* closest;
    Vec3f origPoint;

    if (sNumListedPolygons == 0) {
        Vec3f_Zero(&D_802488B0);
        *collisionPoint = point;
        return collisionPoint;
    } else {
        origPoint = point;
        i = 0;
        lastPoly = NULL;
        for (; i < 4; i++) {
            ListUpTouchedPolygon(point, radius, arg5);
            if (sNumTouchedPolygons == 0) {
                break;
            }

            closest = GetClosestTouchedPolygon();
            if (closest->poly == lastPoly) {
                break;
            }

            func_800CA4BC(closest);
            // it doesn't make sense
            // we got the polygon which is at distance < radius
            // why do we find the point which is exactly at distance = radius
            // this point is further from polygon than the original point
            GetCollisionPointAtDistance(&point, point, closest, radius);
            lastPoly = closest->poly;
        }

        D_802488B0.x = point.x - origPoint.x;
        D_802488B0.y = point.y - origPoint.y;
        D_802488B0.z = point.z - origPoint.z;
        *collisionPoint = point;
        return collisionPoint;
    }
}

#ifdef NON_MATCHING
Poly* SearchPolygonBetween(Vec3f vec1, Vec3f vec2, s32 arg3, s32 oneSided, s32 findClosest) {
    Rect3D bbox;
    f32 lowestRatio;
    Vec3f retVec;
    Vec3f vec1_rel;
    Vec3f vec2_rel;
    Vec3f intersectionLocal;
    Poly* bestPoly;
    Poly* curr;
    Poly** ptr;
    s32 i;
    f32 height1;
    f32 height2;
    f32 ratio;
    f32 midX, midY, midZ;

    if (sNumListedPolygons == 0) {
        return NULL;
    }

    lowestRatio = 1.0f;
    bestPoly = NULL;
    CalculateBoundingRectFromVectors(vec1, vec2, &bbox);
    
    ptr = sPolygons;
    for (i = 0; i < sNumListedPolygons; ptr++, i++) {
        curr = *ptr;

        func_800D79E4(curr, 1);
        if (!IfPolyBoundIntersectsRect(curr, &bbox)) {
            continue;
        }

        func_800D79E4(curr, 2);
        if (oneSided) {
            // first point above polygon, second point below polygon
            vec1_rel.x = vec1.x - curr->vertices[0].x;
            vec1_rel.y = vec1.y - curr->vertices[0].y;
            vec1_rel.z = vec1.z - curr->vertices[0].z;
            height1 = vec1_rel.z * curr->rotationMatrix.normal.z + (vec1_rel.x * curr->rotationMatrix.normal.x + vec1_rel.y * curr->rotationMatrix.normal.y);
            if (height1 <= 0.0) {
                continue;
            }
            vec2_rel.x = vec2.x - curr->vertices[0].x;
            vec2_rel.y = vec2.y - curr->vertices[0].y;
            vec2_rel.z = vec2.z - curr->vertices[0].z;
            height2 = vec2_rel.z * curr->rotationMatrix.normal.z + (vec2_rel.x * curr->rotationMatrix.normal.x + vec2_rel.y * curr->rotationMatrix.normal.y);
            if (height2 >= 0.0) {
                continue;
            }
        } else {
            vec1_rel.x = vec1.x - curr->vertices[0].x;
            vec1_rel.y = vec1.y - curr->vertices[0].y;
            vec1_rel.z = vec1.z - curr->vertices[0].z;
            height1 = vec1_rel.z * curr->rotationMatrix.normal.z + (vec1_rel.x * curr->rotationMatrix.normal.x + vec1_rel.y * curr->rotationMatrix.normal.y);
            vec2_rel.x = vec2.x - curr->vertices[0].x;
            vec2_rel.y = vec2.y - curr->vertices[0].y;
            vec2_rel.z = vec2.z - curr->vertices[0].z;
            height2 = vec2_rel.z * curr->rotationMatrix.normal.z + (vec2_rel.x * curr->rotationMatrix.normal.x + vec2_rel.y * curr->rotationMatrix.normal.y);
            if (height1 * height2 >= 0.0) {
                // two point on the same side
                continue;
            }
        }

        if (height1 == height2) {
            ratio = 0.5f;
        } else {
            ratio = height1 / (height1 - height2);
        }
        if (ratio < 0.0) {
            DummiedPrintf3("SearchPolygonBetween(): ratio < 0\n");
            ratio = 1.0f;
        }
        if (ratio > 1.0) {
            DummiedPrintf3("SearchPolygonBetween(): ratio > 1\n");
            ratio = 1.0f;
        }

        if (ratio >= lowestRatio) {
            continue;
        }
        
        func_800D79E4(curr, 3);
        if (curr->unk_00 < 0) {
            continue;
        }

        midX = vec2_rel.x * ratio + vec1_rel.x * (1.0 - ratio);
        midY = vec2_rel.y * ratio + vec1_rel.y * (1.0 - ratio);
        midZ = vec2_rel.z * ratio + vec1_rel.z * (1.0 - ratio);
        // world to local
        intersectionLocal.x = curr->rotationMatrix.vec1.x * midX + curr->rotationMatrix.vec1.y * midY + curr->rotationMatrix.vec1.z * midZ;
        intersectionLocal.y = curr->rotationMatrix.vec2.x * midX + curr->rotationMatrix.vec2.y * midY + curr->rotationMatrix.vec2.z * midZ;
        intersectionLocal.z = 0.0f;
        
        if (!IsInsidePolygon(intersectionLocal, curr)) {
            continue;
        }

        lowestRatio = ratio;
        bestPoly = curr;
        retVec = intersectionLocal;
        if (findClosest != TRUE) {
            break;
        }
    }

    if (bestPoly != NULL) {
        // local to world
        bestPoly->intersection.x = bestPoly->vertices[0].x + (retVec.x * curr->rotationMatrix.vec1.x + retVec.y * curr->rotationMatrix.vec2.x);
        bestPoly->intersection.y = bestPoly->vertices[0].y + (retVec.x * curr->rotationMatrix.vec1.y + retVec.y * curr->rotationMatrix.vec2.y);
        bestPoly->intersection.z = bestPoly->vertices[0].z + (retVec.x * curr->rotationMatrix.vec1.z + retVec.y * curr->rotationMatrix.vec2.z);
    }

    return bestPoly;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/SearchPolygonBetween.s")
Poly* SearchPolygonBetween(Vec3f vec1, Vec3f vec2, s32 arg3, s32 oneSided, s32 findClosest);
#endif

s32 RayCastBetween(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32* iX, f32* iY, f32* iZ) {
    Rect3D rect;
    Vec3f vecA;
    Vec3f vecB;
    Poly* poly;
    
    Vec3f_Set(&vecA, x1, y1, z1);
    Vec3f_Set(&vecB, x2, y2, z2);

    if (x1 < x2) {
        rect.min.x = x1;
        rect.max.x = x2;
    } else {
        rect.min.x = x2;
        rect.max.x = x1;
    }
    if (y1 < y2) {
        rect.min.y = y1;
        rect.max.y = y2;
    } else {
        rect.min.y = y2;
        rect.max.y = y1;
    }
    if (z1 < z2) {
        rect.min.z = z1;
        rect.max.z = z2;
    } else {
        rect.min.z = z2;
        rect.max.z = z1;
    }

    RegisterCollidersIntersectingRect(&rect, COLLIDER_DISP_TYPE_7 | COLLIDER_DISP_TYPE_70, 2);
    poly = SearchPolygonBetween(vecA, vecB, COLLIDER_DISP_TYPE_7 | COLLIDER_DISP_TYPE_70, FALSE, TRUE);
    if (poly == NULL) {
        return FALSE;
    }
    *iX = poly->intersection.x;
    *iY = poly->intersection.y;
    *iZ = poly->intersection.z;
    return TRUE;
}

#ifdef NON_MATCHING
Poly* SearchPolyBelow(Vec3f point, f32 maxYOffset, f32 minYOffset) {
    s32 i;
    Vec3f highPoint;
    Vec3f lowPoint;    
    Poly** ptr;
    Poly* retPoly;
    f32 dx, dz, dy;
    f32 lowY;
    Rect3D* bbox;
    Vec3f localIntersection;

    if (sNumListedPolygons == 0) {
        return NULL;
    }

    retPoly = NULL;
    highPoint = point;
    highPoint.y += maxYOffset;
    lowPoint = point;
    lowPoint.y += minYOffset;
    lowY = lowPoint.y;

    ptr = sPolygons;
    for (i = 0; i < sNumListedPolygons; ptr++, i++) {
        Poly* curr = *ptr;

        func_800D79E4(curr, 1);
        bbox = &curr->boundBox;
        if (point.x < bbox->min.x ||
            point.x > bbox->max.x ||
            point.z < bbox->min.z ||
            point.z > bbox->max.z ||
            highPoint.y < bbox->min.y ||
            lowY > bbox->max.y)
        {
            continue;
        }

        // need only polygons facing up
        func_800D79E4(curr, 2);
        if (curr->rotationMatrix.normal.y <= 0.0) {
            continue;
        }

        // project point down on the polygon
        dx = point.x - curr->vertices[0].x;
        dz = point.z - curr->vertices[0].z;
        dy = -(curr->rotationMatrix.normal.x * dx + curr->rotationMatrix.normal.z * dz) / curr->rotationMatrix.normal.y;
        if (curr->vertices[0].y + dy < lowY || curr->vertices[0].y + dy > highPoint.y) {
            continue;
        }

        func_800D79E4(curr, 3);
        if (curr->unk_00 < 0) {
            continue;
        }

        // world to local
        localIntersection.x = curr->rotationMatrix.vec1.z * dz +  (curr->rotationMatrix.vec1.x * dx + curr->rotationMatrix.vec1.y * dy);
        localIntersection.y = curr->rotationMatrix.vec2.z * dz +  (curr->rotationMatrix.vec2.x * dx + curr->rotationMatrix.vec2.y * dy);
        localIntersection.z = 0.0f;
        if (!IsInsidePolygon(localIntersection, curr)) {
            continue;
        }

        retPoly = curr;
        curr->intersection.y = lowY = curr->vertices[0].y + dy;
        curr->intersection.x = curr->vertices[0].x + dx;        
        curr->intersection.z = curr->vertices[0].z + dz;
    }

    return retPoly;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/SearchPolyBelow.s")
Poly* SearchPolyBelow(Vec3f point, f32 maxYOffset, f32 minYOffset);
#endif

Poly* func_800CB294(Vec3f point, f32 arg3) {
    f32 d = arg3 * 20.0;
    f32 unused[1];
    Vec3f sp44;
    Rect3D rect;    

    sp44.x = point.x;
    sp44.y = point.y - d;
    sp44.z = point.z;

    CalculateBoundingRectFromVectors(point, sp44, &rect);
    rect.max.y += 10.0;
    RegisterCollidersIntersectingRect(&rect, 0x77, 2);
    return SearchPolyBelow(point, 10.0f, -d);
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/Shadows_Reset.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/Shadows_Set.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CB99C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBB2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBB98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBC08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBD24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBE74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBF54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/Shadows_Draw_AntQueen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/Shadows_Draw.s")

/**
 * @brief Calculates the angle of the (x,z) vector from a 3dim vector, with respect to the positive z-axis.
 * 
 * @param vec: The 3dim vector to calculate the angle of. 
 */
void func_800CC7E0(Vec3f vec) {
    // In this instance the z component is flipped.
    CalculateAngleOfVector(vec.x, -vec.z);
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CC814.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/CalcThrownEnemyNext.s")

void func_800CCDCC(Actor* arg0) {
    Vec3f sp24;

    sp24.x = arg0->unknownPositionThings[0].unk_00 + arg0->pos.x;
    sp24.y = arg0->pos.y;
    sp24.z = arg0->unknownPositionThings[0].unk_08 + arg0->pos.z;
    arg0->unk_98 = 0;
    arg0->unk_9C = 0;
    func_800CC814(arg0, sp24, 1);
    func_800CBC08(arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CCE4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/CalcWalkingEnemyNext.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/CalcJumpingEnemyNext.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CEB10.s")

void CalcEnemyNextPosition(Actor* arg0) {
   switch (arg0->unk_A0.unk_04) {
   case 0:
       func_800CCDCC(arg0);
       break;
   case 1:
       func_800CCE4C(arg0);
       break;
   case 2:
       CalcWalkingEnemyNext(arg0);
       break;
   case 3:
       CalcJumpingEnemyNext(arg0);
       break;
   case 4:
       func_800CEB10(arg0);
       break;
   default:
       DummiedPrintf3("CalcEnemyNextPosition(): Unknown ATR_IDOU_XXXX\n");
       // double 810000
       break;
   }
   if (arg0->tongueCollision >= 2) {
       func_800CBD24(arg0);
   }
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CF080.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CF3B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CF654.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CF808.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CFB6C.s")

void func_800CFDB8(PlayerActor* arg0) {
    arg0->vaulting = 0;
    arg0->surface = -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CFDC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CFE14.s")

void func_800CFF64(PlayerActor* player) {
    player->surfaceSlide = 0;
    player->move.z = 0.0f;
    player->move.y = 0.0f;
    player->move.x = 0.0f;
}

void func_800CFF7C(Vec3f* arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f14;
    Rect3D* temp_v0;

    if (D_80236974 != 1) {
        if ((gCurrentStage == 1) && ((gCurrentZone == 7) || (gCurrentZone == 0xF)) && (levelFlags[0] != 0)) {
            temp_f14 = SUM_OF_SQUARES(arg0->x, arg0->z);
            if (810000.0 < temp_f14) {
                temp_f0_2 = sqrtf((f32) (810000.0 / temp_f14));
                arg0->x = arg0->x * temp_f0_2;
                arg0->z = arg0->z * temp_f0_2;
            }
        } else {
            temp_v0 = &gZoneCollisions[gCurrentZone].rect_30;
            
            if (temp_v0->min.x > arg0->x ) {
                arg0->x = temp_v0->min.x;
            }
            
            if (temp_v0->max.x < arg0->x) {
                arg0->x = temp_v0->max.x;
            }

            if (temp_v0->min.z > arg0->z) {
                arg0->z = temp_v0->min.z;
            }
  
            if (temp_v0->max.z < arg0->z) {
                arg0->z = temp_v0->max.z;
            }
        }
    }
}


Vec3f* func_800D00DC(Vec3f* arg0, Collider* arg1) {
    Vec3f sp24;
    Collider* temp_v1;

    if (arg1->unk80 < 0) {
        Vec3f_Zero(&sp24);
    } else {
        temp_v1 = &D_80236980[arg1->unk80];
        if (((temp_v1->typeMaybe == 0x14) || (temp_v1->typeMaybe == 0x15) || (temp_v1->typeMaybe == 0x16)) && (arg1->unk_11C != 4)) {
            sp24.x = temp_v1->unk_8C;
            sp24.y = temp_v1->unk_90;
            sp24.z = temp_v1->unk_94;
        } else {
            Vec3f_Zero(&sp24);
        }
    }
    *arg0 = sp24;
    return arg0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D01A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D0448.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D04B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D0694.s")
//referred to in US1.0 as "Poly.c CalcNextPosition"
#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/CalcNextPosition.s")

void func_800D34CC(void) {
    s32 i;

    Vec3f_Zero(&D_80108F9C);
    Vec3f_Zero(&D_80108FA8);
    Vec3f_Zero(&D_80108FB4);
    Vec3f_Zero(&D_80108FC0);
    D_80108FCC = 0.0f;
    D_80108FD0 = 0;
    D_80108FD4 = 0;
    D_80108FD8 = 0.0f;
    D_80108FDC = 0.0f;
    D_80108FE0 = 0;
    D_80108FE4 = 0.0f;
    D_80108FE8 = 0;
    D_80108FEC = 0;
    for (i = 0; i < ARRAY_COUNT(D_802489C8); i++) {
        Vec3f_Zero(&D_802489C8[i]);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D3590.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D363C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D3854.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D3D80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D3FC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D4200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D44C8.s")

void func_800D4550(s32 arg0, s32 arg1, Poly* arg2, Vec3f* arg3, Vec3f* arg4) {
    Collision* temp_v0 = &gZoneCollisions[gCurrentZone];

    arg3->x = temp_v0->unkA4;
    arg3->y = temp_v0->unkA8 + (temp_v0->unkD0 * arg2->rotationMatrix.vec1.x);
    arg3->z = temp_v0->unkAC;
    arg4->x = temp_v0->unk98;
    arg4->y = temp_v0->unk9C + (temp_v0->unkD0 * arg2->rotationMatrix.vec1.x);
    arg4->z = temp_v0->unkA0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D45D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D4720.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D4D90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D4F50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D52E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D5394.s")

void func_800D6864(PlayerActor* arg0, Tongue* arg1, Camera* arg2, Vec3f* arg3, Vec3f* arg4) {
    Collision* collider;

    collider = &gZoneCollisions[gCurrentZone];
    arg3->x = arg2->f1.z;
    arg3->y = arg2->f2.x + (collider->unkD0 * arg2->size1);
    arg3->z = arg2->f2.y;
    arg4->x = arg2->f3.x;
    arg4->y = arg2->f3.y + (collider->unkD0 * arg2->size1);
    arg4->z = arg2->f3.z;
}

void ApplyRotationToVector(Vec3f* vecA, Vec3f* vecB, f32 degreesAngle) {
    Vec3f differenceVector;

    // Calculate the difference vector between the two vectors
    differenceVector.x = vecA->x - vecB->x;
    differenceVector.y = vecA->y - vecB->y;
    differenceVector.z = vecA->z - vecB->z;
    
    // Rotate the difference vector by the given angle around the y-axis
    RotateVector3D(&differenceVector, differenceVector, DEGREES_TO_RADIANS_PI(degreesAngle), 2);
    
    // Add the rotated difference vector to the second vector to get the first vector
    vecA->x = vecB->x + differenceVector.x;
    vecA->y = vecB->y + differenceVector.y;
    vecA->z = vecB->z + differenceVector.z;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D69D0.s")

void SetCameraParameters(void) {
    Collision* temp = &gZoneCollisions[gCurrentZone];
    Camera* cam;
    s32 pad;
    s32 i;
    Vec3f sp3C;
    Vec3f sp30;

    if ((gCurrentStage == STAGE_GHOST) && (gCurrentZone == 0xE)) { //billiard room?
        func_800D6864(gPlayerActors, gTongues, gCamera, &sp3C, &sp30);
    } else if ((D_80236974 == 1) && (D_8020D8F4 == 0)) {
        func_800D3854(gPlayerActors, gTongues, gCamera, &sp3C, &sp30, 0);
    } else if (gCamera[0].unk0 == 1) {
        func_800D69D0(temp->unk94, gPlayerActors, gTongues, gCamera, &sp3C, &sp30, 0);
    } else {
        func_800D5394(gPlayerActors, gTongues, gCamera, &sp3C, &sp30, 0);
    }
    
    cam = &gCamera[0];
    
    for (i = 0; i < 4; i++, cam++) {
        cam->f5.x = sp3C.x;
        cam->f5.y = sp3C.y;
        cam->f5.z = sp3C.z;

        cam->f4.x = sp30.x;
        cam->f4.y = sp30.y;
        cam->f4.z = sp30.z;
        if (gCurrentStage != STAGE_VS) {
            break;
        }
    }
}

/* Create a rectangle using two vectors then perform an unknown operation */
void func_800D71E8(f32 x1, f32 x2, f32 y1, f32 y2, f32 z1, f32 z2) {
    Rect3D r;

    // define a rectangle with the given bounds
    r.min.x = x1;
    r.max.x = x2;
    r.min.y = y1;
    r.min.z = z1;
    r.max.y = y2;
    r.max.z = z2;
    
    // ensure max > min
    OrderRectBounds(&r);
    RegisterCollidersIntersectingRect(&r, 0x77, 2); //unknown
}

s32 func_800D7248(f32 x, f32 y, f32 z, f32 arg3, f32 arg4, f32* outX, f32* arg6, f32* arg7) {
    Vec3f vec;
    s32 var_v1;
    Poly* poly;

    vec.x = x;
    vec.y = y;
    vec.z = z;
    
    poly = SearchPolyBelow(vec, arg3, arg4);
    
    // if a collider was found assign its position to the output variables then return 1 for success
    return (poly != NULL) ?
        *outX = poly->intersection.x,
        *arg6 = poly->intersection.y,
        *arg7 = poly->intersection.z,
        1 :
        0;
}

s32 func_800D72DC(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32* outX, f32* outY, f32* outZ) {
    Vec3f vecOne;
    Vec3f vecTwo;
    Poly* poly;

    vecOne.x = x1;
    vecOne.y = y1;
    vecOne.z = z1;

    vecTwo.x = x2;
    vecTwo.y = y2;
    vecTwo.z = z2;
    
    poly = SearchPolygonBetween(vecOne, vecTwo, 0x77, 1, 1);
    return (poly != NULL) ?
        *outX = poly->intersection.x,
        *outY = poly->intersection.y,
        *outZ = poly->intersection.z,
        1 :
        0;
}

void func_800D73BC(f32* x, f32* y, f32* z, f32 arg3) {
    Vec3f destVec;
    Vec3f srcVec;

    srcVec.x = *x;
    srcVec.y = *y;
    srcVec.z = *z;
    
    //wrong number of args(?)
    GetCollisionPoint(&destVec, srcVec, arg3, 0x77);
    
    *x = destVec.x;
    *y = destVec.y;
    *z = destVec.z;
}
