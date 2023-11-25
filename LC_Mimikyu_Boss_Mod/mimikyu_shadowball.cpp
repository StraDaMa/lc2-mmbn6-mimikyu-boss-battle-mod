#include "pch.h"
#include "mimikyu.h"
#include "mimikyu_shadowball_object.h"

enum MimikyuShadowBallState {
    Begin = 0,
    Attack,
    End,
};

static void mimikyu_shadowball_begin(GbaState* gba);
static void mimikyu_shadowball_attack(GbaState* gba);
static void mimikyu_shadowball_end(GbaState* gba);

uint32_t mimikyu_shadowball(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    switch (attackStateBase.state)
    {
    case MimikyuShadowBallState::Begin:
        mimikyu_shadowball_begin(gba);
        break;
    case MimikyuShadowBallState::Attack:
        mimikyu_shadowball_attack(gba);
        break;
    case MimikyuShadowBallState::End:
        mimikyu_shadowball_end(gba);
        break;
    }
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}

static void mimikyu_shadowball_begin(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.animation = MIMIKYU_ANIMATION_SPECIAL_ATTACK_BEGIN;
        attackStateBase.timer = 18;
        navi_sub_NaviCounterTimerSet(gba);
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer < 0)
    {
        attackStateBase.state = MimikyuShadowBallState::Attack;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_shadowball_attack(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.animation = MIMIKYU_ANIMATION_SPECIAL_ATTACK_LOOP;
        attackStateBase.timer = 30;
        gba->r7 = attackStateBase.enemyOffset;
        gba->r0 = objectBase.panelX;
        gba->r1 = objectBase.panelY;
        gba->r2 = AttackElement_None;
        gba->r3 = 10 << 0x10;
        gba->r4 = 0;
        gba->r6 = (10 << 0x10) | MIMIKYU_SHADOWBALL_DAMAGE;
        mimikyu_shadowball_object_spawn(gba);
        return;
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer < 0)
    {
        attackStateBase.state = MimikyuShadowBallState::End;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_shadowball_end(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.animation = MIMIKYU_ANIMATION_IDLE;
        attackStateBase.timer = 16;
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer < 0)
    {
        naviatk_sub_EmAtkReset(gba);
    }
}
