#include "pch.h"
#include "mimikyu_confuseray_object.h"
#include "mimikyu.h"
#include "objects.h"

enum ConfuseRayObjectState
{
    Init = 0,
    Update = 4,
    Delete = 8,
};

enum ConfuseRayObjectSubstate
{
    Idle = 0,
    Launch,
};

static void mimikyu_confuseray_object_init(GbaState* gba);
static void mimikyu_confuseray_object_update(GbaState* gba);
static void mimikyu_confuseray_object_idle(GbaState* gba);
static void mimikyu_confuseray_object_launch(GbaState* gba);

void mimikyu_confuseray_object_main(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    switch (objectBase.state)
    {
    case ConfuseRayObjectState::Init:
        mimikyu_confuseray_object_init(gba);
        break;
    case ConfuseRayObjectState::Update:
        mimikyu_confuseray_object_update(gba);
        break;
    case ConfuseRayObjectState::Delete:
        emcomm_ShlCommonExit(gba);
        break;
    }
}

static void mimikyu_confuseray_object_init(GbaState* gba)
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
    objectBase.animation = MIMIKYU_ANIMATION_CONFUSERAY;
    gba->r0 = MIMIKYU_ANIMATION_CONFUSERAY;
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
    gba->r3 = 0x00;
    btlhit_sub_BattleHitDataSet(gba);
    gba->r0 = 0x20;
    btlhit_sub1_BattleHitAbnormalStatusSet(gba);
    btlhit_sub_BattleHitSet(gba);
    int verticalDirection = 1;
    if (objectBase.panelY <= 1)
    {
        verticalDirection *= -1;
    }
    em_sub_CalcPlEmDirSpd(gba);
    objectBase.speedX = gba->r0 * 0x018000;
    objectBase.speedZ = verticalDirection * (0x10000 / (0x500000 / 0x020000));
    objectBase.timer2 = 0x800;
    objectBase.speedY = objectBase.positionY - (verticalDirection * 0x060000);
    objectBase.state = ConfuseRayObjectState::Update;
    objectBase.substate = 0;
    objectBase.substate2 = 0;
    objectBase.substate3 = 0;
    mimikyu_confuseray_object_update(gba);
}
static void mimikyu_confuseray_object_update(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    uint8_t* collisionDataBase = ramBase + objectBase.collisionData;

    btlhit_sub_BattleHitCheck(gba);
    btlhit_sub1_BattleHitHitMarkCheck(gba);
    btl_sub_BattleEndCheck(gba);
    if (gba->r0 != 0)
    {
        btlhit_sub_BattleHitOff(gba);
        objectBase.state = ConfuseRayObjectState::Delete;
        objectBase.substate = 0;
        objectBase.substate2 = 0;
        objectBase.substate3 = 0;
    }
    if (*(uint32_t*)(collisionDataBase + 0x70) != 0)
    {
        btlhit_sub_BattleHitOff(gba);
        objectBase.state = ConfuseRayObjectState::Delete;
        objectBase.substate = 0;
        objectBase.substate2 = 0;
        objectBase.substate3 = 0;
        return;
    }
    switch (objectBase.substate)
    {
    case ConfuseRayObjectSubstate::Idle:
        mimikyu_confuseray_object_idle(gba);
        break;
    case ConfuseRayObjectSubstate::Launch:
        mimikyu_confuseray_object_launch(gba);
        break;
    }
    block_sub_HitBlockFlash(gba);
    btlhit_sub_BattleHitSet(gba);
    btlobj_sub_BattleObjCharMove(gba);
}

static void mimikyu_confuseray_object_idle(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    if (objectBase.substate2 == 0)
    {
        objectBase.substate2 = 4;
        objectBase.timer = 20;
        gba->r0 = 0x13A;
        sound_SoundSeReq(gba);
    }
    objectBase.timer -= 1;
    if (objectBase.timer <= 0)
    {
        objectBase.substate = ConfuseRayObjectSubstate::Launch;
        objectBase.substate2 = 0x00;
    }
}

static void mimikyu_confuseray_object_launch(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    objectBase.timer2 += objectBase.speedZ;
    objectBase.positionX += objectBase.speedX;
    int16_t* sinTable = (int16_t*)(ramBase + math_sin_table);
    int degrees = (uint16_t)objectBase.timer2 >> 0x08;
    int addValue = (sinTable[degrees] * 12) << 0x08;
    objectBase.positionY = objectBase.speedY + addValue;
    em_sub_PosToBlock(gba);
    btlhit_sub_BattleHitBlockPosSet(gba);
    block_sub_BlockInScreenCheck(gba);
    if (gba->r0 == 0)
    {
        btlhit_sub_BattleHitOff(gba);
        objectBase.state = ConfuseRayObjectState::Delete;
        objectBase.substate = 0;
        objectBase.substate2 = 0;
        objectBase.substate3 = 0;
    }
}

void mimikyu_confuseray_object_spawn(GbaState* gba)
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
    gba->r5 = oldr5;
    shl_sub_ShlInitDataSet(gba);
    newobjectBase.multiObjectIndex = MIMIKYU_MULTIOBJ_CONFUSERAY;
}