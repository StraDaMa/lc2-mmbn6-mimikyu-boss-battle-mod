#include "pch.h"
#include "mimikyu.h"

enum MimikyuShadowSneakState {
    Disappear = 0,
    Move,
    Reappear,
    Attack,
    Return,
};

static void mimikyu_shadow_sneak_disappear(GbaState* gba);
static void mimikyu_shadow_sneak_move(GbaState* gba);
static void mimikyu_shadow_sneak_reappear(GbaState* gba);
static void mimikyu_shadow_sneak_attack(GbaState* gba);
static void mimikyu_shadow_sneak_return(GbaState* gba);

uint32_t mimikyu_shadow_sneak(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    switch (attackStateBase.state)
    {
    case MimikyuShadowSneakState::Disappear:
        mimikyu_shadow_sneak_disappear(gba);
        break;
    case MimikyuShadowSneakState::Move:
        mimikyu_shadow_sneak_move(gba);
        break;
    case MimikyuShadowSneakState::Reappear:
        mimikyu_shadow_sneak_reappear(gba);
        break;
    case MimikyuShadowSneakState::Attack:
        mimikyu_shadow_sneak_attack(gba);
        break;
    case MimikyuShadowSneakState::Return:
        mimikyu_shadow_sneak_return(gba);
        break;
    }
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}

static void mimikyu_shadow_sneak_disappear(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        em_sub_CalcPlEmDirSpd(gba);
        BattleObject& enemyObjectBase = *(BattleObject*)(ramBase + attackStateBase.enemyOffset);
        int targetPanelX = enemyObjectBase.panelX + gba->r0;
        int targetPanelY = enemyObjectBase.panelY;
        gba->r0 = targetPanelX;
        gba->r1 = targetPanelY;
        block_sub_BlockInScreenCheckSub(gba);
        if (gba->r0 == 0)
        {
            // Refund some points for this attack if it fails
            aiStateBase.mediumAttackPoints += 7;
            naviatk_sub_EmAtkReset(gba);
            return;
        }
        attackStateBase.movePanelX = targetPanelX;
        attackStateBase.movePanelY = targetPanelY;
        gba->r0 = targetPanelX;
        gba->r1 = targetPanelY;
        reserve_ReserveSet(gba);
        objectBase.reservedPanelX = objectBase.panelX;
        objectBase.reservedPanelY = objectBase.panelY;
        gba->r0 = objectBase.panelX;
        gba->r1 = objectBase.panelY;
        reserve_ReserveSet(gba);
        gba->r0 = 0x40;
        btlhit_sub1_BattleHitStatusFlagOn(gba);
        objectBase.animation = MIMIKYU_ANIMATION_SHADOW_SNEAK_BEGIN;
        attackStateBase.timer = 20;
        btlhit_sub_BattleHitOff(gba);
        gba->r0 = 0x157;
        sound_SoundSeReq(gba);
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer <= 0)
    {
        attackStateBase.state = MimikyuShadowSneakState::Move;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_shadow_sneak_move(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        gba->r0 = attackStateBase.movePanelX;
        gba->r1 = attackStateBase.movePanelY;
        em_sub_GetBlockPos(gba);
        int targetPanelCoordX = gba->r0;
        int targetPanelCoordY = gba->r1;
        int currentPanelCoordX = objectBase.positionX;
        int currentPanelCoordY = objectBase.positionY;
        objectBase.speedX = (targetPanelCoordX - currentPanelCoordX) / 8;
        objectBase.speedY = (targetPanelCoordY - currentPanelCoordY) / 8;
        attackStateBase.timer = 8;
    }
    objectBase.positionX += objectBase.speedX;
    objectBase.positionY += objectBase.speedY;
    attackStateBase.timer -= 1;
    if (attackStateBase.timer <= 0)
    {
        objectBase.panelX = attackStateBase.movePanelX;
        objectBase.panelY = attackStateBase.movePanelY;
        em_sub_BlockToPos(gba);
        btlhit_sub_BattleHitBlockPosSet(gba);
        attackStateBase.state = MimikyuShadowSneakState::Reappear;
        attackStateBase.stateInitialized = false;
    }
    else
    {
        em_sub_EnemyFlipCheck(gba);
        int flip = gba->r0;
        gba->r4 = (flip << 0x18) | (MIMIKYU_ANIMATION_SHADOW_SNEAK_SHADOW << 0x10) | (MIMIKYU_SPRITE_INDEX << 8) | (MIMIKYU_SPRITE_CATEGORY);
        gba->r1 = objectBase.positionX;
        gba->r2 = objectBase.positionY;
        gba->r3 = objectBase.positionZ;
        gba->r6 = 0xFFFF;
        gba->r7 = 0x08;
        efc28_SetEfc28(gba);
    }

}

static void mimikyu_shadow_sneak_reappear(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        gba->r0 = 0x157;
        sound_SoundSeReq(gba);
        objectBase.animation = MIMIKYU_ANIMATION_SHADOW_SNEAK_END;
        attackStateBase.timer = 20;
        objectBase.flip = 1;
        em_sub_EnemyFlipCheck(gba);
        obj_sub_ObjFlipSet(gba);
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer <= 0)
    {
        attackStateBase.state = MimikyuShadowSneakState::Attack;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_shadow_sneak_attack(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.animation = MIMIKYU_ANIMATION_SHADOW_SNEAK_ATTACK;
        attackStateBase.timer = 40;
        attackStateBase.timer2 = 0;
        navi_sub_NaviCounterTimerSet(gba);
        return;
    }
    attackStateBase.timer2 += 1;
    if ((attackStateBase.timer2 < 40 - 18) && ((attackStateBase.timer2 & 8) != 0))
    {
        em_sub_CalcPlEmDirSpd(gba);
        gba->r0 = objectBase.panelX + gba->r0;
        gba->r1 = objectBase.panelY;
        gba->r2 = 0x11;
        gba->r3 = objectBase.alliance | (objectBase.flip << 0x08);
        block_sub_HitBlockFlashSub(gba);
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer == 40 - 18)
    {
        gba->r0 = 0xB0;
        sound_SoundSeReq(gba);
        em_sub_CalcPlEmDirSpd(gba);
        gba->r0 = objectBase.panelX + gba->r0;
        gba->r1 = objectBase.panelY;
        gba->r2 = AttackElement_Sword;
        gba->r3 = 0x00;
        gba->r4 = 0x12;
        gba->r6 = (10 << 0x10) | MIMIKYU_SHADOWSNEAK_DAMAGE;
        shl96_SetShl96(gba);
    }
    else if (attackStateBase.timer <= 0)
    {
        attackStateBase.state = MimikyuShadowSneakState::Return;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_shadow_sneak_return(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.flip = 0;
        em_sub_EnemyFlipCheck(gba);
        obj_sub_ObjFlipSet(gba);
        objectBase.panelX = objectBase.reservedPanelX;
        objectBase.panelY = objectBase.reservedPanelY;
        gba->r0 = objectBase.panelX;
        gba->r1 = objectBase.panelY;
        reserve_ReserveReset(gba);
        gba->r0 = attackStateBase.movePanelX;
        gba->r1 = attackStateBase.movePanelY;
        reserve_ReserveReset(gba);
        em_sub_BlockToPos(gba);
        gba->r0 = 1;
        btlhit_sub_BattleHitOn(gba);
        btlhit_sub_BattleHitBlockPosSet(gba);
        gba->r0 = 0x40;
        btlhit_sub1_BattleHitStatusFlagOff(gba);
        objectBase.animation = MIMIKYU_ANIMATION_MOVE_IN;
        attackStateBase.timer = 4;
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer <= 0)
    {
        naviatk_sub_EmAtkReset(gba);
    }
}
