#include "c_char_data.h"
#include "constants.h"

// All the battle stat calculations goes here, this is
// the "Active" stats in combat based on primary stats.
void Char_data::calcActiveStats() {
		speed = lvl*LVL_SPEED_FACTOR + agi*AGI_SPEED_FACTOR;
		HP = lvl * LVL_HP_FACTOR + vit * VIT_HP_FACTOR;
		SP = lvl * LVL_SP_FACTOR + intel * INT_SP_FACTOR;
		attack = (lvl * LVL_ATK_FACTOR + pwr * PWR_ATK_FACTOR	+ WEAPON_ATK * WEP_ATK_FACTOR) * wep_train;
		defense = lvl + ARMOR_DEF_VAL * ARMOR_DEF_FACTOR + vit * VIT_DEF_FACTOR + dex * DEX_DEF_FACTOR;
		hit = (lvl * LVL_HIT_FACTOR + dex * DEX_HIT_FACTOR + luk * LUK_HIT_FACTOR) * wep_train;
		evade = lvl * LVL_EVADE_FACTOR + agi * AGI_EVADE_FACTOR + dex * DEX_EVADE_FACTOR + luk * LUK_EVADE_FACTOR;
}
