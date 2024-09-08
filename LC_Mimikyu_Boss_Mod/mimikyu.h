#pragma once
#include "GbaState.h"
#include "objects.h"

uint32_t mimikyu_intro(GbaState* gba);
uint32_t mimikyu_update_ai(GbaState* gba);
uint32_t mimikyu_move(GbaState* gba);
uint32_t mimikyu_shadowball(GbaState* gba);
uint32_t mimikyu_confuseray(GbaState* gba);
uint32_t mimikyu_shadow_claw(GbaState* gba);
uint32_t mimikyu_shadow_sneak(GbaState* gba);
uint32_t mimikyu_playrough(GbaState* gba);
uint32_t mimikyu_proc_disguise(GbaState* gba);
uint32_t mimikyu_disguise(GbaState* gba);
uint32_t mimikyu_on_damage(GbaState* gba);
void mimikyu_on_damage2(GbaState* gba);

enum class DisguiseState : uint8_t
{
    Broken = 0,
    Active,
    BreakProcced,
};

struct MimikyuAIState
{
    bool lowHP; //00
    uint8_t timesMoved;  //01
    uint8_t mediumAttackPoints; //02
    uint8_t bigAttackPoints; //03
    DisguiseState disguiseState; //04
};

struct MimikyuAttackState
{
    uint8_t state; //00
    bool stateInitialized; //01
    uint8_t padding[0x0E]; //02-0F
    int16_t timer; //10
    int16_t timer2; //12
    uint8_t movePanelX; //14
    uint8_t movePanelY; //15
    uint8_t padding2[0x16]; //16-2B
    uint32_t enemyOffset; //2C
    uint32_t attackObject; //30
};

// Constants
#define MIMIKYU_SPRITE_CATEGORY 0x08
#define MIMIKYU_SPRITE_INDEX 0x11

const std::array<uint32_t, 4> mimikyu_move_parameters = {
    0x00000010,
    0x0F8800A0,
    0x00000030,
    0x0F880080,
};

// Attack Damage
#define MIMIKYU_SHADOWBALL_DAMAGE 100
#define MIMIKYU_CONFUSERAY_DAMAGE 50
#define MIMIKYU_SHADOWCLAW_DAMAGE 160
#define MIMIKYU_SHADOWSNEAK_DAMAGE 160
#define MIMIKYU_PLAYROUGH_DAMAGE 60

//Mimikyu Animation macros
enum MIMIKYU_ANIMATION
{
    MIMIKYU_ANIMATION_IDLE = 0,
    MIMIKYU_ANIMATION_HIT,
    MIMIKYU_ANIMATION_STATUS,
    MIMIKYU_ANIMATION_MOVE_IN,
    MIMIKYU_ANIMATION_MOVE_OUT,
    MIMIKYU_ANIMATION_DISGUISE_BEGIN,
    MIMIKYU_ANIMATION_DISGUISE,
    MIMIKYU_ANIMATION_DISGUISE_END,
    MIMIKYU_ANIMATION_SPECIAL_ATTACK_BEGIN,
    MIMIKYU_ANIMATION_SPECIAL_ATTACK_LOOP,
    MIMIKYU_ANIMATION_PHYSICAL_ATTACK,
    MIMIKYU_ANIMATION_SHADOW_SNEAK_BEGIN,
    MIMIKYU_ANIMATION_SHADOW_SNEAK_SHADOW,
    MIMIKYU_ANIMATION_SHADOW_SNEAK_END,
    MIMIKYU_ANIMATION_SHADOW_SNEAK_ATTACK,
    MIMIKYU_ANIMATION_PLAYROUGH_BEGIN,
    MIMIKYU_ANIMATION_PLAYROUGH_JUMP,
    MIMIKYU_ANIMATION_SHADOWCLAW_SLASH,
    MIMIKYU_ANIMATION_STAR,
    MIMIKYU_ANIMATION_SHADOWBALL,
    MIMIKYU_ANIMATION_CONFUSERAY,
    MIMIKYU_ANIMATION_DUSKBALL,
};

//Mimikyu Attack Macros
enum MIMIKYU_ATTACK
{
    MIMIKYU_ATTACK_UPDATE = 0x08,
    MIMIKYU_ATTACK_MOVE,
    MIMIKYU_ATTACK_SHADOWBALL,
    MIMIKYU_ATTACK_CONFUSERAY,
    MIMIKYU_ATTACK_SHADOWCLAW,
    MIMIKYU_ATTACK_SHADOWSNEAK,
    MIMIKYU_ATTACK_PLAYROUGH,
    MIMIKYU_ATTACK_DISGUISE,
};