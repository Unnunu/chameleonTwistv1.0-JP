#include "common.h"

// Auth: Rain, revo, simonlindholm
void DummiedPrintf3(char* arg0, ...) { /* variadic args: simonlindholm*/
}

/*
 * WrapAngle: Wraps an angle to the range [0, 360).
 *      @param angle: pointer to the angle to wrap.
 * 
 *      @return: (the wrapped angle).
 */

#pragma GLOBAL_ASM("asm/nonmatchings/code/vector/WrapAngle.s")
// void WrapAngle(f32* arg0) {
//     if (*arg0 < 0.0) {
//         *arg0 = (*arg0 + 360.0);
//         return;
//     }

//     if (360.0 <= *arg0) {
//         *arg0 = (*arg0 - 360.0);
//     }
// }

/*
 * CompareWrappedAngles: Compares two angles, wrapping them to the range [0, 360) before comparing.
 *      @param angle1: the first angle to compare.
 *      @param angle2: the second angle to compare.
 * 
 *      @return: 1 if angle1 is greater than angle2, -1 if angle1 is less than angle2, and 0 if they are equal.
 */

#pragma GLOBAL_ASM("asm/nonmatchings/code/vector/CompareWrappedAngles.s")
// s32 CompareWrappedAngles(f32 arg0, f32 arg1) {
//     s32 ret;
    
//     WrapAngle(&arg0);
//     WrapAngle(&arg1);
    
//     if (arg0 == arg1) {
//         ret = 0;
//     } else {
//         if (arg0 < 180.0) {
//             if ((arg0 < arg1) && (arg1 <= (arg0 + 180.0))) {
//                 ret = 1;
//             } else {
//                 ret = -1;
//             }
//         } else {
//             if (((arg0 - 180.0) < arg1) && (arg1 <= arg0)) {
//                 ret = -1;
//             } else {
//                 ret = 1;
//             }
//         }
//     }

//     return ret;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/code/vector/func_800D75B4.s")

void OnlyCheckPolyInfoLevel(Poly* arg0, s32 arg1, char* arg2) {
    if (arg0->unk_00 < arg1) {
        DummiedPrintf3(D_8011074C);
        DummiedPrintf3(D_8011077C, arg2);
    }
}

void func_800D79E4(Poly* arg0, s32 arg1) {
    s32 i = arg0->unk_00 + 1;
    if (arg0->unk_00 < arg1) {
        for (; arg1 >= i; i++) {
            func_800D75B4(arg0, i);
        }        
    }
}

/*
 * ProjectOnPolygon: Projects a given 3D vector onto a polygon in 3D space, returning the projected vector.
 *      @param[in,out]  vec:    the vector to project.
 *      @param          perspX: the X coordinate of the perspective point.
 *      @param          perspY: the Y coordinate of the perspective point.
 *      @param          perspZ: the Z coordinate of the perspective point.
 *      @param          poly:   the polygon to project onto.
 * 
 *      @return: the projected vector.
 */

Vec3f* ProjectOnPolygon(Vec3f* vec, f32 perspX, f32 perspY, f32 perspZ, Poly* poly) {
    Vec3f vec_proj;
    f32 p_x;
    f32 dotProduct;
    f32 dist;
    f32 p_x2;

    OnlyCheckPolyInfoLevel(poly, 2, D_8011078C);
    p_x = poly->x;
    p_x2 = poly->x2;
    dotProduct = (poly->z * perspZ) + ((perspX * p_x) + (perspY * poly->y));
    dist = (poly->z2 * perspZ) + ((perspX * p_x2) + (perspY * poly->y2));
    vec_proj.x = (p_x2 * dist) + (dotProduct * p_x);
    vec_proj.y = (poly->y2 * dist) + (dotProduct * poly->y);
    vec_proj.z = (poly->z2 * dist) + (dotProduct * poly->z);
    *vec = vec_proj;
    return vec;
}

/* 
 * WorldToLocal: Converts a 3D vector from world space to local space. 
 *      @param[in,out]  outVec: Pointer to a Vec3f where the resulting local-space vector will be stored.
 *      @param          vec:    The 3D vector to be converted from world space to local space.
 *      @param          poly:   The polygon with respect to which the conversion should be performed.
 *
 *      returns: A pointer to the resulting local-space vector, stored in the `outVec` parameter.
 */

Vec3f* WorldToLocal(Vec3f* outVec, Vec3f vec, Poly* poly) {
    // Take P to be a matrix with the columns being the x, y, and z vectors of the poly struct
    // P(v) = outVec, where v is the input vector agter being translated by an offset vector
    Vec3f temp_vec;
 
    OnlyCheckPolyInfoLevel(poly, 2, D_801107A0);
    vec.x = vec.x - poly->offset_x;
    vec.y = vec.y - poly->offset_y;
    vec.z = vec.z - poly->offset_z;
    temp_vec.x = (poly->z * vec.z) + ((vec.x * poly->x) + (vec.y * poly->y));
    temp_vec.y = (poly->z2 * vec.z) + ((vec.x * poly->x2) + (vec.y * poly->y2));
    temp_vec.z = (poly->z3 * vec.z) + ((vec.x * poly->x3) + (vec.y * poly->y3));
    *outVec = temp_vec;

    return outVec;
}

/* 
 * LocalToWorld: Converts a 3D vector from local space to world space. 
 *      @param[in,out] outVec:     Pointer to a Vec3f where the resulting world-space vector will be stored.
 *      @param         vec:        The 3D vector to be converted from local space to world space.
 *      @param         poly:       The polygon with respect to which the conversion should be performed.
 *
 *      @return: A pointer to the resulting world-space vector, stored in the `outVec` parameter.
 */

Vec3f* LocalToWorld(Vec3f* outVec, Vec3f vec, Poly* poly) {
    Vec3f temp_vec;

    OnlyCheckPolyInfoLevel(poly, 2, D_801107B0);
    temp_vec.x = (poly->x3 * vec.z) + ((vec.x * poly->x) + (vec.y * poly->x2));
    temp_vec.y = (poly->y3 * vec.z) + ((vec.x * poly->y) + (vec.y * poly->y2));
    temp_vec.z = (poly->z3 * vec.z) + ((vec.x * poly->z) + (vec.y * poly->z2));
    temp_vec.x += poly->offset_x;
    temp_vec.y += poly->offset_y;
    temp_vec.z += poly->offset_z;
    *outVec = temp_vec;
    return outVec;
}

/*
 * IsInsidePolygon: Checks if a given 3D vector is inside a polygon in 3D space.
 *      @param vec:  the vector to check.
 *      @param poly: the polygon to check against.
 * 
 *      @return: (s32 bool) 1 if the vector is inside the polygon, 0 otherwise.
 */

#pragma GLOBAL_ASM("asm/nonmatchings/code/vector/IsInsidePolygon.s")
//s32 IsInsidePolygon(Vec3f vec, Poly* poly) {
//    f32 x_0;
//    f32 y_0;
//    OnlyCheckPolyInfoLevel(poly, 3, D_801107C0);
//    x_0 = (poly->unk_74 * vec.y) + (poly->unk_6C * vec.x);
//    y_0 = (poly->unk_78 * vec.y) + (poly->unk_70 * vec.x);
//    if (x_0 < -0.0001) {
//        return 0;
//    }
//    if (y_0 < -0.0001) {
//        return 0;
//    }
//    if (1.0001 < (x_0 + y_0)) {
//        return 0;
//    }
//    return 1;
//}

/*
 * IsOnPolygon: Uses the dot product of the point and the polygon's normal vector to determine if the point is on the polygon
 *     @param vec:  the point to check
 *     @param poly: the polygon to check
 * 
 *     @return: (s32 bool) 1 if the point is on the polygon, 0 otherwise
 */

s32 IsOnPolygon(Vec3f vec, Poly* poly) {
    f32 dotProduct;
    
    OnlyCheckPolyInfoLevel(poly, 2, D_801107D0);
    vec.x -= poly->offset_x;
    vec.y -= poly->offset_y;
    vec.z -= poly->offset_z;
    
    dotProduct = vec.z * poly->z3 + (vec.x * poly->x3 + vec.y * poly->y3);

    if (dotProduct < -1.0) {
        return 0;
    }
    if (dotProduct > 1.0) {
        return 0;
    }
    return 1;
}

/*
 * RotateVector3D: Calculates the result of a given rotation matrix multiplied by a given vector
 *     @param[in,out]   outVec:                 pointer to the output vector
 *     @param           inpVec:                 the input vector
 *     @param           theta:                  the angle of rotation
 *     @param           rotateAroundAxesIndex:  the axis to rotate around / the rotation matrix to use
 * 
 *     @return: the vector after the rotation
*/

Vec3f* RotateVector3D(Vec3f* outVec, Vec3f inpVec, f32 theta, s32 rotateAroundAxesIndex) {
    #define NO_ROTATION 0
    #define X_ROTATION 1
    #define Y_ROTATION 2
    #define Z_ROTATION 3
    Vec3f temp_vec;
    f32 sin;
    f32 cos;

    sin = __sinf(theta);
    cos = __cosf(theta);
    switch (rotateAroundAxesIndex) {
    case NO_ROTATION:
        temp_vec = inpVec;
        break;
    case X_ROTATION:
        temp_vec.x = inpVec.x;
        temp_vec.y = (cos * inpVec.y) - (sin * inpVec.z);
        temp_vec.z = (sin * inpVec.y) + (cos * inpVec.z);
        break;
    case Y_ROTATION:
        temp_vec.y = inpVec.y;
        temp_vec.x = (sin * inpVec.z) + (cos * inpVec.x);
        temp_vec.z = (cos * inpVec.z) - (sin * inpVec.x);
        break;
    case Z_ROTATION:
        temp_vec.z = inpVec.z;
        temp_vec.x = (cos * inpVec.x) - (sin * inpVec.y);
        temp_vec.y = (sin * inpVec.x) + (cos * inpVec.y);
        break;
    }
    
    *outVec = temp_vec;
    return outVec;
}


/*
 * IsNearPoint: checks if a point is within a certain radius of another point
 *     @param   vec1:           first vector
 *     @param   vec2:           second vector
 *     @param   approxRadius:   radius to check
 * 
 *     @return: (s32 bool) 1 if within radius, 0 if not
 */

s32 IsNearPoint(Vec3f vec1, Vec3f vec2, f32 approxRadius) {
    f32 x_0;
    f32 z_0;
    f32 y_0;
    s32 ret;

    // Find a vector to represent the distance between the two points
    x_0 = vec1.x - vec2.x;
    y_0 = vec1.y - vec2.y;
    z_0 = vec1.z - vec2.z;
    
    if (NORM_3(x_0, y_0, z_0) < approxRadius) {     // Check if said vector is within the given radius from the origin
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}

/*
 * Vec3f_Equals | Vec3f_EqualsCopy: compares two Vec3f structs  [Both are neccessary for checksum]
 *     @param vec1: first vector
 *     @param vec2: second vector
 * 
 *     @return: (s32 bool) 1 if equal, 0 if not
 */

s32 Vec3f_Equals(Vec3f vec1, Vec3f vec2) {
    return ((vec1.x == vec2.x) && (vec1.y == vec2.y) && (vec1.z == vec2.z)) ? 1 : 0;
}

s32 Vec3f_EqualsCopy(Vec3f vec1, Vec3f vec2) {
    return ((vec1.x == vec2.x) && (vec1.y == vec2.y) && (vec1.z == vec2.z)) ? 1 : 0;
}

/*
 * Vec3f_Set: sets a vector to have given values
 *     @param   vec:        pointer for vector to set
 *     @params  x, y, z:    vector components
 */

void Vec3f_Set(Vec3f* vec, f32 x, f32 y, f32 z) {
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

/*
 * Vec3f_Zero: sets a vector to the zero vector
 *     @param vec: pointer for vector to set
 */

void Vec3f_Zero(Vec3f* vec) {
    vec->x = 0.0f;
    vec->y = 0.0f;
    vec->z = 0.0f;
}