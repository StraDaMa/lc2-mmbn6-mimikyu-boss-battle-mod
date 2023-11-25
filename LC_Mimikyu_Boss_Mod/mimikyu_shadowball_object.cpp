#include "pch.h"
#include "mimikyu_shadowball_object.h"
#include "objects.h"
#include "mimikyu.h"

enum ShadowBallObjectState
{
    Init = 0,
    Update = 4,
    Delete = 8,
};

enum ShadowBallObjectSubstate
{
    Idle = 0,
    Launch,
};

static void mimikyu_shadowball_object_init(GbaState* gba);
static void mimikyu_shadowball_object_update(GbaState* gba);
static void mimikyu_shadowball_object_idle(GbaState* gba);
static void mimikyu_shadowball_object_launch(GbaState* gba);

void mimikyu_shadowball_object_main(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    switch (objectBase.state)
    {
    case ShadowBallObjectState::Init:
        mimikyu_shadowball_object_init(gba);
        break;
    case ShadowBallObjectState::Update:
        mimikyu_shadowball_object_update(gba);
        break;
    case ShadowBallObjectState::Delete:
        emcomm_ShlCommonExit(gba);
        break;
    }
}

static void mimikyu_shadowball_object_init(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    em_sub_BlockToPos(gba);
    gba->r0 = 0x80;
    gba->r1 = MIMIKYU_SPRITE_CATEGORY;
    gba->r2 = MIMIKYU_SPRITE_INDEX;
    objtrans_ObjCharInit(gba);
    obj_sub_ObjNoShadow(gba);
    objectBase.flags |= 0x02;
    objectBase.animation = MIMIKYU_ANIMATION_SHADOWBALL;
    gba->r0 = MIMIKYU_ANIMATION_SHADOWBALL;
    obj_sub_ObjDmaSeqSet(gba);
    objtrans_ObjCharSet(gba);
    em_sub_EnemyFlipCheck(gba);
    obj_sub_ObjFlipSet(gba);
    btlhit0_BattleHitOpen(gba);
    if (gba->r0 == 0)
    {
        obj_mv_ShlClose(gba);
        return;
    }
    gba->r1 = 0x04;
    gba->r2 = 0x05;
    gba->r3 = 0x03;
    btlhit_sub_BattleHitDataSet(gba);
    btlhit_sub_BattleHitSet(gba);
    objectBase.state = ShadowBallObjectState::Update;
    objectBase.substate = 0;
    objectBase.substate2 = 0;
    objectBase.substate3 = 0;
    mimikyu_shadowball_object_update(gba);
}

static void mimikyu_shadowball_object_update(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);

    btlhit_sub_BattleHitCheck(gba);
    btlhit_sub1_BattleHitHitMarkCheck(gba);
    btl_sub_BattleEndCheck(gba);
    if (gba->r0 != 0)
    {
        btlhit_sub_BattleHitOff(gba);
        objectBase.state = ShadowBallObjectState::Delete;
        objectBase.substate = 0;
        objectBase.substate2 = 0;
        objectBase.substate3 = 0;
        return;
    }
    uint8_t* collisionDataBase = ramBase + objectBase.collisionData;
    if (*(uint32_t*)(collisionDataBase + 0x70) != 0)
    {
        gba->r0 = 0x70;
        sound_SoundSeReq(gba);
        gba->r1 = objectBase.positionX;
        gba->r2 = objectBase.positionY;
        gba->r3 = objectBase.positionZ;
        gba->r4 = 0x010000;
        efc00_SetEfc00(gba);
        btlhit_sub_BattleHitOff(gba);
        objectBase.state = ShadowBallObjectState::Delete;
        objectBase.substate = 0;
        objectBase.substate2 = 0;
        objectBase.substate3 = 0;
        return;
    }
    switch (objectBase.substate)
    {
    case ShadowBallObjectSubstate::Idle:
        mimikyu_shadowball_object_idle(gba);
        break;
    case ShadowBallObjectSubstate::Launch:
        mimikyu_shadowball_object_launch(gba);
        break;
    }
    btlhit_sub_BattleHitSet(gba);
    btlobj_sub_BattleObjCharMove(gba);
}

static void mimikyu_shadowball_object_idle(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    if (objectBase.substate2 == 0)
    {
        objectBase.substate2 = 4;
        objectBase.timer = 30;
        gba->r0 = 0xFC;
        sound_SoundSeReq(gba);
    }
    objectBase.timer -= 1;
    if (objectBase.timer <= 0)
    {
        objectBase.substate = ShadowBallObjectSubstate::Launch;
        objectBase.substate2 = 0;
    }
}

static void mimikyu_shadowball_object_launch(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    em_sub_CalcPlEmDirSpd(gba);
    int32_t direction = gba->r0;
    if (objectBase.substate2 == 0)
    {
        objectBase.substate2 = 4;
        objectBase.speedX = direction * 0xB000;
        return;
    }
    gba->r0 = objectBase.panelX;
    gba->r1 = objectBase.panelY;
    em_sub_GetBlockPos(gba);
    int currentPanelY = gba->r1;
    BattleObject& opponentObjectBase = *(BattleObject*)(ramBase + objectBase.parentObject2);
    int shadowBallXCoord = objectBase.positionX;
    int opponentXCoord = opponentObjectBase.positionX;
    if ((direction * (shadowBallXCoord - opponentXCoord)) < 0)
    {
        int shadowBallYCoord = objectBase.positionY;
        int opponentYCoord = opponentObjectBase.positionY;
        if (opponentYCoord == shadowBallYCoord)
        {
            objectBase.speedY = 0;
        }
        else if (opponentYCoord >= shadowBallYCoord)
        {
            objectBase.speedY = 0xB000;
        }
        else
        {
            objectBase.speedY = -1 * 0xB000;
        }
    }
    objectBase.positionX += objectBase.speedX;
    objectBase.speedX += direction * 0x400;
    int oldYCoord = objectBase.positionY;
    objectBase.positionY += objectBase.speedY;
    if (oldYCoord != currentPanelY)
    {
        gba->r0 = objectBase.positionY;
        gba->r1 = oldYCoord;
        gba->r2 = currentPanelY;
        em_sub_EnemyPointArriveCheck(gba);
        if (gba->r0 != 0)
        {
            objectBase.positionY = currentPanelY;
        }
    }
    em_sub_PosToBlock(gba);
    btlhit_sub_BattleHitBlockPosSet(gba);
    block_sub_BlockInScreenCheck(gba);
    if (gba->r0 == 0)
    {
        btlhit_sub_BattleHitOff(gba);
        objectBase.state = ShadowBallObjectState::Delete;
        objectBase.substate = 0;
        objectBase.substate2 = 0;
        objectBase.substate3 = 0;
        return;
    }
    block_sub_HitBlockFlash(gba);
}

void mimikyu_shadowball_object_spawn(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    uint32_t oldr0 = gba->r0;
    uint32_t oldr1 = gba->r1;
    uint32_t oldr2 = gba->r2;
    uint32_t oldr5 = gba->r5;
    mimikyu_multiobject3_spawn(gba);
    BattleObject& newobjectBase = *(BattleObject*)(ramBase + gba->r0);
    gba->r1 = oldr0;
    gba->r2 = oldr1;
    gba->r3 = oldr2;
    gba->r5= oldr5;
    shl_sub_ShlInitDataSet(gba);
    newobjectBase.multiObjectIndex = MIMIKYU_MULTIOBJ_SHADOWBALL;
    newobjectBase.parentObject2 = gba->r7;

}
