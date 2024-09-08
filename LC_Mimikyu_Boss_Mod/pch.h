// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include "rom.h"
#include "labels.h"
#include "labels_f.h"


extern int math_sin_table;
extern int math_cos_table;
// Battle Network Functions
extern uint32_t(*block_sub_BlockFlash)(GbaState*);
extern uint32_t(*block_sub_BlockInScreenCheck)(GbaState*);
extern uint32_t(*block_sub_BlockInScreenCheckSub)(GbaState*);
extern uint32_t(*block_sub_HitBlockFlash)(GbaState*);
extern uint32_t(*block_sub_HitBlockFlashSub)(GbaState*);
extern uint32_t(*block_sub1_CheckLineHeadBlock)(GbaState*);
extern uint32_t(*bs0batk_Bs0bShiftPosCheck)(GbaState*);
extern uint32_t(*btl_sub_BattleEndCheck)(GbaState*);
extern uint32_t(*btl_sub1_CheckEmActionEnable)(GbaState*);
extern uint32_t(*btl_sub1_CloseEmAction)(GbaState*);
extern uint32_t(*btl_sub1_EntryEmAction)(GbaState*);
extern uint32_t(*btlhit_sub_BattleHitBlockPosSet)(GbaState*);
extern uint32_t(*btlhit_sub_BattleHitCheck)(GbaState*);
extern uint32_t(*btlhit_sub_BattleHitDataSet)(GbaState*);
extern uint32_t(*btlhit_sub_BattleHitOff)(GbaState*);
extern uint32_t(*btlhit_sub_BattleHitOn)(GbaState*);
extern uint32_t(*btlhit_sub_BattleHitSet)(GbaState*);
extern uint32_t(*btlhit_sub1_BattleHitAbnormalStatusSet)(GbaState*);
extern uint32_t(*btlhit_sub1_BattleHitHitMarkCheck)(GbaState*);
extern uint32_t(*btlhit_sub1_BattleHitStatusFlagOff)(GbaState*);
extern uint32_t(*btlhit_sub1_BattleHitStatusFlagOn)(GbaState*);
extern uint32_t(*btlhit0_BattleHitOpen)(GbaState*);
extern uint32_t(*btlobj_sub_BattleObjCharMove)(GbaState*);
extern uint32_t(*btlobj_sub_BattleObjCharMove5)(GbaState*);
extern uint32_t(*efc00_SetEfc00)(GbaState*);
extern uint32_t(*efc28_SetEfc28)(GbaState*);
extern uint32_t(*em_sub_BlockToPos)(GbaState*);
extern uint32_t(*em_sub_CalcPlEmDirSpd)(GbaState*);
extern uint32_t(*em_sub_EnemyFlipCheck)(GbaState*);
extern uint32_t(*em_sub_EnemyPointArriveCheck)(GbaState*);
extern uint32_t(*em_sub_EnemyShiftEnableCheck)(GbaState*);
extern uint32_t(*em_sub_GetBlockPos)(GbaState*);
extern uint32_t(*em_sub_PosToBlock)(GbaState*);
extern uint32_t(*emcomm_EmCommonEntry2_02)(GbaState*);
extern uint32_t(*emcomm_ShlCommonExit)(GbaState*);
extern uint32_t(*fade_FadeSet)(GbaState*);
extern uint32_t(*game_sub_GameBattleEnterSub)(GbaState*);
extern uint32_t(*navi_sub_NaviCounterTimerSet)(GbaState*);
extern uint32_t(*navi_sub2_GetNaviAdrs2)(GbaState*);
extern uint32_t(*naviatk_sub_EmAtkReset)(GbaState*);
extern uint32_t(*naviatk_sub_EmAtkSet)(GbaState*);
extern uint32_t(*naviatk_sub_EmShiftAtkSet)(GbaState*);
extern uint32_t(*obj_mv_ShlClose)(GbaState*);
extern uint32_t(*obj_mv_ShlOpen)(GbaState*);
extern uint32_t(*obj_sub_ObjBldReset)(GbaState*);
extern uint32_t(*obj_sub_ObjDmaSeqSet)(GbaState*);
extern uint32_t(*obj_sub_ObjFlipSet)(GbaState*);
extern uint32_t(*obj_sub_ObjMosaicReset)(GbaState*);
extern uint32_t(*obj_sub_ObjNoShadow)(GbaState*);
extern uint32_t(*obj_sub_ObjRotCalc)(GbaState*);
extern uint32_t(*obj_sub_ObjRotOpen)(GbaState*);
extern uint32_t(*obj_sub_ObjShadowSet)(GbaState*);
extern uint32_t(*objtrans_ObjCharInit)(GbaState*);
extern uint32_t(*objtrans_ObjCharSet)(GbaState*);
extern uint32_t(*rand_Rand2)(GbaState*);
extern uint32_t(*rand_RandInit)(GbaState*);
extern uint32_t(*reserve_ReserveReset)(GbaState*);
extern uint32_t(*reserve_ReserveSet)(GbaState*);
extern uint32_t(*shl_sub_ShlInitDataSet)(GbaState*);
extern uint32_t(*shl03_SetShl03)(GbaState*);
extern uint32_t(*shl96_SetShl96)(GbaState*);
extern uint32_t(*sound_SoundSeReq)(GbaState*);
extern uint32_t(*sub1_BattleReportFlagGet)(GbaState*);
extern uint32_t(*sub1_CalcTrajectory4)(GbaState*);

// Legacy Collection functions
extern void (*lc_post_sound_event)(uint32_t sound_event, int unk);
#endif //PCH_H
