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
	corpse = false;
	hero = false;
	targeted = 0;
	
	// core stats
	offense = defense = physical = magical = 0;
	physoff = physdef = magoff = magdef = 0;
	level = 0;
	hp = maxhp = hp_per_minute = hp_ticker = 0;
	mp = maxmp = mp_per_minute = mp_ticker = 0;
	accuracy = 75;
	avoidance = 25;
	crit = 0;
	
	//controls
	mouse_move = false;

	// equipment stats	
	dmg_melee_min = 1;
	dmg_melee_max = 4;
	dmg_magic_min = 0;
	dmg_magic_max = 0;
	dmg_ranged_min = 0;
	dmg_ranged_max = 0;
	absorb_min = 0;
	absorb_max = 0;
	ammo_stones = false;
	ammo_arrows = false;
	
	// buff and debuff stats
	slow_duration = 0;
	bleed_duration = 0;
	stun_duration = 0;
	immobilize_duration = 0;
	immunity_duration = 0;	
	shield_hp = 0;
	shield_frame = 0;
	vengeance_stacks = 0;
	vengeance_frame = 0;
	cooldown_ticks = 0;
	blocking = false;
	
	// xp table
	// what experience do you need to reach the next level
	// formula:
	// scale 20-50-100-200-500-1000-2000-5000
	// multiplied by current level
	// affter 10000, increase by 5k each level
	
	xp_table[0] = 0;
	xp_table[1] = 20;
	xp_table[2] = 100;
	xp_table[3] = 300;
	xp_table[4] = 800;
	xp_table[5] = 2500;
	xp_table[6] = 6000;
	xp_table[7] = 14000;
	xp_table[8] = 40000;
	xp_table[9] = 90000;
	xp_table[10] = 150000;
	xp_table[11] = 220000;
	xp_table[12] = 300000;
	xp_table[13] = 390000;
	xp_table[14] = 490000;
	xp_table[15] = 600000;
	xp_table[16] = 720000;
	xp_table[17] = -1;

	teleportation=false;
	
	for (int i=0; i<POWERSLOT_COUNT; i++) {
		power_chance[i] = 0;
		power_index[i] = 0;
		power_cooldown[i] = 0;
		power_ticks[i] = 0;
	}
	melee_range = 64;
	
	melee_weapon_power = -1;
	ranged_weapon_power = -1;
	magic_weapon_power = -1;
	
	attunement_fire = 100;
	attunement_ice = 100;

	gold = 0;
	death_penalty = false;
}

/**
 * load a statblock, typically for an enemy definition
 */
void StatBlock::load(string filename) {
	ifstream infile;
	string line;
	string key;
	string val;
	string starts_with;
	int num;
	
	infile.open(filename.c_str(), ios::in);

	if (infile.is_open()) {
		while (!infile.eof()) {

			line = getLine(infile);

			if (line.length() > 0) {
			
			
				starts_with = line.at(0);
				
				if (starts_with == "#") {
					// skip comments
				}
				else if (starts_with == "[") {
					// skip headers
				}
				else { // this is data.  treatment depends on section type
					parse_key_pair(line, key, val);          
					key = trim(key, ' ');
					val = trim(val, ' ');
					num = atoi(val.c_str());
					
					if (key == "name") name = val;
					else if (key == "sfx_prefix") sfx_prefix = val;
					else if (key == "gfx_prefix") gfx_prefix = val;
					
					else if (key == "level") level = num;
					else if (key == "xp") xp = num;
					
					else if (key == "loot_chance") loot_chance = num;
					
					// combat stats
					else if (key == "hp") {
						hp = num;
						maxhp = num;
					}
					else if (key == "mp") {
						mp = num;
						maxmp = num;
					}
					else if (key == "cooldown") cooldown = num;
					else if (key == "accuracy") accuracy = num;
					else if (key == "avoidance") avoidance = num;
					else if (key == "dmg_melee_min") dmg_melee_min = num;
					else if (key == "dmg_melee_max") dmg_melee_max = num;
					else if (key == "dmg_magic_min") dmg_magic_min = num;
					else if (key == "dmg_magic_max") dmg_magic_max = num;
					else if (key == "dmg_ranged_min") dmg_ranged_min = num;
					else if (key == "dmg_ranged_max") dmg_ranged_max = num;
					else if (key == "absorb_min") absorb_min = num;
					else if (key == "absorb_max") absorb_max = num;
					
					// behavior stats
					else if (key == "speed") speed = num;
					else if (key == "dspeed") dspeed = num;
					else if (key == "dir_favor") dir_favor = num;
					else if (key == "chance_pursue") chance_pursue = num;
					else if (key == "chance_flee") chance_flee = num;

					else if (key == "chance_melee_phys") power_chance[MELEE_PHYS] = num;
					else if (key == "chance_melee_mag") power_chance[MELEE_MAG] = num;
					else if (key == "chance_ranged_phys") power_chance[RANGED_PHYS] = num;
					else if (key == "chance_ranged_mag") power_chance[RANGED_MAG] = num;
					else if (key == "power_melee_phys") power_index[MELEE_PHYS] = num;
					else if (key == "power_melee_mag") power_index[MELEE_MAG] = num;
					else if (key == "power_ranged_phys") power_index[RANGED_PHYS] = num;
					else if (key == "power_ranged_mag") power_index[RANGED_MAG] = num;
					else if (key == "cooldown_melee_phys") power_cooldown[MELEE_PHYS] = num;
					else if (key == "cooldown_melee_mag") power_cooldown[MELEE_MAG] = num;
					else if (key == "cooldown_ranged_phys") power_cooldown[RANGED_PHYS] = num;
					else if (key == "cooldown_ranged_mag") power_cooldown[RANGED_MAG] = num;
					
					else if (key == "melee_range") melee_range = num;
					else if (key == "threat_range") threat_range = num;
					
					else if (key == "attunement_fire") attunement_fire=num;
					else if (key == "attunement_ice") attunement_ice=num;

					// animation stats
					else if (key == "render_size_x") render_size.x = num;
					else if (key == "render_size_y") render_size.y = num;
					else if (key == "render_offset_x") render_offset.x = num;
					else if (key == "render_offset_y") render_offset.y = num;
					else if (key == "anim_stance_position") anim_stance_position = num; 
					else if (key == "anim_stance_frames") anim_stance_frames = num;
					else if (key == "anim_stance_duration")anim_stance_duration = num;
					else if (key == "anim_run_position") anim_run_position = num;
					else if (key == "anim_run_frames") anim_run_frames = num;
					else if (key == "anim_run_duration") anim_run_duration = num;
					else if (key == "anim_melee_position") anim_melee_position = num;
					else if (key == "anim_melee_frames") anim_melee_frames = num;
					else if (key == "anim_melee_duration") anim_melee_duration = num;
					else if (key == "anim_magic_position") anim_magic_position = num;
					else if (key == "anim_magic_frames") anim_magic_frames = num;
					else if (key == "anim_magic_duration") anim_magic_duration = num;
					else if (key == "anim_ranged_position")anim_ranged_position = num;
					else if (key == "anim_ranged_frames") anim_ranged_frames = num;
					else if (key == "anim_ranged_duration")anim_ranged_duration = num;
					else if (key == "anim_block_position") anim_block_position = num;
					else if (key == "anim_block_frames") anim_block_frames = num;
					else if (key == "anim_block_duration") anim_block_duration = num;
					else if (key == "anim_hit_position") anim_hit_position = num;
					else if (key == "anim_hit_frames") anim_hit_frames = num;
					else if (key == "anim_hit_duration") anim_hit_duration = num;
					else if (key == "anim_die_position") anim_die_position = num;
					else if (key == "anim_die_frames") anim_die_frames = num;
					else if (key == "anim_die_duration") anim_die_duration = num;
					else if (key == "anim_critdie_position") anim_critdie_position = num;
					else if (key == "anim_critdie_frames") anim_critdie_frames = num;
					else if (key == "anim_critdie_duration") anim_critdie_duration = num;
					else if (key == "melee_weapon_power") melee_weapon_power = num;
					else if (key == "magic_weapon_power") magic_weapon_power = num;
					else if (key == "ranged_weapon_power") ranged_weapon_power = num;
	
				}
			}
		}
	}
	infile.close();
}

/**
 * Reduce temphp first, then hp
 */
void StatBlock::takeDamage(int dmg) {
	if (shield_hp > 0) {
		shield_hp -= dmg;
		if (shield_hp < 0) {
			hp += shield_hp;
			shield_hp = 0;
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

	hp = maxhp = 12 + physical * 4;
	mp = maxmp = magical * 4;
			
	accuracy = 70 + offense * 5;
	avoidance = 20 + defense * 5;
	physoff = physical + offense;
	physdef = physical + defense;
	magoff = magical + offense;
	magdef = magical + defense;
	crit = physical + magical + offense + defense;
	hp_per_minute = 9 + physical;
	mp_per_minute = 9 + magical;
	
	for (int i=1; i<17; i++) {
		if (xp >= xp_table[i])
			level=i+1;
	}
}

/**
 * Process per-frame actions
 */
void StatBlock::logic() {

	// handle cooldowns
	if (cooldown_ticks > 0) cooldown_ticks--; // global cooldown

	for (int i=0; i<POWERSLOT_COUNT; i++) { // NPC/enemy powerslot cooldown
		if (power_ticks[i] > 0) power_ticks[i]--;
	}

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
	if (slow_duration > 0)
		slow_duration--;
	if (bleed_duration > 0)
		bleed_duration--;
	if (stun_duration > 0)
		stun_duration--;
	if (immobilize_duration > 0)
		immobilize_duration--;
	if (immunity_duration > 0)
		immunity_duration--;
	
	// apply bleed
	if (bleed_duration % FRAMES_PER_SEC == 1) {
		takeDamage(1);
	}
	
	// handle targeted
	if (targeted > 0)
		targeted--;
		
	// handle buff/debuff animations
	shield_frame++;
	if (shield_frame == 12) shield_frame = 0;
	
	vengeance_frame+= vengeance_stacks;
	if (vengeance_frame >= 24) vengeance_frame -= 24;

}

/**
 * Remove temporary buffs/debuffs
 */
void StatBlock::clearEffects() {
	immunity_duration = 0;
	immobilize_duration = 0;
	bleed_duration = 0;
	stun_duration = 0;
	shield_hp = 0;
	vengeance_stacks = 0;
}


/**
 * Get the renderable for various effects on the player (buffs/debuffs)
 *
 * @param effect_type STAT_EFFECT_* consts defined in StatBlock.h
 */
Renderable StatBlock::getEffectRender(int effect_type) {
	Renderable r;
	r.map_pos.x = pos.x;
	r.map_pos.y = pos.y;
	
	if (effect_type == STAT_EFFECT_SHIELD) {
		r.src.x = (shield_frame/3) * 128;
		r.src.y = 0;
		r.src.w = 128;
		r.src.h = 128;
		r.offset.x = 64;
		r.offset.y = 96; 
		r.object_layer = true;
	}
	else if (effect_type == STAT_EFFECT_VENGEANCE) {
		r.src.x = (vengeance_frame/6) * 64;
		r.src.y = 128;
		r.src.w = 64;
		r.src.h = 64;
		r.offset.x = 32;
		r.offset.y = 32; 
		r.object_layer = false;	
	}
	return r;	
}

StatBlock::~StatBlock() {
}

