// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "map.h"
#include "mimikyu.h"
#include "objects.h"
#include "lz4.h"

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

typedef uint32_t(*(*BNFuncPtr))(GbaState*);

static int init_mod_resources(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    uint32_t labelsFreeSpace = 0xF2B00;
    uint32_t romFreeSpace = 0x087FF500;

    math_sin_table = 0xA5A58;
    math_cos_table = 0xA5AD8;

    const uint32_t mimikyuEnemyID = 0x125;
    const uint32_t mimikyuAIID = 0x07;
    const uint16_t mimikyuBattleDrop = 0x4000 | 778;

    //Copy base labels file
    //memcpy(ramBase + labelsFreeSpace, labels_data, labels_size);
    LZ4_decompress_fast((const char*)labels_data, (char*)ramBase + labelsFreeSpace, labels_size_uncompressed);

    labelsFreeSpace += labels_size_uncompressed;
    labelsFreeSpace = (labelsFreeSpace + 3) & ~3;
    // map.asm
    // Set function pointer used by cutscene
    *(uint32_t(**)(GbaState*)) (ramBase + labels_lans_room_start_pikachu_battle_code_ptr) = lans_room_start_pikachu_battle;
    // Repoint Map Init Flag Listener
    *(uint32_t*)(ramBase + 0xABB30) = labels_lans_room_init_flag_listener_hook;
    // Repoint Map Flag Listener
    *(uint32_t*)(ramBase + 0xABB44) = labels_lans_room_flag_listener_hook;
    // Repoint NPC list
    *(uint32_t*)(ramBase + 0xABB58) = labels_lans_room_npc_list;

    // mimikyu_boss.asm
    // Set function pointers used by boss
    *(BNFuncPtr)(ramBase + labels_mimikyu_update_ai_ptr) = mimikyu_update_ai;
    *(BNFuncPtr)(ramBase + labels_mimikyu_move_ptr) = mimikyu_move;
    *(BNFuncPtr)(ramBase + labels_mimikyu_shadowball_ptr) = mimikyu_shadowball;
    *(BNFuncPtr)(ramBase + labels_mimikyu_confuseray_ptr) = mimikyu_confuseray;
    *(BNFuncPtr)(ramBase + labels_mimikyu_shadow_claw_ptr) = mimikyu_shadow_claw;
    *(BNFuncPtr)(ramBase + labels_mimikyu_shadow_sneak_ptr) = mimikyu_shadow_sneak;
    *(BNFuncPtr)(ramBase + labels_mimikyu_playrough_ptr) = mimikyu_playrough;
    *(BNFuncPtr)(ramBase + labels_mimikyu_proc_disguise_ptr) = mimikyu_proc_disguise;
    *(BNFuncPtr)(ramBase + labels_mimikyu_disguise_ptr) = mimikyu_disguise;
    *(BNFuncPtr)(ramBase + labels_mimikyu_on_damage_ptr) = mimikyu_on_damage;
    ////Write Mimikyu over TomahawkMan (Not actually needed)
    //uint8_t* characterParametersPtr = ramBase + 0x521EC + (mimikyuEnemyID * 3);
    //characterParametersPtr[0] = 0x00;
    //characterParametersPtr[1] = 0x01;
    //characterParametersPtr[2] = 0x07;

    //Write Navi Structs
    *(uint32_t*)(ramBase + 0x2EBA4 + (mimikyuAIID * 4)) = labels_mimikyu_ai_struct1;
    *(uint32_t*)(ramBase + 0x2EC08 + (mimikyuAIID * 4)) = labels_mimikyu_ai_struct2;

    // Write Battle Drops
    uint16_t* rewardsPtr = (uint16_t*)(ramBase + 0x5CFB4 + (mimikyuEnemyID * 0x28));
    for (size_t i = 0; i < 20; i++)
    {
        rewardsPtr[i] = mimikyuBattleDrop;
    }
    // Write Support Chip Pool (emchip_tbl_EnemySupportChipTableAdrsGet)
    *(uint32_t*)(ramBase + 0x4F7C4 + (mimikyuAIID * 4)) = labels_mimikyu_chip_pool;
    // subobject spawn (emaccess_EnemyAccessorySetSub)
    *(uint32_t*)(ramBase + 0x4F240 + (mimikyuAIID * 4)) = EXE6G_emaccess__dummy_func;
    // update 1 (boss_boss_move)
    *(uint32_t*)(ramBase + 0x2EADC + (mimikyuAIID * 4)) = EXE6G_hitcheck_BossHitCheck3;
    // update 2 (boss_boss_move)
    *(uint32_t*)(ramBase + 0x2EB40 + (mimikyuAIID * 4)) = labels_mimikyu_proc_disguise_ptr;
    // update 3 (boss_boss_move)
    *(uint32_t*)(ramBase + 0x2EC6C + (mimikyuAIID * 4)) = EXE6G_emaccess__dummy_func;
    // attack pool (boss_boss_move)
    *(uint32_t*)(ramBase + 0x2EA78 + (mimikyuAIID * 4)) = labels_mimikyu_attack_pool;
    //battle start function 1 (em_sub2_EnemyFirstInitProc)
    *(uint32_t*)(ramBase + 0x2ECD0 + (mimikyuAIID * 4)) = EXE6G_emaccess__dummy_func;
    //battle start function 2 (em_sub2_EnemyInitProc)
    *(uint32_t*)(ramBase + 0x2ED34 + (mimikyuAIID * 4)) = EXE6G_emaccess__dummy_func;
    //on delete routine (emaccess_EnemyAccessoryKillSub)
    *(uint32_t*)(ramBase + 0x4F408 + (mimikyuAIID * 4)) = EXE6G_emaccess__dummy_func;
    //on paralyze routine (em_sub2_EnemyPalsyInit)
    *(uint32_t*)(ramBase + 0x2ED98 + (mimikyuAIID * 4)) = labels_mimikyu_on_damage_ptr;
    //on damage routine (em_sub2_EnemyStaggerInit)
    *(uint32_t*)(ramBase + 0x2EEC4 + (mimikyuAIID * 4)) = labels_mimikyu_on_damage_ptr;
    //on push routine (em_sub2_EnemyBlowInit)
    *(uint32_t*)(ramBase + 0x2EF28 + (mimikyuAIID * 4)) = labels_mimikyu_on_damage_ptr;
    //on freeze routine (em_sub2_EnemyFreezeInit)
    *(uint32_t*)(ramBase + 0x2EDFC + (mimikyuAIID * 4)) = labels_mimikyu_on_damage_ptr;
    //on bubble routine (em_sub2_EnemyBubbleInit)
    *(uint32_t*)(ramBase + 0x2EE60 + (mimikyuAIID * 4)) = labels_mimikyu_on_damage_ptr;

    // objects.asm
    // Set function pointers used by objects
    *(BNFuncPtr)(ramBase + labels_mimikyu_multiobject3_main_ptr) = mimikyu_multiobject3_main;
    // Write multiobject main
    *(uint32_t*)(ramBase + 0xA4050 + (MIMIKYU_MULTIOBJECT_ID * 4)) = labels_mimikyu_multiobject3_main_ptr;
    // Replace one sword wave parameter
    const std::array<uint8_t, 16> newSwordWaveParameter = {
        0x07, 0x05, 0x03, 0x04,
        MIMIKYU_SPRITE_CATEGORY, MIMIKYU_SPRITE_INDEX, MIMIKYU_ANIMATION_SHADOWCLAW_SLASH,
        0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00
    };
    memcpy(ramBase + 0xA2160 + (0x12 * 0x10), newSwordWaveParameter.data(), newSwordWaveParameter.size());

    //Copy base rom file
    //memcpy(ramBase + romFreeSpace, rom_data, rom_size);
    LZ4_decompress_fast((const char*)rom_data, (char*)ramBase + romFreeSpace, rom_size_uncompressed);
    
    romFreeSpace += rom_size_uncompressed;
    romFreeSpace = (romFreeSpace + 3) & 3;

    // Repoint Sprites
    *(uint32_t*)(ramBase + 0x76A04) = rom_sprite_mimikyu;
    *(uint32_t*)(ramBase + 0x76E74) = rom_sprite_pikachu_ow;
    // Get this version's functions
    block_sub1_CheckLineHeadBlock          = *(BNFuncPtr)(ramBase + EXE6G_block_sub1_CheckLineHeadBlock);
    block_sub_BlockFlash                   = *(BNFuncPtr)(ramBase + EXE6G_block_sub_BlockFlash);
    block_sub_BlockInScreenCheck           = *(BNFuncPtr)(ramBase + EXE6G_block_sub_BlockInScreenCheck);
    block_sub_BlockInScreenCheckSub        = *(BNFuncPtr)(ramBase + EXE6G_block_sub_BlockInScreenCheckSub);
    block_sub_HitBlockFlash                = *(BNFuncPtr)(ramBase + EXE6G_block_sub_HitBlockFlash);
    block_sub_HitBlockFlashSub             = *(BNFuncPtr)(ramBase + EXE6G_block_sub_HitBlockFlashSub);
    bs0batk_Bs0bShiftPosCheck              = *(BNFuncPtr)(ramBase + EXE6G_bs0batk_Bs0bShiftPosCheck);
    btl_sub_BattleEndCheck                 = *(BNFuncPtr)(ramBase + EXE6G_btl_sub_BattleEndCheck);
    btlhit0_BattleHitOpen                  = *(BNFuncPtr)(ramBase + EXE6G_btlhit0_BattleHitOpen);
    btlhit_sub1_BattleHitAbnormalStatusSet = *(BNFuncPtr)(ramBase + EXE6G_btlhit_sub1_BattleHitAbnormalStatusSet);
    btlhit_sub1_BattleHitHitMarkCheck      = *(BNFuncPtr)(ramBase + EXE6G_btlhit_sub1_BattleHitHitMarkCheck);
    btlhit_sub1_BattleHitStatusFlagOff     = *(BNFuncPtr)(ramBase + EXE6G_btlhit_sub1_BattleHitStatusFlagOff);
    btlhit_sub1_BattleHitStatusFlagOn      = *(BNFuncPtr)(ramBase + EXE6G_btlhit_sub1_BattleHitStatusFlagOn);
    btlhit_sub_BattleHitBlockPosSet        = *(BNFuncPtr)(ramBase + EXE6G_btlhit_sub_BattleHitBlockPosSet);
    btlhit_sub_BattleHitCheck              = *(BNFuncPtr)(ramBase + EXE6G_btlhit_sub_BattleHitCheck);
    btlhit_sub_BattleHitDataSet            = *(BNFuncPtr)(ramBase + EXE6G_btlhit_sub_BattleHitDataSet);
    btlhit_sub_BattleHitOff                = *(BNFuncPtr)(ramBase + EXE6G_btlhit_sub_BattleHitOff);
    btlhit_sub_BattleHitOn                 = *(BNFuncPtr)(ramBase + EXE6G_btlhit_sub_BattleHitOn);
    btlhit_sub_BattleHitSet                = *(BNFuncPtr)(ramBase + EXE6G_btlhit_sub_BattleHitSet);
    btlobj_sub_BattleObjCharMove           = *(BNFuncPtr)(ramBase + EXE6G_btlobj_sub_BattleObjCharMove);
    efc00_SetEfc00                         = *(BNFuncPtr)(ramBase + EXE6G_efc00_SetEfc00);
    efc28_SetEfc28                         = *(BNFuncPtr)(ramBase + EXE6G_efc28_SetEfc28);
    em_sub_BlockToPos                      = *(BNFuncPtr)(ramBase + EXE6G_em_sub_BlockToPos);
    em_sub_CalcPlEmDirSpd                  = *(BNFuncPtr)(ramBase + EXE6G_em_sub_CalcPlEmDirSpd);
    em_sub_EnemyFlipCheck                  = *(BNFuncPtr)(ramBase + EXE6G_em_sub_EnemyFlipCheck);
    em_sub_EnemyPointArriveCheck           = *(BNFuncPtr)(ramBase + EXE6G_em_sub_EnemyPointArriveCheck);
    em_sub_EnemyShiftEnableCheck           = *(BNFuncPtr)(ramBase + EXE6G_em_sub_EnemyShiftEnableCheck);
    em_sub_GetBlockPos                     = *(BNFuncPtr)(ramBase + EXE6G_em_sub_GetBlockPos);
    em_sub_PosToBlock                      = *(BNFuncPtr)(ramBase + EXE6G_em_sub_PosToBlock);
    emcomm_ShlCommonExit                   = *(BNFuncPtr)(ramBase + EXE6G_emcomm_ShlCommonExit);
    fade_FadeSet                           = *(BNFuncPtr)(ramBase + EXE6G_fade_FadeSet);
    game_sub_GameBattleEnterSub            = *(BNFuncPtr)(ramBase + EXE6G_game_sub_GameBattleEnterSub);
    navi_sub2_GetNaviAdrs2                 = *(BNFuncPtr)(ramBase + EXE6G_navi_sub2_GetNaviAdrs2);
    navi_sub_NaviCounterTimerSet           = *(BNFuncPtr)(ramBase + EXE6G_navi_sub_NaviCounterTimerSet);
    naviatk_sub_EmAtkReset                 = *(BNFuncPtr)(ramBase + EXE6G_naviatk_sub_EmAtkReset);
    naviatk_sub_EmAtkSet                   = *(BNFuncPtr)(ramBase + EXE6G_naviatk_sub_EmAtkSet);
    naviatk_sub_EmShiftAtkSet              = *(BNFuncPtr)(ramBase + EXE6G_naviatk_sub_EmShiftAtkSet);
    obj_mv_ShlClose                        = *(BNFuncPtr)(ramBase + EXE6G_obj_mv_ShlClose);
    obj_mv_ShlOpen                         = *(BNFuncPtr)(ramBase + EXE6G_obj_mv_ShlOpen);
    obj_sub_ObjDmaSeqSet                   = *(BNFuncPtr)(ramBase + EXE6G_obj_sub_ObjDmaSeqSet);
    obj_sub_ObjFlipSet                     = *(BNFuncPtr)(ramBase + EXE6G_obj_sub_ObjFlipSet);
    obj_sub_ObjNoShadow                    = *(BNFuncPtr)(ramBase + EXE6G_obj_sub_ObjNoShadow);
    obj_sub_ObjRotCalc                     = *(BNFuncPtr)(ramBase + EXE6G_obj_sub_ObjRotCalc);
    obj_sub_ObjRotOpen                     = *(BNFuncPtr)(ramBase + EXE6G_obj_sub_ObjRotOpen);
    objtrans_ObjCharInit                   = *(BNFuncPtr)(ramBase + EXE6G_objtrans_ObjCharInit);
    objtrans_ObjCharSet                    = *(BNFuncPtr)(ramBase + EXE6G_objtrans_ObjCharSet);
    rand_Rand2                             = *(BNFuncPtr)(ramBase + EXE6G_rand_Rand2);
    rand_RandInit                          = *(BNFuncPtr)(ramBase + EXE6G_rand_RandInit);
    reserve_ReserveReset                   = *(BNFuncPtr)(ramBase + EXE6G_reserve_ReserveReset);
    reserve_ReserveSet                     = *(BNFuncPtr)(ramBase + EXE6G_reserve_ReserveSet);
    shl03_SetShl03                         = *(BNFuncPtr)(ramBase + EXE6G_shl03_SetShl03);
    shl96_SetShl96                         = *(BNFuncPtr)(ramBase + EXE6G_shl96_SetShl96);
    shl_sub_ShlInitDataSet                 = *(BNFuncPtr)(ramBase + EXE6G_shl_sub_ShlInitDataSet);
    sound_SoundSeReq                       = *(BNFuncPtr)(ramBase + EXE6G_sound_SoundSeReq);
    sub1_CalcTrajectory4                   = *(BNFuncPtr)(ramBase + EXE6G_sub1_CalcTrajectory4);
    return rand_RandInit(gba);
}

static int init_mod_resources_f(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    uint32_t labelsFreeSpace = 0xF2B00;
    uint32_t romFreeSpace = 0x087FF500;

    math_sin_table = 0xA4FF0;
    math_cos_table = 0xA5070;

    const uint32_t mimikyuEnemyID = 0x107;
    const uint32_t mimikyuAIID = 0x02;
    const uint16_t mimikyuBattleDrop = 0x4000 | 778;

    //Copy base labels file
    //memcpy(ramBase + labelsFreeSpace, labels_f_data, labels_f_size);
    LZ4_decompress_fast((const char*)labels_f_data, (char*)ramBase + labelsFreeSpace, labels_f_size_uncompressed);

    labelsFreeSpace += labels_f_size_uncompressed;
    labelsFreeSpace = (labelsFreeSpace + 3) & ~3;
    // map.asm
    // Set function pointer used by cutscene
    *(uint32_t(**)(GbaState*)) (ramBase + labels_f_lans_room_start_pikachu_battle_code_ptr) = lans_room_start_pikachu_battle;
    // Repoint Map Init Flag Listener
    *(uint32_t*)(ramBase + 0xAB0D0) = labels_f_lans_room_init_flag_listener_hook;
    // Repoint Map Flag Listener
    *(uint32_t*)(ramBase + 0xAB0E4) = labels_f_lans_room_flag_listener_hook;
    // Repoint NPC list
    *(uint32_t*)(ramBase + 0xAB0F8) = labels_f_lans_room_npc_list;

    // mimikyu_boss.asm
    // Set function pointers used by boss
    *(BNFuncPtr)(ramBase + labels_f_mimikyu_update_ai_ptr) = mimikyu_update_ai;
    *(BNFuncPtr)(ramBase + labels_f_mimikyu_move_ptr) = mimikyu_move;
    *(BNFuncPtr)(ramBase + labels_f_mimikyu_shadowball_ptr) = mimikyu_shadowball;
    *(BNFuncPtr)(ramBase + labels_f_mimikyu_confuseray_ptr) = mimikyu_confuseray;
    *(BNFuncPtr)(ramBase + labels_f_mimikyu_shadow_claw_ptr) = mimikyu_shadow_claw;
    *(BNFuncPtr)(ramBase + labels_f_mimikyu_shadow_sneak_ptr) = mimikyu_shadow_sneak;
    *(BNFuncPtr)(ramBase + labels_f_mimikyu_playrough_ptr) = mimikyu_playrough;
    *(BNFuncPtr)(ramBase + labels_f_mimikyu_proc_disguise_ptr) = mimikyu_proc_disguise;
    *(BNFuncPtr)(ramBase + labels_f_mimikyu_disguise_ptr) = mimikyu_disguise;
    *(BNFuncPtr)(ramBase + labels_f_mimikyu_on_damage_ptr) = mimikyu_on_damage;
    ////Write Mimikyu over ElecMan (Not actually needed)
    //uint8_t* characterParametersPtr = ramBase + 0x521EC + (mimikyuEnemyID * 3);
    //characterParametersPtr[0] = 0x00;
    //characterParametersPtr[1] = 0x01;
    //characterParametersPtr[2] = 0x07;

    //Write Navi Structs
    *(uint32_t*)(ramBase + 0x2ECC4 + (mimikyuAIID * 4)) = labels_f_mimikyu_ai_struct1;
    *(uint32_t*)(ramBase + 0x2ED28 + (mimikyuAIID * 4)) = labels_f_mimikyu_ai_struct2;

    // Write Battle Drops
    uint16_t* rewardsPtr = (uint16_t*)(ramBase + 0x5CBA4 + (mimikyuEnemyID * 0x28));
    for (size_t i = 0; i < 20; i++)
    {
        rewardsPtr[i] = mimikyuBattleDrop;
    }
    // Write Support Chip Pool (emchip_tbl_EnemySupportChipTableAdrsGet)
    *(uint32_t*)(ramBase + 0x4EE84 + (mimikyuAIID * 4)) = labels_f_mimikyu_chip_pool;
    // subobject spawn (emaccess_EnemyAccessorySetSub)
    *(uint32_t*)(ramBase + 0x4E900 + (mimikyuAIID * 4)) = EXE6F_emaccess__dummy_func;
    // update 1 (boss_boss_move)
    // hitcheck_BossHitCheck gives tomahawkid status guard, hitcheck_BossHitCheck3 works
    *(uint32_t*)(ramBase + 0x2EBFC + (mimikyuAIID * 4)) = EXE6F_hitcheck_BossHitCheck3;
    // update 2 (boss_boss_move)
    *(uint32_t*)(ramBase + 0x2EC60 + (mimikyuAIID * 4)) = labels_f_mimikyu_proc_disguise_ptr;
    // update 3 (boss_boss_move)
    *(uint32_t*)(ramBase + 0x2ED8C + (mimikyuAIID * 4)) = EXE6F_emaccess__dummy_func;
    // attack pool (boss_boss_move)
    *(uint32_t*)(ramBase + 0x2EB98 + (mimikyuAIID * 4)) = labels_f_mimikyu_attack_pool;
    //battle start function 1 (em_sub2_EnemyFirstInitProc)
    *(uint32_t*)(ramBase + 0x2EDF0 + (mimikyuAIID * 4)) = EXE6F_emaccess__dummy_func;
    //battle start function 2 (em_sub2_EnemyInitProc)
    *(uint32_t*)(ramBase + 0x2EE54 + (mimikyuAIID * 4)) = EXE6F_emaccess__dummy_func;
    //on delete routine (emaccess_EnemyAccessoryKillSub)
    *(uint32_t*)(ramBase + 0x4EAC8 + (mimikyuAIID * 4)) = EXE6F_emaccess__dummy_func;
    //on paralyze routine (em_sub2_EnemyPalsyInit)
    *(uint32_t*)(ramBase + 0x2EEB8 + (mimikyuAIID * 4)) = labels_f_mimikyu_on_damage_ptr;
    //on damage routine (em_sub2_EnemyStaggerInit)
    *(uint32_t*)(ramBase + 0x2EFE4 + (mimikyuAIID * 4)) = labels_f_mimikyu_on_damage_ptr;
    //on push routine (em_sub2_EnemyBlowInit)
    *(uint32_t*)(ramBase + 0x2F048 + (mimikyuAIID * 4)) = labels_f_mimikyu_on_damage_ptr;
    //on freeze routine (em_sub2_EnemyFreezeInit)
    *(uint32_t*)(ramBase + 0x2EF1C + (mimikyuAIID * 4)) = labels_f_mimikyu_on_damage_ptr;
    //on bubble routine (em_sub2_EnemyBubbleInit)
    *(uint32_t*)(ramBase + 0x2EF80 + (mimikyuAIID * 4)) = labels_f_mimikyu_on_damage_ptr;

    // objects.asm
    // Set function pointers used by objects
    *(BNFuncPtr)(ramBase + labels_f_mimikyu_multiobject3_main_ptr) = mimikyu_multiobject3_main;
    // Write multiobject main
    *(uint32_t*)(ramBase + 0xA35E8 + (MIMIKYU_MULTIOBJECT_ID * 4)) = labels_f_mimikyu_multiobject3_main_ptr;
    // Replace one sword wave parameter
    const std::array<uint8_t, 16> newSwordWaveParameter = {
        0x07, 0x05, 0x03, 0x04,
        MIMIKYU_SPRITE_CATEGORY, MIMIKYU_SPRITE_INDEX, MIMIKYU_ANIMATION_SHADOWCLAW_SLASH,
        0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00
    };
    memcpy(ramBase + 0xA1540 + (0x12 * 0x10), newSwordWaveParameter.data(), newSwordWaveParameter.size());

    //Copy base rom file
    //memcpy(ramBase + romFreeSpace, rom_data, rom_size);
    LZ4_decompress_fast((const char*)rom_data, (char*)ramBase + romFreeSpace, rom_size_uncompressed);

    romFreeSpace += rom_size_uncompressed;
    romFreeSpace = (romFreeSpace + 3) & 3;

    // Repoint Sprites
    *(uint32_t*)(ramBase + 0x7672C) = rom_sprite_mimikyu;
    *(uint32_t*)(ramBase + 0x76B9C) = rom_sprite_pikachu_ow;
    // Get this version's functions
    block_sub1_CheckLineHeadBlock          = *(BNFuncPtr)(ramBase + EXE6F_block_sub1_CheckLineHeadBlock);
    block_sub_BlockFlash                   = *(BNFuncPtr)(ramBase + EXE6F_block_sub_BlockFlash);
    block_sub_BlockInScreenCheck           = *(BNFuncPtr)(ramBase + EXE6F_block_sub_BlockInScreenCheck);
    block_sub_BlockInScreenCheckSub        = *(BNFuncPtr)(ramBase + EXE6F_block_sub_BlockInScreenCheckSub);
    block_sub_HitBlockFlash                = *(BNFuncPtr)(ramBase + EXE6F_block_sub_HitBlockFlash);
    block_sub_HitBlockFlashSub             = *(BNFuncPtr)(ramBase + EXE6F_block_sub_HitBlockFlashSub);
    bs0batk_Bs0bShiftPosCheck              = *(BNFuncPtr)(ramBase + EXE6F_bs0batk_Bs0bShiftPosCheck);
    btl_sub_BattleEndCheck                 = *(BNFuncPtr)(ramBase + EXE6F_btl_sub_BattleEndCheck);
    btlhit0_BattleHitOpen                  = *(BNFuncPtr)(ramBase + EXE6F_btlhit0_BattleHitOpen);
    btlhit_sub1_BattleHitAbnormalStatusSet = *(BNFuncPtr)(ramBase + EXE6F_btlhit_sub1_BattleHitAbnormalStatusSet);
    btlhit_sub1_BattleHitHitMarkCheck      = *(BNFuncPtr)(ramBase + EXE6F_btlhit_sub1_BattleHitHitMarkCheck);
    btlhit_sub1_BattleHitStatusFlagOff     = *(BNFuncPtr)(ramBase + EXE6F_btlhit_sub1_BattleHitStatusFlagOff);
    btlhit_sub1_BattleHitStatusFlagOn      = *(BNFuncPtr)(ramBase + EXE6F_btlhit_sub1_BattleHitStatusFlagOn);
    btlhit_sub_BattleHitBlockPosSet        = *(BNFuncPtr)(ramBase + EXE6F_btlhit_sub_BattleHitBlockPosSet);
    btlhit_sub_BattleHitCheck              = *(BNFuncPtr)(ramBase + EXE6F_btlhit_sub_BattleHitCheck);
    btlhit_sub_BattleHitDataSet            = *(BNFuncPtr)(ramBase + EXE6F_btlhit_sub_BattleHitDataSet);
    btlhit_sub_BattleHitOff                = *(BNFuncPtr)(ramBase + EXE6F_btlhit_sub_BattleHitOff);
    btlhit_sub_BattleHitOn                 = *(BNFuncPtr)(ramBase + EXE6F_btlhit_sub_BattleHitOn);
    btlhit_sub_BattleHitSet                = *(BNFuncPtr)(ramBase + EXE6F_btlhit_sub_BattleHitSet);
    btlobj_sub_BattleObjCharMove           = *(BNFuncPtr)(ramBase + EXE6F_btlobj_sub_BattleObjCharMove);
    efc00_SetEfc00                         = *(BNFuncPtr)(ramBase + EXE6F_efc00_SetEfc00);
    efc28_SetEfc28                         = *(BNFuncPtr)(ramBase + EXE6F_efc28_SetEfc28);
    em_sub_BlockToPos                      = *(BNFuncPtr)(ramBase + EXE6F_em_sub_BlockToPos);
    em_sub_CalcPlEmDirSpd                  = *(BNFuncPtr)(ramBase + EXE6F_em_sub_CalcPlEmDirSpd);
    em_sub_EnemyFlipCheck                  = *(BNFuncPtr)(ramBase + EXE6F_em_sub_EnemyFlipCheck);
    em_sub_EnemyPointArriveCheck           = *(BNFuncPtr)(ramBase + EXE6F_em_sub_EnemyPointArriveCheck);
    em_sub_EnemyShiftEnableCheck           = *(BNFuncPtr)(ramBase + EXE6F_em_sub_EnemyShiftEnableCheck);
    em_sub_GetBlockPos                     = *(BNFuncPtr)(ramBase + EXE6F_em_sub_GetBlockPos);
    em_sub_PosToBlock                      = *(BNFuncPtr)(ramBase + EXE6F_em_sub_PosToBlock);
    emcomm_ShlCommonExit                   = *(BNFuncPtr)(ramBase + EXE6F_emcomm_ShlCommonExit);
    fade_FadeSet                           = *(BNFuncPtr)(ramBase + EXE6F_fade_FadeSet);
    game_sub_GameBattleEnterSub            = *(BNFuncPtr)(ramBase + EXE6F_game_sub_GameBattleEnterSub);
    navi_sub2_GetNaviAdrs2                 = *(BNFuncPtr)(ramBase + EXE6F_navi_sub2_GetNaviAdrs2);
    navi_sub_NaviCounterTimerSet           = *(BNFuncPtr)(ramBase + EXE6F_navi_sub_NaviCounterTimerSet);
    naviatk_sub_EmAtkReset                 = *(BNFuncPtr)(ramBase + EXE6F_naviatk_sub_EmAtkReset);
    naviatk_sub_EmAtkSet                   = *(BNFuncPtr)(ramBase + EXE6F_naviatk_sub_EmAtkSet);
    naviatk_sub_EmShiftAtkSet              = *(BNFuncPtr)(ramBase + EXE6F_naviatk_sub_EmShiftAtkSet);
    obj_mv_ShlClose                        = *(BNFuncPtr)(ramBase + EXE6F_obj_mv_ShlClose);
    obj_mv_ShlOpen                         = *(BNFuncPtr)(ramBase + EXE6F_obj_mv_ShlOpen);
    obj_sub_ObjDmaSeqSet                   = *(BNFuncPtr)(ramBase + EXE6F_obj_sub_ObjDmaSeqSet);
    obj_sub_ObjFlipSet                     = *(BNFuncPtr)(ramBase + EXE6F_obj_sub_ObjFlipSet);
    obj_sub_ObjNoShadow                    = *(BNFuncPtr)(ramBase + EXE6F_obj_sub_ObjNoShadow);
    obj_sub_ObjRotCalc                     = *(BNFuncPtr)(ramBase + EXE6F_obj_sub_ObjRotCalc);
    obj_sub_ObjRotOpen                     = *(BNFuncPtr)(ramBase + EXE6F_obj_sub_ObjRotOpen);
    objtrans_ObjCharInit                   = *(BNFuncPtr)(ramBase + EXE6F_objtrans_ObjCharInit);
    objtrans_ObjCharSet                    = *(BNFuncPtr)(ramBase + EXE6F_objtrans_ObjCharSet);
    rand_Rand2                             = *(BNFuncPtr)(ramBase + EXE6F_rand_Rand2);
    rand_RandInit                          = *(BNFuncPtr)(ramBase + EXE6F_rand_RandInit);
    reserve_ReserveReset                   = *(BNFuncPtr)(ramBase + EXE6F_reserve_ReserveReset);
    reserve_ReserveSet                     = *(BNFuncPtr)(ramBase + EXE6F_reserve_ReserveSet);
    shl03_SetShl03                         = *(BNFuncPtr)(ramBase + EXE6F_shl03_SetShl03);
    shl96_SetShl96                         = *(BNFuncPtr)(ramBase + EXE6F_shl96_SetShl96);
    shl_sub_ShlInitDataSet                 = *(BNFuncPtr)(ramBase + EXE6F_shl_sub_ShlInitDataSet);
    sound_SoundSeReq                       = *(BNFuncPtr)(ramBase + EXE6F_sound_SoundSeReq);
    sub1_CalcTrajectory4                   = *(BNFuncPtr)(ramBase + EXE6F_sub1_CalcTrajectory4);
    return rand_RandInit(gba);
}

uint8_t* gregarHookPtr = nullptr;
uint8_t* falzarHookPtr = nullptr;

EXTERN_DLL_EXPORT int luaopen_LC_Mimikyu_Boss_Mod(void* unused) {
    std::array<uint8_t, 16> hookBase{
        0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  //mov rax, functionPointer
        0xFF, 0xD0,                                                  //call rax
        0x90, 0x90, 0x90, 0x90,                                      //nop nop nop nop
    };
    DWORD oldPerms;
    *(uint64_t*)&hookBase[2] = (uint64_t)&init_mod_resources;
    uint8_t* gregarHook = (uint8_t*)gregarHookPtr;
    VirtualProtect(gregarHook, 16, PAGE_EXECUTE_READWRITE, &oldPerms);
    memcpy(gregarHook, hookBase.data(), 16);
    VirtualProtect(gregarHook, 16, oldPerms, &oldPerms);

    *(uint64_t*)&hookBase[2] = (uint64_t)&init_mod_resources_f;
    uint8_t* falzarHook = (uint8_t*)falzarHookPtr;
    VirtualProtect(falzarHook, 16, PAGE_EXECUTE_READWRITE, &oldPerms);
    memcpy(falzarHook, hookBase.data(), 16);
    VirtualProtect(falzarHook, 16, oldPerms, &oldPerms);
    return 0;
}

static uint8_t* find_with_mask(uint8_t* memory, uint8_t* memory_end,
    const uint8_t* search, const uint32_t search_length,
    const uint8_t ignore_value) {
    // Loop through each position in memory
    while (memory <= memory_end - search_length)
    {
        bool match = true;
        // Loop through each uint8_t in the search bytes
        for (uint32_t j = 0; j < search_length; j++) {
            // Check if the value is ignored or matches
            if ((search[j] != ignore_value) && (memory[j] != search[j])) {
                match = false;
                break;
            }
        }
        if (match)
            return memory;
        memory++;
    }
    // Return nullptr if no match is found
    return nullptr;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        // Check for hook offsets here so chaud loader can print an error if not found
        HMODULE exeBase = GetModuleHandleA("MMBN_LC2.exe");
        if (exeBase == NULL)
        {
            return FALSE;
        }
        MODULEINFO mi = {};
        HANDLE process = GetCurrentProcess();
        GetModuleInformation(process, exeBase, &mi, sizeof(mi));

        uint8_t* exeBasePtr = (uint8_t*)exeBase;
        //E8 ?? ?? ?? ?? 3D A3 1C 00 00 0F 85 CA 05 00 00
        const std::array<uint8_t, 16> searchBytes = {
            0xE8, 0xFF, 0xFF, 0xFF, 0xFF,            //CALL rand_RandInit
            0x3D, 0xA3, 0x1C, 0x00, 0x00,            //CMP EAX,0x1CA3
            0x0F, 0x85, 0xCA, 0x05, 0x00, 0x00,      //JNZ
        };
        // Just search everything
        uint8_t* exePtr = exeBasePtr + 0x00;
        uint8_t* exeEndPtr = exeBasePtr + mi.SizeOfImage;
        // First result for these bytes is gregar
        gregarHookPtr = find_with_mask(exePtr, exeEndPtr,
            searchBytes.data(), searchBytes.size(), 0xFF);
        if (gregarHookPtr == nullptr)
            return false;
        // Second result for these bytes is falzar
        falzarHookPtr = find_with_mask(gregarHookPtr + searchBytes.size(), exeEndPtr,
            searchBytes.data(), searchBytes.size(), 0xFF);
        if (falzarHookPtr == nullptr)
            return false;
        // There should be no more results for this
        uint8_t* testHookPtr = find_with_mask(falzarHookPtr + searchBytes.size(), exeEndPtr,
            searchBytes.data(), searchBytes.size(), 0xFF);
        if (testHookPtr != nullptr)
            return false;
    }
    break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}