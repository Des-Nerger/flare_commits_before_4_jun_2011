/**
 * class StatBlock
 *
 * Character stats and calculations
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "StatBlock.h"

StatBlock::StatBlock() {
	alive = true;
	offense = defense = physical = magical = 0;
	physoff = physdef = magoff = magdef = 0;
	
	level = 1;
	
	// physical stats
	hp = 12 + physical * 4;
	maxhp = hp;
	hp_per_minute = 0;
	hp_ticker = 0;
	
	// magical stats
	mp = magical * 2;
	maxmp = mp;
	mp_per_minute = 0;
	mp_ticker = 0;
	
	// offense stats
	accuracy = 75 + offense * 5;
	
	// defense stats
	avoidance = 25 + defense * 5;

	// overall stats
	crit = 0;

	// equipment stats	
	dmg_melee_min = 1;
	dmg_melee_max = 4;
	minabsorb = 0;
	maxabsorb = 0;
	resist_fire = 0;
	resist_ice = 0;
	
	// buff and debuff stats
	slowed = false;
	slowed_duration = 0;
	bleeding = false;
	bleeding_duration = 0;
	stunned = false;
	stunned_duration = 0;
	
	temphp = 0;
	offensive_stance = false;
	defensive_stance = false;
	reflect = false;
	
}

/**
 * Reduce temphp first, then hp
 */
void StatBlock::takeDamage(int dmg) {
	if (temphp > 0) {
		temphp -= dmg;
		if (temphp < 0) {
			hp += temphp;
			temphp = 0;
		}
	}
	else {
		hp -= dmg;
	}
	if (hp <= 0) {
		hp = 0;
		alive = false;
	}
}

/**
 * Recalc derived stats from base stats
 * Creatures might skip these formulas.
 */
void StatBlock::recalc() {
	hp = 12 + physical * 4;
	maxhp = hp;
	mp = magical * 4;
	maxmp = mp;
	accuracy = 75 + offense * 5;
	avoidance = 25 + defense * 5;
	physoff = physical + offense;
	physdef = physical + defense;
	magoff = magical + offense;
	magdef = magical + defense;
	crit = physical + magical + offense + defense;
}

/**
 * Process per-frame actions
 */
void StatBlock::logic() {

	// health regen
	if (hp_per_minute > 0 && hp < maxhp && hp > 0) {
		hp_ticker++;
		if (hp_ticker >= (60 * FRAMES_PER_SEC)/hp_per_minute) {
			hp++;
			hp_ticker = 0;
		}
	}

	// mana regen
	if (mp_per_minute > 0 && mp < maxmp && hp > 0) {
		mp_ticker++;
		if (mp_ticker >= (60 * FRAMES_PER_SEC)/mp_per_minute) {
			mp++;
			mp_ticker = 0;
		}
	}
	
	// handle debuff durations
	if (slowed_duration > 0) {
		slowed_duration--;
		if (slowed_duration == 0) slowed = false;
	}
	if (bleeding_duration > 0) {
		bleeding_duration--;
		if (bleeding_duration == 0) bleeding = false;
	}
	if (stunned_duration > 0) {
		stunned_duration--;
		if (stunned_duration == 0) stunned = false;
	}

}

StatBlock::~StatBlock() {
}

