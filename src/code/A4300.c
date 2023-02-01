#include "common.h"
//AOF=82

void func_800CBC08(unkA4300*);
void func_800CC814(unkA4300*, Vec3f, s32);

void func_800C8F00(void) {
    D_80236968 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800C8F0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800C9504.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800C9600.s")

void func_800C9704(void) {
    D_8023696C = 0;
    func_800C9504();
}

void func_800C9728(void) {
    func_800C9504();
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800C9748.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800C982C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800C9928.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800C9A24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800C9B18.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800C9B7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800C9BD8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800C9DB0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CA194.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CA3FC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CA4BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CA5B4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CA734.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CA8F8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CAE08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CAF88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CB294.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CB3A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CB470.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CB99C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CBB2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CBB98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CBC08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CBD24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CBE74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CBF54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CBFB4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CC2F4.s")

void func_800CC7E0(Vec3f arg0) {
    func_800C8C14(arg0.x, -arg0.z);
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CC814.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CCA00.s")

void func_800CCDCC(unkA4300* arg0) {
    Vec3f sp24;

    sp24.x = arg0->unk_54 + arg0->unk_24;
    sp24.y = arg0->unk_28;
    sp24.z = arg0->unk_5C + arg0->unk_2C;
    arg0->unk_98 = 0;
    arg0->unk_9C = 0;
    func_800CC814(arg0, sp24, 1);
    func_800CBC08(arg0);
}


#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CCE4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CD634.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CDF0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CEB10.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CEFC4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CF080.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CF3B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CF654.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CF808.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CFB6C.s")

void func_800CFDB8(playerActor* player) {
    player->vaulting = 0;
    player->surface = -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CFDC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CFE14.s")

void func_800CFF64(playerActor* player) {
    player->surfaceSlide = 0;
    player->zMove = 0.0f;
    player->yMove = 0.0f;
    player->xMove = 0.0f;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800CFF7C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D00DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D01A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D0448.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D04B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D0694.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D0708.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D34CC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D3590.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D363C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D3854.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D3D80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D3FC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D4200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D44C8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D4550.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D45D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D4720.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D4D90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D4F50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D52E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D5394.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D6864.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D68EC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D69D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D7028.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D71E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D7248.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D72DC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/A4300/func_800D73BC.s")
