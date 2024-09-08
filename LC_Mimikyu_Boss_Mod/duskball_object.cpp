#include "pch.h"
#include "objects.h"
#include "duskball_object.h"
#include "mimikyu.h"

enum DuskBallObjectState
{
    Init = 0,
    Update = 4,
    Delete = 8,
};

static void duskball_object_init(GbaState* gba);
static void duskball_object_update(GbaState* gba);

void duskball_object_main(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    switch (objectBase.state)
    {
    case DuskBallObjectState::Init:
        duskball_object_init(gba);
        break;
    case DuskBallObjectState::Update:
        duskball_object_update(gba);
        break;
    case DuskBallObjectState::Delete:
        emcomm_ShlCommonExit(gba);
        break;
    }
    btlobj_sub_BattleObjCharMove5(gba);
}

void duskball_object_init(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    const BattleObject& parentObject = *(BattleObject*)(ramBase + objectBase.parentObject);
    // Set the position of this object to just off screen of mimikyus side of the battlefield
    objectBase.panelX = (objectBase.alliance == 0) ? 0 : 7;
    objectBase.panelY = parentObject.panelY;
    objectBase.positionZ = parentObject.positionZ;
    em_sub_BlockToPos(gba);
    gba->r0 = 0x80;
    gba->r1 = MIMIKYU_SPRITE_CATEGORY;
    gba->r2 = MIMIKYU_SPRITE_INDEX;
    objtrans_ObjCharInit(gba);
    obj_sub_ObjShadowSet(gba);
    objectBase.flags |= 0x02;
    gba->r0 = MIMIKYU_ANIMATION_DUSKBALL;
    obj_sub_ObjDmaSeqSet(gba);
    objtrans_ObjCharSet(gba);
    em_sub_EnemyFlipCheck(gba);
    obj_sub_ObjFlipSet(gba);
    objectBase.state = DuskBallObjectState::Update;
    objectBase.substate = 0;
    objectBase.substate2 = 0;
    objectBase.substate3 = 0;
    duskball_object_update(gba);
}

void duskball_object_update(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    if (!objectBase.substate){
        objectBase.substate = true;
        const int totalFrames = 40;
        objectBase.timer = totalFrames;

        gba->r0 = gba->r5 + 0x34;
        const BattleObject& parentObject = *(BattleObject*)(ramBase + objectBase.parentObject);
        gba->r1 = parentObject.positionX;
        gba->r2 = parentObject.positionY;
        gba->r3 = parentObject.positionZ;
        gba->r4 = totalFrames;
        gba->r6 = 0xFFFFC000;
        sub1_CalcTrajectory4(gba);
        objectBase.speedX = gba->r0;
        objectBase.speedY = gba->r1;
        objectBase.speedZ = gba->r2;
        // Play throwing sound
        //gba->r0 = 0xB2;
        gba->r0 = 0x103;
        sound_SoundSeReq(gba);
    }
    objectBase.positionX += objectBase.speedX;
    objectBase.positionY += objectBase.speedY;
    objectBase.positionZ += objectBase.speedZ;
    objectBase.speedZ += (int32_t)0xFFFFC000;
    objectBase.timer -= 1;
    if (objectBase.timer <= 0)
    {
        // Play ball open sound
        gba->r0 = 0x123;
        sound_SoundSeReq(gba);
        objectBase.state = DuskBallObjectState::Delete;
        // Clear reference to this object from Mimikyu so the attack ends
        *(uint32_t*)(ramBase + objectBase.parentObject2) = 0x00000000;
    }
}

void duskball_object_spawn(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    mimikyu_multiobject3_spawn(gba);
    BattleObject& newobjectBase = *(BattleObject*)(ramBase + gba->r0);
    // Update during timestop and opening
    newobjectBase.flags |= 0x14;
    newobjectBase.multiObjectIndex = MIMIKYU_MULTIOBJ_DUSKBALL;
    newobjectBase.parentObject2 = gba->r7;
}
