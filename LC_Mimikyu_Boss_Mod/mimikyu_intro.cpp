#include "pch.h"
#include "mimikyu.h"
#include "duskball_object.h"
#include "duskball_effect_object.h"
#include "audio.h"

enum MimikyuIntroState {
    Init = 0,
    Update = 4,
    End = 8,
};


enum MimikyuIntroUpdateState {
    BallThrow = 0,
    BallEffect,
    Drop,
    Idle,
};

static void mimikyu_intro_init(GbaState* gba);
static void mimikyu_intro_update(GbaState* gba);

static void mimikyu_intro_ball_throw(GbaState* gba);
static void mimikyu_intro_ball_effect(GbaState* gba);
static void mimikyu_intro_drop(GbaState* gba);
static void mimikyu_intro_idle(GbaState* gba);

uint32_t mimikyu_intro(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    switch (objectBase.substate2)
    {
    case 0:
        mimikyu_intro_init(gba);
        break;
    case 4:
        mimikyu_intro_update(gba);
        break;
    case 8:
        // Original end code
        emcomm_EmCommonEntry2_02(gba);
        break;
    }
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}

void mimikyu_intro_init(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    // Do original entry code here to set up boilerplate
    //emcomm_EmCommonEntry2_00(gba);
    objectBase.flags &= ~0x02;
    if (!objectBase.substate3) {
        gba->r0 = 0;
        btl_sub1_EntryEmAction(gba);
        objectBase.substate3 = true;
    }
    sub1_BattleReportFlagGet(gba);
    if (gba->r0 & 0x01) {
        gba->r0 = 0;
        btl_sub1_CheckEmActionEnable(gba);
        if (gba->r0) {
            objectBase.substate2 = MimikyuIntroState::Update;
            objectBase.substate3 = 0;
        }
    }
}

void mimikyu_intro_update(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    switch (objectBase.substate3)
    {
        case MimikyuIntroUpdateState::BallThrow:
            mimikyu_intro_ball_throw(gba);
            break;
        case MimikyuIntroUpdateState::BallEffect:
            mimikyu_intro_ball_effect(gba);
            break;
        case MimikyuIntroUpdateState::Drop:
            mimikyu_intro_drop(gba);
            break;
        case MimikyuIntroUpdateState::Idle:
            mimikyu_intro_idle(gba);
            break;
    }
    // Bosses don't normally update at this stage
    // This sprite object function does
    btlobj_sub_BattleObjCharMove5(gba);
}

void mimikyu_intro_ball_throw(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    objectBase.flags &= ~0x02;
    if (!objectBase.substate4) {
        objectBase.substate4 = true;
        // Shift up to where the ball is going to create the effect
        objectBase.positionZ = 30 << 0x10;
        gba->r7 = gba->r5 + offsetof(BattleObject, parentObject2);
        duskball_object_spawn(gba);
        objectBase.parentObject2 = gba->r0;
        return;
    }
    // This will be set to 0 when the duskball throw ends
    if (objectBase.parentObject2 == 0) {
        objectBase.substate3 = MimikyuIntroUpdateState::BallEffect;
        objectBase.substate4 = 0;
    }
}

void mimikyu_intro_ball_effect(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    objectBase.flags &= ~0x02;
    if (!objectBase.substate4) {
        objectBase.substate4 = true;
        objectBase.timer = 20;
        // Spawn portal effect
        gba->r1 = objectBase.positionX;
        gba->r2 = objectBase.positionY;
        gba->r3 = objectBase.positionZ - (22 << 0x10);
        gba->r4 = 0x00;
        duskball_effect_object_spawn(gba);
        // Spawn shine effect
        gba->r1 = objectBase.positionX;
        gba->r2 = objectBase.positionY;
        gba->r3 = objectBase.positionZ;
        gba->r4 = 0x03;
        efc00_SetEfc00(gba);
        // Enable flags to update during the intro
        BattleObject& effectObject = *(BattleObject*)(ramBase + gba->r0);
        effectObject.flags |= 0x14;
    }
    objectBase.timer -= 1;
    if (objectBase.timer <= 0)
    {
        objectBase.substate3 = MimikyuIntroUpdateState::Drop;
        objectBase.substate4 = 0;
    }
}

void mimikyu_intro_drop(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    if (!objectBase.substate4) {
        objectBase.substate4 = true;
        // Finally make sprite visable
        objectBase.flags |= 0x02;
        // This looks like a jumping animation
        objectBase.animation = MIMIKYU_ANIMATION_SPECIAL_ATTACK_BEGIN;
        // Make Mimikyu do a small jump before reaching the ground
        const int totalFrames = 24;
        objectBase.timer = totalFrames;
        gba->r0 = gba->r5 + 0x34;
        gba->r1 = objectBase.positionX;
        gba->r2 = objectBase.positionY;
        gba->r3 = 0;
        gba->r4 = totalFrames;
        gba->r6 = 0xFFFF8000;
        sub1_CalcTrajectory4(gba);
        objectBase.speedX = gba->r0;
        objectBase.speedY = gba->r1;
        objectBase.speedZ = gba->r2;
        // Play mimikyu cry
        lc_post_sound_event(PLAY_WEMS_MIMIKYU_CRY_WEM, 1);
        // Spawn 4 sparkle effects randomly around Mimikyu
        for (int i = 0; i < 4; i++)
        {
            rand_Rand2(gba);
            uint32_t randNumber = gba->r0;
            int xOffset = (randNumber & 0xFF) % 32;
            xOffset -= 16;
            int yOffset = ((randNumber >> 8) & 0xFF) % 32;
            yOffset += 16;
            // Spawn sparkle effects
            gba->r1 = objectBase.positionX + (xOffset << 0x10);
            gba->r2 = objectBase.positionY;
            gba->r3 = objectBase.positionZ + (yOffset << 0x10);
            gba->r4 = 0x26 | (1 << 0x10);
            efc00_SetEfc00(gba);
            // Enable flags to update during the intro
            BattleObject& effectObject = *(BattleObject*)(ramBase + gba->r0);
            effectObject.flags |= 0x14;
        }
    }
    objectBase.positionX += objectBase.speedX;
    objectBase.positionY += objectBase.speedY;
    objectBase.positionZ += objectBase.speedZ;
    objectBase.speedZ += (int32_t)0xFFFF8000;
    objectBase.timer -= 1;
    if (objectBase.timer <= 0)
    {
        // Set this to 0 in case it was slightly off after the jump
        objectBase.positionZ = 0;
        objectBase.substate3 = MimikyuIntroUpdateState::Idle;
        objectBase.substate4 = 0;
    }
}

void mimikyu_intro_idle(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    if (!objectBase.substate4) {
        objectBase.substate4 = true;
        objectBase.timer = 24;
        objectBase.animation = MIMIKYU_ANIMATION_IDLE;
    }
    objectBase.timer -= 1;
    if (objectBase.timer <= 0)
    {
        // End intro
        gba->r0 = 0;
        btl_sub1_CloseEmAction(gba);
        objectBase.substate2 = MimikyuIntroState::End;
        objectBase.substate3 = 0;
        objectBase.substate4 = 0;
    }
}