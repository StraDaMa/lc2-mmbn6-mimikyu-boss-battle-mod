#include "pch.h"
#include "mimikyu.h"
#include "mimikyu_confuseray_object.h"

enum MimikyuConfuseRayState {
    Begin = 0,
    Attack,
    End,
};

static void mimikyu_confuseray_begin(GbaState* gba);
static void mimikyu_confuseray_attack(GbaState* gba);
static void mimikyu_confuseray_end(GbaState* gba);

uint32_t mimikyu_confuseray(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    switch (attackStateBase.state)
    {
    case MimikyuConfuseRayState::Begin:
        mimikyu_confuseray_begin(gba);
        break;
    case MimikyuConfuseRayState::Attack:
        mimikyu_confuseray_attack(gba);
        break;
    case MimikyuConfuseRayState::End:
        mimikyu_confuseray_end(gba);
        break;
    }
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}

static void mimikyu_confuseray_begin(GbaState* gba)
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
        attackStateBase.state = MimikyuConfuseRayState::Attack;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_confuseray_attack(GbaState* gba)
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
        gba->r0 = objectBase.panelX;
        gba->r1 = objectBase.panelY;
        gba->r2 = AttackElement_None;
        gba->r3 = 10 << 0x10;
        gba->r4 = 0;
        gba->r6 = (10 << 0x10) | MIMIKYU_CONFUSERAY_DAMAGE;
        mimikyu_confuseray_object_spawn(gba);
        gba->r1 = objectBase.positionX;
        gba->r2 = objectBase.positionY;
        gba->r3 = objectBase.positionZ;
        gba->r4 = 03;
        efc00_SetEfc00(gba);
        return;
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer < 0)
    {
        attackStateBase.state = MimikyuConfuseRayState::End;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_confuseray_end(GbaState* gba)
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
