#include "pch.h"
#include "mimikyu.h"


uint32_t mimikyu_on_damage(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    mimikyu_on_damage2(gba);
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}

void mimikyu_on_damage2(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    // get attack state (r7 during attacks)
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + objectBase.aiData + 0xA0);
    // Reset Flip
    objectBase.flip = 0;
    em_sub_EnemyFlipCheck(gba);
    obj_sub_ObjFlipSet(gba);
    // Reset collision region because some attacks remove it
    gba->r0 = 1;
    btlhit_sub_BattleHitOn(gba);
    // remove panels temporarily reserved during attacks
    gba->r0 = attackStateBase.movePanelX;
    gba->r1 = attackStateBase.movePanelY;
    reserve_ReserveReset(gba);
}

uint32_t mimikyu_proc_disguise(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    uint8_t* collisionDataBase = ramBase + objectBase.collisionData;
    MimikyuAIState& aiStateBase = *(MimikyuAIState*)(ramBase + objectBase.aiData + 0x80);
    MimikyuAttackState& attackStateBase = *(MimikyuAttackState*)(ramBase + objectBase.aiData + 0xA0);
    if (aiStateBase.disguiseState != DisguiseState::Broken)
    {
        int totalDamage = *(uint16_t*)(collisionDataBase + 0x80);
        if (totalDamage >= 10)
        {
            *(uint16_t*)(collisionDataBase + 0x80 + 0x00) = 0;
            *(uint16_t*)(collisionDataBase + 0x80 + 0x02) = 0;
            *(uint16_t*)(collisionDataBase + 0x80 + 0x04) = 0;
            *(uint16_t*)(collisionDataBase + 0x80 + 0x08) = 0;
            *(uint16_t*)(collisionDataBase + 0x80 + 0x0A) = 0;

            *(uint16_t*)(collisionDataBase + 0x8E) = 0;

            *(uint32_t*)(collisionDataBase + 0x74) = 0;
            collisionDataBase[0x0F] = 0;
            collisionDataBase[0x11] = 0;
            *(uint16_t*)(collisionDataBase + 0xA4) = 0;
            // Dont reset paralyze on counter hit
            if((*(uint32_t*)(collisionDataBase + 0x70) & 0x40) == 0)
                *(uint16_t*)(collisionDataBase + 0x1C) = 0;
            *(uint16_t*)(collisionDataBase + 0x2A) = 0;
            *(uint16_t*)(collisionDataBase + 0x2C) = 0;
            *(uint32_t*)(collisionDataBase + 0x70) &= ~(0x301BE);
            *(uint32_t*)(collisionDataBase + 0x40) = 0;
            // Let mimikyu finish the current attack
            // Allow canceling movement though
            if (objectBase.substate <= MIMIKYU_ATTACK_MOVE)
            {
                gba->r0 = MIMIKYU_ATTACK_DISGUISE;
                naviatk_sub_EmAtkSet(gba);
            }
            else
            {
                aiStateBase.disguiseState = DisguiseState::BreakProcced;
            }
            
        }
    }
    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}
