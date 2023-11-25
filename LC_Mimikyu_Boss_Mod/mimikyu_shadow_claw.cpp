#include "pch.h"
#include "mimikyu.h"

enum MimikyuShadowClawState {
    BeginMove = 0,
    Move,
    Attack,
};

static void mimikyu_shadow_claw_begin_move(GbaState* gba);
static void mimikyu_shadow_claw_move(GbaState* gba);
static void mimikyu_shadow_claw_attack(GbaState* gba);

uint32_t mimikyu_shadow_claw(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);

    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    switch (attackStateBase.state)
    {
    case MimikyuShadowClawState::BeginMove:
        mimikyu_shadow_claw_begin_move(gba);
        break;
    case MimikyuShadowClawState::Move:
        mimikyu_shadow_claw_move(gba);
        break;
    case MimikyuShadowClawState::Attack:
        mimikyu_shadow_claw_attack(gba);
        break;
    }
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}

static void mimikyu_shadow_claw_begin_move(GbaState* gba)
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
            // Refund some points for this attack if it fails
            aiStateBase.mediumAttackPoints += 7;
            naviatk_sub_EmAtkReset(gba);
            return;
        }
        BattleObject& enemyObjectBase = *(BattleObject*)(ramBase + attackStateBase.enemyOffset);

        int moveParameterOffset = objectBase.alliance * 2;
        gba->r2 = mimikyu_move_parameters[moveParameterOffset + 0];
        gba->r3 = mimikyu_move_parameters[moveParameterOffset + 1];
        gba->r0 = objectBase.alliance;
        gba->r1 = enemyObjectBase.panelY;
        block_sub1_CheckLineHeadBlock(gba);
        if (gba->r0 == 0)
        {
            // Refund some points for this attack if it fails
            aiStateBase.mediumAttackPoints += 7;
            naviatk_sub_EmAtkReset(gba);
            return;
        }
        objectBase.reservedPanelX = gba->r0;
        objectBase.reservedPanelY = gba->r1;
        reserve_ReserveSet(gba);
        gba->r0 = 0x40;
        btlhit_sub1_BattleHitStatusFlagOn(gba);
        objectBase.animation = MIMIKYU_ANIMATION_MOVE_OUT;
        attackStateBase.timer = 3;
        return;
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer <= 0)
    {
        objectBase.animation = MIMIKYU_ANIMATION_MOVE_IN;
        attackStateBase.timer = 3;
        gba->r0 = objectBase.reservedPanelX;
        gba->r1 = objectBase.reservedPanelY;
        objectBase.panelX = gba->r0;
        objectBase.panelY = gba->r1;
        reserve_ReserveReset(gba);
        em_sub_BlockToPos(gba);
        btlhit_sub_BattleHitBlockPosSet(gba);
        attackStateBase.state = MimikyuShadowClawState::Move;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_shadow_claw_move(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);

    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    attackStateBase.timer -= 1;
    if (attackStateBase.timer <= 0)
    {
        gba->r0 = 0x40;
        btlhit_sub1_BattleHitStatusFlagOff(gba);
        gba->r0 = 0x01 << 0x13;
        btlhit_sub1_BattleHitStatusFlagOn(gba);
        attackStateBase.state = MimikyuShadowClawState::Attack;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_shadow_claw_attack(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);

    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.animation = MIMIKYU_ANIMATION_PHYSICAL_ATTACK;
        attackStateBase.timer = 40;
        attackStateBase.timer2 = 0;
        navi_sub_NaviCounterTimerSet(gba);
        return;
    }
    attackStateBase.timer2 += 1;
    if ((attackStateBase.timer2 < 40 - 24) && ((attackStateBase.timer2 & 8) != 0))
    {
        em_sub_CalcPlEmDirSpd(gba);
        gba->r0 = gba->r0 + objectBase.panelX;
        gba->r1 = objectBase.panelY;
        gba->r2 = 0x11;
        gba->r3 = objectBase.alliance | (objectBase.flip << 0x08);
        block_sub_HitBlockFlashSub(gba);
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer == 40 - 24)
    {
        gba->r0 = 0xB0;
        sound_SoundSeReq(gba);
        em_sub_CalcPlEmDirSpd(gba);
        gba->r0 = gba->r0 + objectBase.panelX;
        gba->r1 = objectBase.panelY;
        gba->r2 = AttackElement_Sword;
        gba->r3 = 0x00;
        gba->r4 = 0x12;
        gba->r6 = (10 << 0x10) | MIMIKYU_SHADOWCLAW_DAMAGE;
        shl96_SetShl96(gba);
    }
    else if (attackStateBase.timer <= 0)
    {
        naviatk_sub_EmAtkReset(gba);
    }
}
