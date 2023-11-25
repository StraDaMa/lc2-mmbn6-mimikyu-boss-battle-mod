#include "pch.h"
#include "mimikyu.h"
#include "mimikyu_star_object.h"

enum MimikyuPlayRoughState {
    Move = 0,
    Jump,
    Attack,
    Return
};

static void mimikyu_playrough_begin(GbaState* gba);
static void mimikyu_playrough_jump(GbaState* gba);
static void mimikyu_playrough_attack(GbaState* gba);
static void mimikyu_playrough_return(GbaState* gba);

uint32_t mimikyu_playrough(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    switch (attackStateBase.state)
    {
    case MimikyuPlayRoughState::Move:
        mimikyu_playrough_begin(gba);
        break;
    case MimikyuPlayRoughState::Jump:
        mimikyu_playrough_jump(gba);
        break;
    case MimikyuPlayRoughState::Attack:
        mimikyu_playrough_attack(gba);
        break;
    case MimikyuPlayRoughState::Return:
        mimikyu_playrough_return(gba);
        break;
    }
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}

static void mimikyu_playrough_begin(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.animation = MIMIKYU_ANIMATION_PLAYROUGH_BEGIN;
        uint8_t* enemyObjectBase = ramBase + attackStateBase.enemyOffset;
        attackStateBase.movePanelX = enemyObjectBase[0x12];
        attackStateBase.movePanelY = enemyObjectBase[0x13];
        attackStateBase.timer = 30;
        navi_sub_NaviCounterTimerSet(gba);
    }
    attackStateBase.timer -= 1;
    if ((attackStateBase.timer & 0x8) != 0)
    {
        gba->r0 = attackStateBase.movePanelX;
        gba->r1 = attackStateBase.movePanelY;
        block_sub_BlockFlash(gba);
    }
    if (attackStateBase.timer <= 0)
    {
        attackStateBase.state = MimikyuPlayRoughState::Jump;
        attackStateBase.stateInitialized = false;
    }
}

static void mimikyu_playrough_jump(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.animation = MIMIKYU_ANIMATION_PLAYROUGH_JUMP;
        objectBase.reservedPanelX = objectBase.panelX;
        objectBase.reservedPanelY = objectBase.panelY;
        gba->r0 = objectBase.panelX;
        gba->r1 = objectBase.panelY;
        reserve_ReserveSet(gba);
        gba->r0 = 0x40;
        btlhit_sub1_BattleHitStatusFlagOn(gba);
        gba->r0 = attackStateBase.movePanelX;
        gba->r1 = attackStateBase.movePanelY;
        em_sub_GetBlockPos(gba);
        int destCoordX = gba->r0;
        int destCoordY = gba->r1;
        int destCoordZ = 0;
        int totalFrames = 16;
        attackStateBase.timer = totalFrames;
        gba->r0 = gba->r5 + 0x34;
        gba->r1 = destCoordX;
        gba->r2 = destCoordY;
        gba->r3 = destCoordZ;
        gba->r4 = totalFrames;
        gba->r6 = 0xFFFF8000;
        sub1_CalcTrajectory4(gba);
        objectBase.speedX = gba->r0;
        objectBase.speedY = gba->r1;
        objectBase.speedZ = gba->r2;
        btlhit_sub_BattleHitOff(gba);
        gba->r0 = 0x94;
        sound_SoundSeReq(gba);
    }
    objectBase.positionX += objectBase.speedX;
    objectBase.positionY += objectBase.speedY;
    objectBase.positionZ += objectBase.speedZ;
    objectBase.speedZ += (int32_t)0xFFFF8000;
    attackStateBase.timer -= 1;
    if (attackStateBase.timer <= 0)
    {
        attackStateBase.state = MimikyuPlayRoughState::Attack;
        attackStateBase.stateInitialized = false;
    }
}
static void mimikyu_playrough_attack(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        attackStateBase.timer = 0;
        attackStateBase.timer2 = 5 + 1;
        objectBase.panelX = attackStateBase.movePanelX;
        objectBase.panelY = attackStateBase.movePanelY;
        em_sub_BlockToPos(gba);
        objectBase.positionZ = 0;
        btlhit_sub_BattleHitBlockPosSet(gba);
    }
    attackStateBase.timer -= 1;
    if (attackStateBase.timer <= 0)
    {
        attackStateBase.timer2 -= 1;
        if (attackStateBase.timer2 <= 0)
        {
            attackStateBase.state = MimikyuPlayRoughState::Return;
            attackStateBase.stateInitialized = false;
        }
        else
        {
            gba->r0 = 0x6B;
            sound_SoundSeReq(gba);
            attackStateBase.timer = 12;
            gba->r0 = objectBase.panelX;
            gba->r1 = objectBase.panelY;
            gba->r2 = AttackElement_None;
            gba->r3 = 0x00;
            gba->r4 = 0x04050501;
            gba->r6 = (10 << 0x10) | MIMIKYU_PLAYROUGH_DAMAGE;
            gba->r7 = 0x01;
            shl03_SetShl03(gba);
            for (size_t i = 0; i < 2; i++)
            {
                rand_Rand2(gba);
                uint32_t randValue = gba->r0;
                int xOffset = randValue & 0x1F;
                int yOffset = (randValue >> 0x08) & 0x1F;
                xOffset = (xOffset << 27) >> (27 - 16);
                yOffset = yOffset << 0x10;
                int xCoord = objectBase.positionX;
                int yCoord = objectBase.positionY;
                int zCoord = objectBase.positionZ;
                gba->r1 = xCoord + xOffset;
                gba->r2 = yCoord;
                gba->r3 = zCoord + yOffset;
                gba->r4 = 0x040000;
                efc00_SetEfc00(gba);
            }
            for (size_t i = 0; i < 2; i++)
            {
                rand_Rand2(gba);
                uint32_t randValue = gba->r0;
                int xOffset = randValue & 0x1F;
                int yOffset = (randValue >> 0x08) & 0x1F;
                xOffset = (xOffset << 27) >> (27 - 16);
                yOffset = yOffset << 0x10;
                int xCoord = objectBase.positionX;
                int yCoord = objectBase.positionY;
                int zCoord = objectBase.positionZ;
                gba->r1 = xCoord + xOffset;
                gba->r2 = yCoord;
                gba->r3 = zCoord + yOffset;
                gba->r4 = 0x02;
                efc00_SetEfc00(gba);
            }
            rand_Rand2(gba);
            uint32_t randValue = gba->r0;
            int xOffset = randValue & 0xF;
            int yOffset = (randValue >> 0x08) & 0xF;
            xOffset = (xOffset << 28) >> (28 - 16);
            yOffset = yOffset << 0x10;
            int xCoord = objectBase.positionX;
            int yCoord = objectBase.positionY;
            int zCoord = objectBase.positionZ;
            gba->r1 = xCoord + xOffset;
            gba->r2 = yCoord + yOffset;
            gba->r3 = zCoord;
            gba->r4 = 0x57;
            efc00_SetEfc00(gba);

            rand_Rand2(gba);
            randValue = gba->r0;
            xOffset = randValue & 0xF;
            yOffset = (randValue >> 0x08) & 0xF;
            xOffset += 8;
            yOffset += 12;
            xOffset = (xOffset << 27) >> (27 - 16);
            yOffset <<= 0x10;
            xCoord = objectBase.positionX;
            yCoord = objectBase.positionY;
            zCoord = objectBase.positionZ;
            gba->r1 = xCoord + xOffset;
            gba->r2 = yCoord;
            gba->r3 = zCoord + yOffset;
            gba->r4 = (xOffset > 0) ? 1 : 0;
            mimikyu_star_object_spawn(gba);
        }
    }
}

static void mimikyu_playrough_return(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    if (attackStateBase.stateInitialized == false)
    {
        attackStateBase.stateInitialized = true;
        objectBase.panelX = objectBase.reservedPanelX;
        objectBase.panelY = objectBase.reservedPanelY;
        gba->r0 = objectBase.panelX;
        gba->r1 = objectBase.panelY;
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