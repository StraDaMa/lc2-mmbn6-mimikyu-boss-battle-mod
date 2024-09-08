#include "pch.h"
#include "objects.h"
#include "mimikyu_shadowball_object.h"
#include "mimikyu_confuseray_object.h"
#include "mimikyu_star_object.h"
#include "duskball_object.h"
#include "duskball_effect_object.h"

// Putting all new objects used by Mimikyu in a single object
// so each object doesnt take up an object slot
uint32_t mimikyu_multiobject3_main(GbaState* gba)
{
    GbaStateStoreAllEssential(gba);
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    switch ((MIMIKYU_MULTIOBJ)(objectBase.multiObjectIndex))
    {
    case MIMIKYU_MULTIOBJ_SHADOWBALL:
        mimikyu_shadowball_object_main(gba);
        break;
    case MIMIKYU_MULTIOBJ_CONFUSERAY:
        mimikyu_confuseray_object_main(gba);
        break;
    case MIMIKYU_MULTIOBJ_STAR:
        mimikyu_star_object_main(gba);
        break;
    case MIMIKYU_MULTIOBJ_DUSKBALL:
        duskball_object_main(gba);
        break;
    case MIMIKYU_MULTIOBJ_DUSKBALL_EFFECT:
        duskball_effect_object_main(gba);
        break;
    }

    GbaStateRestoreAllEssential(gba);
    return gba->r15;
}

void mimikyu_multiobject3_spawn(GbaState* gba)
{
    uint8_t* ramBase = gba->ramBase;
    BattleObject& objectBase = *(BattleObject*)(ramBase + gba->r5);
    uint32_t oldR5 = gba->r5;
    gba->r0 = MIMIKYU_MULTIOBJECT_ID;
    obj_mv_ShlOpen(gba);
    gba->r0 = gba->r5;
    BattleObject& newObjectBase = *(BattleObject*)(ramBase + gba->r0);
    gba->r5 = oldR5;
    newObjectBase.alliance = objectBase.alliance;
    newObjectBase.parentObject = oldR5;

}