#include "pch.h"
#include "mimikyu.h"
#include "mimikyu_star_object.h"

enum StarObjectState
{
    Init = 0,
    Update = 4,
    Delete = 8,
};

static void mimikyu_star_object_init(GbaState* gba);
static void mimikyu_star_object_update(GbaState* gba);

void mimikyu_star_object_main(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    switch (objectBase.state)
    {
    case StarObjectState::Init:
        mimikyu_star_object_init(gba);
        break;
    case StarObjectState::Update:
        mimikyu_star_object_update(gba);
        break;
    case StarObjectState::Delete:
        obj_mv_ShlClose(gba);
        break;
    }
}

static void mimikyu_star_object_init(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    gba->r0 = 0x80;
    gba->r1 = MIMIKYU_SPRITE_CATEGORY;
    gba->r2 = MIMIKYU_SPRITE_INDEX;
    objtrans_ObjCharInit(gba);
    obj_sub_ObjNoShadow(gba);
    objectBase.flags |= 0x02;
    objectBase.animation = MIMIKYU_ANIMATION_STAR;
    gba->r0 = MIMIKYU_ANIMATION_STAR;
    obj_sub_ObjDmaSeqSet(gba);
    objtrans_ObjCharSet(gba);
    obj_sub_ObjRotOpen(gba);
    rand_Rand2(gba);
    uint32_t randValue = gba->r0;
    int startAngle = randValue & 0x7F;
    randValue >>= 8;
    if (objectBase.parameter1 != 0)
        startAngle += 0x80;
    objectBase.damage = startAngle;
    int startScaleFactor = randValue & 0x0F;
    objectBase.parameter1 = startScaleFactor + 0x40;
    randValue >>= 4;
    int startRotation = randValue & 0x1F;
    randValue >>= 5;
    objectBase.timer2 = startRotation;
    objectBase.speedX = 0;
    objectBase.timer = 10;

    objectBase.state = StarObjectState::Update;
    objectBase.substate = 0;
    objectBase.substate2 = 0;
    objectBase.substate3 = 0;
    mimikyu_star_object_update(gba);

}

static void mimikyu_star_object_update(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    int rotation = (uint16_t)objectBase.timer2;
    rotation += 0x10;
    objectBase.timer2 += rotation;
    gba->r0 = rotation >> 0x08;
    gba->r1 = objectBase.parameter1;
    gba->r2 = objectBase.parameter1;
    obj_sub_ObjRotCalc(gba);
    int slowdownAmount = objectBase.speedX;
    slowdownAmount += 0xC0;
    if (slowdownAmount >= 0x00000800)
        slowdownAmount = 0x00000800;
    objectBase.speedX = slowdownAmount;
    int speed = 0x00000800 - slowdownAmount;
    int angle = objectBase.damage;
    int16_t* sinTable = (int16_t*)(ramBase + math_sin_table);
    int16_t* cosTable = (int16_t*)(ramBase + math_cos_table);
    objectBase.positionX += cosTable[angle] * speed;
    objectBase.positionY += sinTable[angle] * speed;

    objectBase.timer -= 1;
    if (objectBase.timer <= 0)
    {
        objectBase.state = StarObjectState::Delete;
        objectBase.substate = 0;
        objectBase.substate2 = 0;
        objectBase.substate3 = 0;
    }
    btlobj_sub_BattleObjCharMove(gba);
}

void mimikyu_star_object_spawn(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);

    mimikyu_multiobject3_spawn(gba);
    BattleObject& newobjectBase = *(BattleObject*)(ramBase + gba->r0);
    newobjectBase.multiObjectIndex = MIMIKYU_MULTIOBJ_STAR;
}