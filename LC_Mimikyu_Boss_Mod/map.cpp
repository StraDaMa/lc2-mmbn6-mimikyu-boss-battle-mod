#include "pch.h"
#include "map.h"

uint32_t lans_room_start_pikachu_battle(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    gba->r0 = labels_lans_room_pikachu_battle;
    gba->r1 = 0x01;
    game_sub_GameBattleEnterSub(gba);
    gba->r0 = 0x2C;
    gba->r1 = 0x10;
    fade_FadeSet(gba);
    gba->r0 = 0;
    // CS_CALL_ASM requires this flag to be set to continue the script
    gba->conditionFlags = 4;
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}