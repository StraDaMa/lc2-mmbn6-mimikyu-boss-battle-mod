#include "pch.h"
#include "objects.h"
#include "duskball_object.h"
#include "mimikyu.h"
#include <array>

enum DuskBallEffectObjectState
{
    Init = 0,
    Update = 4,
    Delete = 8,
};

enum DuskBallEffectUpdateState
{
    Grow = 0,
    Idle,
    Vanish,
};

static void duskball_effect_object_init(GbaState* gba);
static void duskball_effect_object_update(GbaState* gba);
static void duskball_effect_object_update_grow(GbaState* gba);
static void duskball_effect_object_update_idle(GbaState* gba);
static void duskball_effect_object_update_vanish(GbaState* gba);

void duskball_effect_object_main(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    switch (objectBase.state)
    {
    case DuskBallEffectObjectState::Init:
        duskball_effect_object_init(gba);
        break;
    case DuskBallEffectObjectState::Update:
        duskball_effect_object_update(gba);
        break;
    case DuskBallEffectObjectState::Delete:
        emcomm_ShlCommonExit(gba);
        break;
    }
    btlobj_sub_BattleObjCharMove5(gba);
}

void duskball_effect_object_init(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    gba->r0 = 0x80;
    //EraseMan ghost portal sprite
    gba->r1 = 0x10;
    gba->r2 = 0x4B;
    objtrans_ObjCharInit(gba);
    obj_sub_ObjNoShadow(gba);
    objectBase.flags |= 0x02;
    gba->r0 = 0x01;
    obj_sub_ObjDmaSeqSet(gba);
    objtrans_ObjCharSet(gba);
    // Enable scaling, this also means it cannot be flipped
    obj_sub_ObjRotOpen(gba);
    objectBase.state = DuskBallEffectObjectState::Update;
    objectBase.substate = DuskBallEffectUpdateState::Grow;
    objectBase.substate2 = 0;
    objectBase.substate3 = 0;
    duskball_effect_object_update(gba);
}

void duskball_effect_object_update(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    switch (objectBase.substate)
    {
    case Grow:
        duskball_effect_object_update_grow(gba);
        break;
    case Idle:
        duskball_effect_object_update_idle(gba);
        break;
    case Vanish:
        duskball_effect_object_update_vanish(gba);
        break;
    }
}

const static std::array<uint8_t, 6> scale_table = {
    0x40,
    0x30,
    0x40,
    0x60,
    0x80,
    0xF0,
};

void duskball_effect_object_update_grow(GbaState* gba){
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    if (!objectBase.substate2) {
        objectBase.substate2 = true;
        objectBase.timer = 24;
    }
    objectBase.timer--;

    uint8_t scale_factor = scale_table[objectBase.timer / 4];
    gba->r0 = 0;
    gba->r1 = scale_factor;
    gba->r2 = scale_factor;
    obj_sub_ObjRotCalc(gba);
    if (objectBase.timer < 0) {
        objectBase.substate = DuskBallEffectUpdateState::Idle;
        objectBase.substate2 = 0;
        objectBase.substate3 = 0;
    }
}

void duskball_effect_object_update_idle(GbaState* gba){
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    if (!objectBase.substate2) {
        objectBase.substate2 = true;
        objectBase.timer = 12;
    }
    objectBase.timer--;
    if (objectBase.timer <= 0) {
        objectBase.substate = DuskBallEffectUpdateState::Vanish;
        objectBase.substate2 = 0;
        objectBase.substate3 = 0;
    }
}

void duskball_effect_object_update_vanish(GbaState* gba){
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    if (!objectBase.substate2) {
        objectBase.substate2 = true;
        objectBase.timer = 12;
    }
    objectBase.timer--;

    uint8_t scale_factor = scale_table[5 - (objectBase.timer / 2)];
    gba->r0 = 0;
    gba->r1 = scale_factor;
    gba->r2 = scale_factor;
    obj_sub_ObjRotCalc(gba);
    if (objectBase.timer < 0) {
        objectBase.state = DuskBallEffectObjectState::Delete;
        objectBase.substate = 0;
        objectBase.substate2 = 0;
        objectBase.substate3 = 0;
    }
}



void duskball_effect_object_spawn(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    mimikyu_multiobject3_spawn(gba);
    BattleObject& newobjectBase = *(BattleObject*)(ramBase + gba->r0);
    // Update during timestop and opening
    newobjectBase.flags |= 0x14;
    newobjectBase.multiObjectIndex = MIMIKYU_MULTIOBJ_DUSKBALL_EFFECT;
}
