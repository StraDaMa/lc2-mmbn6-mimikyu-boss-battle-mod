#include "pch.h"
#include "mimikyu.h"

enum MimikyuMoveState {
    Start = 0,
    End,
    EndIdle,
};

static void mimikyu_move_start(GbaState* gba);
static void mimikyu_move_end(GbaState* gba);
static void mimikyu_move_end_idle(GbaState* gba);

uint32_t mimikyu_move(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    switch (attackStateBase.state)
    {
    case MimikyuMoveState::Start:
        mimikyu_move_start(gba);
        break;
    case MimikyuMoveState::End:
        mimikyu_move_end(gba);
        break;
    case MimikyuMoveState::EndIdle:
        mimikyu_move_end_idle(gba);
        break;
    }
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}


static void mimikyu_move_start(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);

    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        em_sub_EnemyShiftEnableCheck(gba);
        if (gba->r0 == 0)
        {
            attackStateBase.state = MimikyuMoveState::EndIdle;
            attackStateBase.stateInitialized = false;
            return;
        }
        int moveParameterOffset = objectBase.alliance * 2;
        gba->r2 = mimikyu_move_parameters[moveParameterOffset + 0];
        gba->r3 = mimikyu_move_parameters[moveParameterOffset + 1];
        bs0batk_Bs0bShiftPosCheck(gba);
        if (gba->r0 == 0)
        {
            attackStateBase.state = MimikyuMoveState::EndIdle;
            attackStateBase.stateInitialized = false;
            return;
        }
        objectBase.reservedPanelX = gba->r0;
        objectBase.reservedPanelY = gba->r1;
        reserve_ReserveSet(gba);
        gba->r0 = 0x40;
        btlhit_sub1_BattleHitStatusFlagOn(gba);
        objectBase.animation = MIMIKYU_ANIMATION_MOVE_OUT;
        attackStateBase.timer = 0x02;
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer < 0)
    {
        attackStateBase.state = MimikyuMoveState::End;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_move_end(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        gba->r0 = objectBase.reservedPanelX;
        gba->r1 = objectBase.reservedPanelY;
        objectBase.panelX = gba->r0;
        objectBase.panelY = gba->r1;
        reserve_ReserveReset(gba);
        em_sub_BlockToPos(gba);
        btlhit_sub_BattleHitBlockPosSet(gba);
        gba->r0 = 0x40;
        btlhit_sub1_BattleHitStatusFlagOff(gba);
        gba->r0 = 1 << 0x13;
        btlhit_sub1_BattleHitStatusFlagOn(gba);
        objectBase.animation = MIMIKYU_ANIMATION_MOVE_IN;
        attackStateBase.timer = 0x04;
    }
    attackStateBase.timer = attackStateBase.timer - 1;
    if (attackStateBase.timer < 0)
    {
        attackStateBase.state = MimikyuMoveState::EndIdle;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_move_end_idle(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.animation = MIMIKYU_ANIMATION_IDLE;
        attackStateBase.timer = 0x14;
    }
    attackStateBase.timer = attackStateBase.timer - 1;
    if (attackStateBase.timer < 0)
    {
        naviatk_sub_EmAtkReset(gba);
    }
}
