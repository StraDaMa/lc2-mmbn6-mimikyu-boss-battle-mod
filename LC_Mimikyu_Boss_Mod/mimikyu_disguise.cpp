#include "pch.h"
#include "mimikyu.h"

enum MimikyuDisguiseAttackState {
    Begin = 0,
    Idle,
    End,
};

static void mimikyu_disguise_begin(GbaState* gba);
static void mimikyu_disguise_loop(GbaState* gba);
static void mimikyu_disguise_return(GbaState* gba);

uint32_t mimikyu_disguise(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    switch (attackStateBase.state)
    {
    case MimikyuDisguiseAttackState::Begin:
        mimikyu_disguise_begin(gba);
        break;
    case MimikyuDisguiseAttackState::Idle:
        mimikyu_disguise_loop(gba);
        break;
    case MimikyuDisguiseAttackState::End:
        mimikyu_disguise_return(gba);
        break;
    }
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}

static void mimikyu_disguise_begin(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.animation = MIMIKYU_ANIMATION_DISGUISE_BEGIN;
        aiStateBase.disguiseState = DisguiseState::Broken;
        objectBase.panelX = objectBase.reservedPanelX;
        objectBase.panelY = objectBase.reservedPanelY;
        gba->r0 = objectBase.panelX;
        gba->r1 = objectBase.panelY;
        reserve_ReserveReset(gba);
        em_sub_BlockToPos(gba);
        btlhit_sub_BattleHitBlockPosSet(gba);
        objectBase.positionZ = 0;
        mimikyu_on_damage2(gba);
        gba->r0 = 0x100041;
        btlhit_sub1_BattleHitStatusFlagOff(gba);
        attackStateBase.timer = 12;
        gba->r0 = 0xB1;
        sound_SoundSeReq(gba);
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer <= 0)
    {
        attackStateBase.state = MimikyuDisguiseAttackState::Idle;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_disguise_loop(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.animation = MIMIKYU_ANIMATION_DISGUISE;
        attackStateBase.timer = 90;
        gba->r0 = 0xE2;
        sound_SoundSeReq(gba);
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer <= 0)
    {
        attackStateBase.state = MimikyuDisguiseAttackState::End;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_disguise_return(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.animation = MIMIKYU_ANIMATION_DISGUISE_END;
        attackStateBase.timer = 12;
        gba->r0 = 0xB2;
        sound_SoundSeReq(gba);
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer <= 0)
    {
        naviatk_sub_EmAtkReset(gba);
    }
}
