#pragma once
#include "GbaState.h"

//Mimikyu Object IDs
enum MIMIKYU_MULTIOBJ
{
    MIMIKYU_MULTIOBJ_SHADOWBALL = 0x00,
    MIMIKYU_MULTIOBJ_CONFUSERAY,
    MIMIKYU_MULTIOBJ_STAR
};

struct BattleObject
{
    uint8_t flags; //00
    uint8_t index; //01
    uint8_t typeSpriteOffset; //02
    uint8_t memoryIndex; //03
    uint8_t parameter1; //04
    uint8_t parameter2; //05
    uint8_t parameter3; //06
    uint8_t parameter4; //07
    uint8_t state; //08
    uint8_t substate; //09
    uint8_t substate2; //0A
    uint8_t substate3; //0B
    uint8_t padding[0x4]; //0C-0F
    uint8_t animation; //10
    uint8_t previousAnimation; //11
    uint8_t panelX; //12
    uint8_t panelY; //13
    uint8_t reservedPanelX; //14
    uint8_t reservedPanelY; //15
    uint8_t alliance; //16
    uint8_t flip; //17
    uint8_t padding2[0x8]; //18-1F
    int16_t timer; //20
    int16_t timer2; //22
    uint8_t padding3[0x8];//24-2B
    uint16_t damage; //2C
    uint16_t moodDamage; //2E
    uint8_t padding4[0x4]; //30-33
    int32_t positionX; //34
    int32_t positionY; //38
    int32_t positionZ; //3C
    int32_t speedX; //40
    int32_t speedY; //44
    int32_t speedZ; //48
    uint32_t parentObject; //4C
    uint32_t parentObject2; //50
    uint32_t collisionData; //54
    uint32_t aiData; //58
    uint8_t padding5[0x33];//5C-8E
    uint8_t multiObjectIndex; //8F
};

#define MIMIKYU_MULTIOBJECT_ID 0x2C

// Attack Elements
#define AttackElement_None 0x00
#define AttackElement_Heat 0x01
#define AttackElement_Aqua 0x02
#define AttackElement_Elec 0x03
#define AttackElement_Wood 0x04
// secondary elements are bits
#define AttackElement_Break 0x10
#define AttackElement_Wind 0x20
#define AttackElement_Cursor 0x40
#define AttackElement_Sword 0x80

uint32_t mimikyu_multiobject3_main(GbaState* gba);
void mimikyu_multiobject3_spawn(GbaState* gba);