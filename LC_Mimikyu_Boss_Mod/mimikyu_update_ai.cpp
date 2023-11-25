#include "pch.h"
#include "mimikyu.h"

// Update AI to always use the same attack
//uint32_t mimikyu_update_ai(GbaState* gba)
//{
//    GbaStateStoreAllEssential(gba);
//    uint8_t* ramBase = gba->ramBase;
//    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
//    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + *(uint32_t*)(objectBase + 0x58) + 0x80);
//    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
//
//    // Set byte used for Disguise
//    aiStateBase.disguiseState = DisguiseState::Active;
//    // Find enemy and store reference to it
//    gba->r0 = objectBase.alliance ^ 1;
//    navi_sub2_GetNaviAdrs2(gba);
//    attackStateBase.enemyOffset = gba->r0;
//    gba->r0 = MIMIKYU_ATTACK_MOVE;
//    naviatk_sub_EmAtkSet(gba);
//    GbaStateRestoreAllEssential(gba);
//    return gba->r15;
//}

uint32_t mimikyu_update_ai(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + gba->r6);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + gba->r7);
    // Check if Disguise was procced
    if (aiStateBase.disguiseState == DisguiseState::BreakProcced)
    {
        gba->r0 = MIMIKYU_ATTACK_DISGUISE;
        naviatk_sub_EmAtkSet(gba);
        GbaStateRestoreAllEssential(gba);
        return gba->r15;
    }
    // Set byte used for Disguise
    aiStateBase.disguiseState = DisguiseState::Active;
    // Find enemy and store reference to it
    gba->r0 = objectBase.alliance ^ 1;
    navi_sub2_GetNaviAdrs2(gba);
    attackStateBase.enemyOffset = gba->r0;
    // Check for enough movements for an attack
    if (aiStateBase.timesMoved < 2)
    {
        // Add a random number of points towards midtier of attack
        rand_Rand2(gba);
        uint32_t randValue = gba->r0;
        aiStateBase.mediumAttackPoints += randValue & 7;
        randValue >>= 8;
        // Sometimes move less than 3 times by adding a random value
        aiStateBase.timesMoved += 1 + (randValue & 1);
        gba->r0 = MIMIKYU_ATTACK_MOVE;
        naviatk_sub_EmShiftAtkSet(gba);
        GbaStateRestoreAllEssential(gba);
        return gba->r15;
    }
    // Doing an attack resets move counter
    aiStateBase.timesMoved = 0;
    //Check high tier attack
    if (aiStateBase.bigAttackPoints >= 3)
    {
        aiStateBase.mediumAttackPoints = 0;
        aiStateBase.bigAttackPoints = 0;
        gba->r0 = MIMIKYU_ATTACK_PLAYROUGH;
        naviatk_sub_EmAtkSet(gba);
    }
    else if (aiStateBase.mediumAttackPoints >= 25)
    {
        //lose a couple points but maintain progress towards high tier
        aiStateBase.mediumAttackPoints -= 13;
        //increase progress towards high tier attack
        aiStateBase.bigAttackPoints += 1;
        rand_Rand2(gba);
        uint32_t randValue = gba->r0 & 0x0F;
        if (randValue >= 9)
        {
            gba->r0 = MIMIKYU_ATTACK_SHADOWSNEAK;
            naviatk_sub_EmAtkSet(gba);
        }
        else
        {
            gba->r0 = MIMIKYU_ATTACK_SHADOWCLAW;
            naviatk_sub_EmAtkSet(gba);
        }
    }
    else
    {
        rand_Rand2(gba);
        uint32_t randValue = gba->r0 & 0x0F;
        if (randValue >= 0x08)
        {
            gba->r0 = MIMIKYU_ATTACK_CONFUSERAY;
            naviatk_sub_EmAtkSet(gba);
        }
        else
        {
            gba->r0 = MIMIKYU_ATTACK_SHADOWBALL;
            naviatk_sub_EmAtkSet(gba);
        }
    }
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}