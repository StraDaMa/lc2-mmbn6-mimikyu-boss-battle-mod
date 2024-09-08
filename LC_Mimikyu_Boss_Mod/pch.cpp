// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
int math_sin_table;
int math_cos_table;
uint32_t(*block_sub_BlockFlash)(GbaState*);
uint32_t(*block_sub_BlockInScreenCheck)(GbaState*);
uint32_t(*block_sub_BlockInScreenCheckSub)(GbaState*);
uint32_t(*block_sub_HitBlockFlash)(GbaState*);
uint32_t(*block_sub_HitBlockFlashSub)(GbaState*);
uint32_t(*block_sub1_CheckLineHeadBlock)(GbaState*);
uint32_t(*bs0batk_Bs0bShiftPosCheck)(GbaState*);
uint32_t(*btl_sub_BattleEndCheck)(GbaState*);
uint32_t(*btl_sub1_CheckEmActionEnable)(GbaState*);
uint32_t(*btl_sub1_CloseEmAction)(GbaState*);
uint32_t(*btl_sub1_EntryEmAction)(GbaState*);
uint32_t(*btlhit_sub_BattleHitBlockPosSet)(GbaState*);
uint32_t(*btlhit_sub_BattleHitCheck)(GbaState*);
uint32_t(*btlhit_sub_BattleHitDataSet)(GbaState*);
uint32_t(*btlhit_sub_BattleHitOff)(GbaState*);
uint32_t(*btlhit_sub_BattleHitOn)(GbaState*);
uint32_t(*btlhit_sub_BattleHitSet)(GbaState*);
uint32_t(*btlhit_sub1_BattleHitAbnormalStatusSet)(GbaState*);
uint32_t(*btlhit_sub1_BattleHitHitMarkCheck)(GbaState*);
uint32_t(*btlhit_sub1_BattleHitStatusFlagOff)(GbaState*);
uint32_t(*btlhit_sub1_BattleHitStatusFlagOn)(GbaState*);
uint32_t(*btlhit0_BattleHitOpen)(GbaState*);
uint32_t(*btlobj_sub_BattleObjCharMove)(GbaState*);
uint32_t(*btlobj_sub_BattleObjCharMove5)(GbaState*);
uint32_t(*efc00_SetEfc00)(GbaState*);
uint32_t(*efc28_SetEfc28)(GbaState*);
uint32_t(*em_sub_BlockToPos)(GbaState*);
uint32_t(*em_sub_CalcPlEmDirSpd)(GbaState*);
uint32_t(*em_sub_EnemyFlipCheck)(GbaState*);
uint32_t(*em_sub_EnemyPointArriveCheck)(GbaState*);
uint32_t(*em_sub_EnemyShiftEnableCheck)(GbaState*);
uint32_t(*em_sub_GetBlockPos)(GbaState*);
uint32_t(*em_sub_PosToBlock)(GbaState*);
uint32_t(*emcomm_EmCommonEntry2_02)(GbaState*);
uint32_t(*emcomm_ShlCommonExit)(GbaState*);
uint32_t(*fade_FadeSet)(GbaState*);
uint32_t(*game_sub_GameBattleEnterSub)(GbaState*);
uint32_t(*navi_sub_NaviCounterTimerSet)(GbaState*);
uint32_t(*navi_sub2_GetNaviAdrs2)(GbaState*);
uint32_t(*naviatk_sub_EmAtkReset)(GbaState*);
uint32_t(*naviatk_sub_EmAtkSet)(GbaState*);
uint32_t(*naviatk_sub_EmShiftAtkSet)(GbaState*);
uint32_t(*obj_mv_ShlClose)(GbaState*);
uint32_t(*obj_mv_ShlOpen)(GbaState*);
uint32_t(*obj_sub_ObjDmaSeqSet)(GbaState*);
uint32_t(*obj_sub_ObjFlipSet)(GbaState*);
uint32_t(*obj_sub_ObjNoShadow)(GbaState*);
uint32_t(*obj_sub_ObjRotCalc)(GbaState*);
uint32_t(*obj_sub_ObjRotOpen)(GbaState*);
uint32_t(*obj_sub_ObjShadowSet)(GbaState*);
uint32_t(*objtrans_ObjCharInit)(GbaState*);
uint32_t(*objtrans_ObjCharSet)(GbaState*);
uint32_t(*rand_Rand2)(GbaState*);
uint32_t(*rand_RandInit)(GbaState*);
uint32_t(*reserve_ReserveReset)(GbaState*);
uint32_t(*reserve_ReserveSet)(GbaState*);
uint32_t(*shl_sub_ShlInitDataSet)(GbaState*);
uint32_t(*shl03_SetShl03)(GbaState*);
uint32_t(*shl96_SetShl96)(GbaState*);
uint32_t(*sound_SoundSeReq)(GbaState*);
uint32_t(*sub1_BattleReportFlagGet)(GbaState*);
uint32_t(*sub1_CalcTrajectory4)(GbaState*);

// Legacy Collection functions
void (*lc_post_sound_event)(uint32_t sound_event, int unk);