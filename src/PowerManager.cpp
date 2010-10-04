/**
 * class PowerManager
 *
 * @author Clint Bellanger
 * @license GPL
 */


#include "PowerManager.h"

/**
 * PowerManager constructor
 */
PowerManager::PowerManager() {
	
	powers[POWER_SHOOT].name = "Shoot";
	powers[POWER_SHOOT].type = POWTYPE_MISSILE;
	powers[POWER_SHOOT].icon = 0;
	powers[POWER_SHOOT].description = "Basic ranged attack";
	powers[POWER_SHOOT].new_state = POWSTATE_SHOOT;
	powers[POWER_SHOOT].face = true;
	powers[POWER_SHOOT].requires_ammo = true;
	powers[POWER_SHOOT].aim_assist = 32;	

	powers[POWER_SWING].name = "Swing";
	powers[POWER_SWING].type = POWTYPE_SINGLE;
	powers[POWER_SWING].icon = 1;
	powers[POWER_SWING].description = "Basic melee attack";
	powers[POWER_SWING].new_state = POWSTATE_SWING;
	powers[POWER_SWING].face = true;
	
	powers[POWER_LORE].name = "Lore";
	powers[POWER_LORE].icon = 2;
	powers[POWER_LORE].description = "Reveal hidden information about an object or area (NYI)";

	powers[POWER_RETURN].name = "Return";
	powers[POWER_RETURN].icon = 3;
	powers[POWER_RETURN].description = "Warp to your chosen Sanctuary (NYI)";

	powers[POWER_BLOOD].name = "Blood Strike";
	powers[POWER_BLOOD].type = POWTYPE_SINGLE;
	powers[POWER_BLOOD].icon = 4;
	powers[POWER_BLOOD].description = "Inflict a bleeding wound that causes additional damage over 3 seconds";
	powers[POWER_BLOOD].new_state = POWSTATE_SWING;
	powers[POWER_BLOOD].face = true;
	powers[POWER_BLOOD].requires_mana = true;

	powers[POWER_BLOCK].name = "Block";
	powers[POWER_BLOCK].icon = 5;
	powers[POWER_BLOCK].description = "Raise your shield and increase your defenses";
	powers[POWER_BLOCK].new_state = POWSTATE_BLOCK;
	powers[POWER_BLOCK].face = true;	

	powers[POWER_SHOCK].name = "Shock";
	powers[POWER_SHOCK].type = POWTYPE_MISSILE;
	powers[POWER_SHOCK].icon = 6;
	powers[POWER_SHOCK].description = "Channel a bolt of magic through your enchanted weapon";
	powers[POWER_SHOCK].new_state = POWSTATE_CAST;
	powers[POWER_SHOCK].face = true;
	powers[POWER_SHOCK].requires_mana = true;
	powers[POWER_SHOCK].aim_assist = 32;	

	powers[POWER_HEAL].name = "Heal";
	powers[POWER_HEAL].type = POWTYPE_NONDAMAGE;
	powers[POWER_HEAL].icon = 7;
	powers[POWER_HEAL].description = "Restore Health";
	powers[POWER_HEAL].new_state = POWSTATE_CAST;
	powers[POWER_HEAL].face = false;
	powers[POWER_HEAL].requires_mana = true;
	
	powers[POWER_MULTISHOT].name = "Multishot";
	powers[POWER_MULTISHOT].type = POWTYPE_MISSILE_X3;
	powers[POWER_MULTISHOT].icon = 8;
	powers[POWER_MULTISHOT].description = "Fire three projectiles";
	powers[POWER_MULTISHOT].new_state = POWSTATE_SHOOT;
	powers[POWER_MULTISHOT].face = true;
	powers[POWER_MULTISHOT].requires_mana = true;
	
	powers[POWER_WARCRY].name = "Warcry";
	powers[POWER_WARCRY].type = POWTYPE_NONDAMAGE;
	powers[POWER_WARCRY].icon = 9;
	powers[POWER_WARCRY].description = "Remove debuffs and become immune to debuffs for 5 seconds";
	powers[POWER_WARCRY].new_state = POWSTATE_CAST;
	powers[POWER_WARCRY].face = false;
	powers[POWER_WARCRY].requires_mana = true;
							
	powers[POWER_QUAKE].name = "Quake";
	powers[POWER_QUAKE].type = POWTYPE_SINGLE;
	powers[POWER_QUAKE].icon = 10;
	powers[POWER_QUAKE].description = "Stun nearby enemies";
	powers[POWER_QUAKE].new_state = POWSTATE_CAST;
	powers[POWER_QUAKE].face = false;
	powers[POWER_QUAKE].requires_mana = true;

	powers[POWER_SHIELD].name = "Shield";
	powers[POWER_SHIELD].type = POWTYPE_NONDAMAGE;
	powers[POWER_SHIELD].icon = 11;
	powers[POWER_SHIELD].description = "Create a force field that absorbs damage";
	powers[POWER_SHIELD].new_state = POWSTATE_CAST;
	powers[POWER_SHIELD].face = false;
	powers[POWER_SHIELD].requires_mana = true;

	powers[POWER_CLEAVE].name = "Cleave";
	powers[POWER_CLEAVE].type = POWTYPE_SINGLE;
	powers[POWER_CLEAVE].icon = 12;
	powers[POWER_CLEAVE].description = "Attack all enemies in a wide arc";
	powers[POWER_CLEAVE].new_state = POWSTATE_SWING;
	powers[POWER_CLEAVE].face = true;
	powers[POWER_CLEAVE].requires_mana = true;
	
	powers[POWER_CHARGE].name = "Charge";
	powers[POWER_CHARGE].icon = 13;
	powers[POWER_CHARGE].description = "Bull rush and stun an enemy (NYI)";
	
	powers[POWER_FREEZE].name = "Freeze";
	powers[POWER_FREEZE].type = POWTYPE_GROUNDRAY;
	powers[POWER_FREEZE].icon = 14;
	powers[POWER_FREEZE].description = "Create a ray of piercing cold that slows enemies";
	powers[POWER_FREEZE].new_state = POWSTATE_CAST;
	powers[POWER_FREEZE].face = true;
	powers[POWER_FREEZE].requires_mana = true;

	powers[POWER_TELEPORT].name = "Teleport";
	powers[POWER_TELEPORT].type = POWTYPE_NONDAMAGE;
	powers[POWER_TELEPORT].icon = 15;
	powers[POWER_TELEPORT].description = "Blink to a nearby location";
	powers[POWER_TELEPORT].new_state = POWSTATE_CAST;
	powers[POWER_TELEPORT].face = true;
	powers[POWER_TELEPORT].requires_mana = true;
	
	powers[POWER_PIERCING].name = "Piercing Shot";
	powers[POWER_PIERCING].type = POWTYPE_MISSILE;	
	powers[POWER_PIERCING].icon = 16;
	powers[POWER_PIERCING].description = "Fire a shot that ignores armor and goes through multiple enemies";
	powers[POWER_PIERCING].new_state = POWSTATE_SHOOT;
	powers[POWER_PIERCING].face = true;
	powers[POWER_PIERCING].requires_mana = true;
	
	powers[POWER_VENGEANCE].name = "Vengeance";
	powers[POWER_VENGEANCE].type = POWTYPE_SINGLE;
	powers[POWER_VENGEANCE].icon = 17;
	powers[POWER_VENGEANCE].description = "After blocking, unlease a deadly and accurate counter-attack";
	powers[POWER_VENGEANCE].new_state = POWSTATE_SWING;
	powers[POWER_VENGEANCE].face = true;
	powers[POWER_VENGEANCE].requires_mana = true;
	
	powers[POWER_BURN].name = "Burn";
	powers[POWER_BURN].type = POWTYPE_SINGLE;
	powers[POWER_BURN].icon = 18;
	powers[POWER_BURN].description = "Blast enemies in a distant area with fire";
	powers[POWER_BURN].new_state = POWSTATE_CAST;
	powers[POWER_BURN].face = true;
	powers[POWER_BURN].requires_mana = true;

	powers[POWER_TIMESTOP].name = "Time Stop";
	powers[POWER_TIMESTOP].type = POWTYPE_NONDAMAGE;
	powers[POWER_TIMESTOP].icon = 19;
	powers[POWER_TIMESTOP].description = "Stop time for 5 seconds";
	powers[POWER_TIMESTOP].new_state = POWSTATE_CAST;
	powers[POWER_TIMESTOP].face = false;
	powers[POWER_TIMESTOP].requires_mana = true;

	powers[POWER_SPARK_BLOOD].type = POWTYPE_NONDAMAGE;
	powers[POWER_SPARK_FIRE].type = POWTYPE_NONDAMAGE;
	powers[POWER_SPARK_ICE].type = POWTYPE_NONDAMAGE;
	
	loadGraphics();
	loadSounds();
}

void PowerManager::loadGraphics() {
	
	arrows = IMG_Load("images/powers/arrows.png");
	stone = IMG_Load("images/powers/stone.png");
	lightning = IMG_Load("images/powers/lightning.png");
	blast = IMG_Load("images/powers/blast.png");
	quake = IMG_Load("images/powers/quake.png");
	heal = IMG_Load("images/powers/heal.png");
	shield = IMG_Load("images/powers/shield.png");
	sparks = IMG_Load("images/powers/sparks.png");
	freeze = IMG_Load("images/powers/freeze.png");
	runes = IMG_Load("images/powers/runes.png");
	
	if(!arrows || !stone || !lightning || !blast || !quake || !heal || !shield || !sparks || !freeze || !runes) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
}

void PowerManager::loadSounds() {
	sfx_shock = Mix_LoadWAV("soundfx/powers/shock.ogg");
	sfx_freeze = Mix_LoadWAV("soundfx/powers/freeze.ogg");	
	sfx_burn = Mix_LoadWAV("soundfx/powers/burn.ogg");
	sfx_heal = Mix_LoadWAV("soundfx/powers/heal.ogg");
	sfx_teleport = Mix_LoadWAV("soundfx/powers/teleport.ogg");	
	sfx_timestop = Mix_LoadWAV("soundfx/powers/timestop.ogg");	
}

/**
 * Change direction to face the target map location
 */
int PowerManager::calcDirection(int origin_x, int origin_y, int target_x, int target_y) {

	// inverting Y to convert map coordinates to standard cartesian coordinates
	int dx = target_x - origin_x;
	int dy = origin_y - target_y;

	// avoid div by zero
	if (dx == 0) {
		if (dy > 0) return 3;
		else return 7;
	}
	
	float slope = ((float)dy)/((float)dx);
	if (0.5 <= slope && slope <= 2.0) {
		if (dy > 0) return 4;
		else return 0;
	}
	if (-0.5 <= slope && slope <= 0.5) {
		if (dx > 0) return 5;
		else return 1;
	}
	if (-2.0 <= slope && slope <= -0.5) {
		if (dx > 0) return 6;
		else return 2;
	}
	if (2 <= slope || -2 >= slope) {
		if (dy > 0) return 3;
		else return 7;
	}
	return 0;
}



/**
 * Basic single-use items
 */
bool PowerManager::consume(int power_index, StatBlock *src_stats) {
	return false;
}

/**
 * Hazards that are graphics/sounds only
 */
bool PowerManager::nonDamage(int power_index, StatBlock *src_stats, Point target) {
	Hazard *haz = new Hazard();
	haz->pos.x = src_stats->pos.x;
	haz->pos.y = src_stats->pos.y;	
	if (src_stats->hero)
		haz->source = SRC_HERO;
	else
		haz->source = SRC_ENEMY;
	haz->rendered = true;
	haz->active = false;
	
	if (power_index == POWER_HEAL) {
		haz->direction = 0;
		haz->lifespan = 18;
		haz->frame_duration = 3;
		haz->frame_loop = -1;
		haz->frame_offset.y = 64;
		src_stats->mp--;
		haz->sprites = heal;
		
		// perform actual healing
		src_stats->hp += rand() % (src_stats->dmg_magic_max - src_stats->dmg_magic_min) + src_stats->dmg_magic_min;
		if (src_stats->hp > src_stats->maxhp) src_stats->hp = src_stats->maxhp;
		Mix_PlayChannel(-1, sfx_heal, 0);
	}
	else if (power_index == POWER_TIMESTOP) {
		haz->active = true;
		haz->dmg_min = haz->dmg_max = 0;
		haz->accuracy = src_stats->accuracy;
		haz->rendered = false;
		haz->lifespan = 1;
		haz->radius = 512;
		haz->stun_duration = 150;
		haz->multitarget = true;
		src_stats->mp--;
		Mix_PlayChannel(-1, sfx_timestop, 0);
	}
	else if (power_index == POWER_SHIELD) {
		src_stats->mp--;
		src_stats->shield_hp = src_stats->dmg_magic_max;
		delete(haz); // no hazard needed
		return true;
	}
	else if (power_index == POWER_WARCRY) {
		src_stats->mp--;
		
		// remove all debuffs
		src_stats->slow_duration = 0;
		src_stats->immobilize_duration = 0;
		src_stats->stun_duration = 0;
		src_stats->bleed_duration = 0;
		
		delete(haz); // no hazard needed
		return true;
	}
	else if (power_index == POWER_SPARK_BLOOD) {
		haz->lifespan = 24;
		haz->frame_duration = 6;
		haz->frame_loop = 100;
		haz->frame_offset.y = 64;
		haz->direction = rand() % 2;
		haz->sprites = sparks;
	}
	else if (power_index == POWER_SPARK_FIRE) {
		haz->lifespan = 24;
		haz->frame_duration = 6;
		haz->frame_loop = 100;
		haz->frame_offset.y = 64;
		haz->direction = rand() % 2 + 2;
		haz->sprites = sparks;
	}
	else if (power_index == POWER_SPARK_ICE) {
		haz->lifespan = 24;
		haz->frame_duration = 6;
		haz->frame_loop = 100;
		haz->frame_offset.y = 64;
		haz->direction = rand() % 2 + 4;
		haz->sprites = sparks;
	}
	else if (power_index == POWER_TELEPORT) {
		haz->direction = 0;
		haz->lifespan = 16;
		haz->frame_duration = 4;
		haz->frame_loop = -1;
		haz->floor = true;
		haz->sprites = runes;
		src_stats->teleportation = true;
		src_stats->teleport_destination.x = target.x;
		src_stats->teleport_destination.y = target.y;
		src_stats->mp--;
		Mix_PlayChannel(-1,sfx_teleport,0);
	}
	
	hazards.push(haz);

	// Hazard memory is now the responsibility of HazardManager
	return true;			
}


/**
 * Basic projectile hazard
 * Assumes projectile starts at stats.pos and moves towards target
 */
bool PowerManager::missile(int power_index, StatBlock *src_stats, Point target) {

	Hazard *haz = new Hazard();

	// common to all missiles
	haz->pos.x = src_stats->pos.x;
	haz->pos.y = src_stats->pos.y;
	haz->crit_chance = src_stats->crit;
	haz->accuracy = src_stats->accuracy;
	if (src_stats->hero)
		haz->source = SRC_HERO;
	else
		haz->source = SRC_ENEMY;
	haz->rendered = true;
	haz->frame_offset.y = 64;
		
	// specific powers have different stats here
	int speed;
	if (power_index == POWER_SHOOT || power_index == POWER_PIERCING) {
		haz->lifespan = 8;
		haz->radius = 96;
		haz->dmg_min = src_stats->dmg_ranged_min;
		haz->dmg_max = src_stats->dmg_ranged_max;
		speed = 48;
		if (src_stats->ammo_stones) {
			haz->sprites = stone;
			haz->direction = 0;
		}
		else {
			haz->sprites = arrows;
			haz->direction = calcDirection(src_stats->pos.x, src_stats->pos.y, target.x, target.y);
		}
		
		if (power_index == POWER_PIERCING) {
			haz->multitarget = true;
			haz->trait_armor_penetration = true;
			src_stats->mp--;
		}
	}
	else if (power_index == POWER_SHOCK) {
		haz->direction = calcDirection(src_stats->pos.x, src_stats->pos.y, target.x, target.y);\
		haz->lifespan = 10;
		haz->radius = 96;
		haz->dmg_min = src_stats->dmg_magic_min;
		haz->dmg_max = src_stats->dmg_magic_max;
		haz->frame_loop = 4;
		haz->frame_duration = 1;
		haz->sprites = lightning;
		haz->trait_air = true;
		src_stats->mp--;
		speed = 32;
		Mix_PlayChannel(-1,sfx_shock,0);
	}
	
	// calculate speed
	float dx = target.x - src_stats->pos.x;
	float dy = target.y - src_stats->pos.y;
	float theta = atan(dy/dx);
	haz->speed.x = speed * cos(theta);
	haz->speed.y = speed * sin(theta);
	if (dx > 0 && haz->speed.x < 0 || dx < 0 && haz->speed.x > 0)
		haz->speed.x *= -1;
	if (dy > 0 && haz->speed.y < 0 || dy < 0 && haz->speed.y > 0)
		haz->speed.y *= -1;
	
	hazards.push(haz);

	// Hazard memory is now the responsibility of HazardManager
	return true;
}

/**
 * Triple projectile attack
 */
bool PowerManager::missileX3(int power_index, StatBlock *src_stats, Point target) {

	Hazard *haz[3];
	int speed;
	double angle = 0.2;

	for (int i=0; i<3; i++) {
		haz[i] = new Hazard();
	
		haz[i]->pos.x = src_stats->pos.x;
		haz[i]->pos.y = src_stats->pos.y;
		haz[i]->crit_chance = src_stats->crit;
		haz[i]->accuracy = src_stats->accuracy;
		if (src_stats->hero)
			haz[i]->source = SRC_HERO;
		else
			haz[i]->source = SRC_ENEMY;
		haz[i]->rendered = true;
		haz[i]->frame_offset.y = 64;
		
		// specific powers have different stats here
		if (power_index == POWER_MULTISHOT) {
			haz[i]->lifespan = 8;
			haz[i]->radius = 96;
			haz[i]->dmg_min = src_stats->dmg_ranged_min;
			haz[i]->dmg_max = src_stats->dmg_ranged_max;
			speed = 48;
			if (src_stats->ammo_stones) {
				haz[i]->sprites = stone;
				haz[i]->direction = 0;
			}
			else {
				haz[i]->sprites = arrows;
				haz[i]->direction = calcDirection(src_stats->pos.x, src_stats->pos.y, target.x, target.y);
			}
		}
	}
	
	src_stats->mp--;
	
	// calculate speeds
	float dx = target.x - src_stats->pos.x;
	float dy = target.y - src_stats->pos.y;
	float theta = atan(dy/dx);
	
	// middle missile
	haz[0]->speed.x = speed * cos(theta);
	haz[0]->speed.y = speed * sin(theta);
	if (dx > 0 && haz[0]->speed.x < 0 || dx < 0 && haz[0]->speed.x > 0)
		haz[0]->speed.x *= -1;
	if (dy > 0 && haz[0]->speed.y < 0 || dy < 0 && haz[0]->speed.y > 0)
		haz[0]->speed.y *= -1;
	hazards.push(haz[0]);
	
	// side missile
	haz[1]->speed.x = speed * cos(theta + angle);
	haz[1]->speed.y = speed * sin(theta + angle);
	if (dx > 0 && haz[1]->speed.x < 0 || dx < 0 && haz[1]->speed.x > 0)
		haz[1]->speed.x *= -1;
	if (dy > 0 && haz[1]->speed.y < 0 || dy < 0 && haz[1]->speed.y > 0)
		haz[1]->speed.y *= -1;
	hazards.push(haz[1]);	

	// side missile
	haz[2]->speed.x = speed * cos(theta - angle);
	haz[2]->speed.y = speed * sin(theta - angle);
	if (dx > 0 && haz[2]->speed.x < 0 || dx < 0 && haz[2]->speed.x > 0)
		haz[2]->speed.x *= -1;
	if (dy > 0 && haz[2]->speed.y < 0 || dy < 0 && haz[2]->speed.y > 0)
		haz[2]->speed.y *= -1;
	hazards.push(haz[2]);	
	
	// Hazard memory is now the responsibility of HazardManager
	return true;
}





/**
 * Ground Rays are multiple hazards that spawn in a straight line
 */
bool PowerManager::groundRay(int power_index, StatBlock *src_stats, Point target) {

	src_stats->mp--;

	Hazard *haz[6];
	FPoint location_iterator;
	FPoint speed;
	int delay_iterator;
	int map_speed = 64;

	// calculate speed
	float dx = target.x - src_stats->pos.x;
	float dy = target.y - src_stats->pos.y;
	float theta = atan(dy/dx);
	speed.x = map_speed * cos(theta);
	speed.y = map_speed * sin(theta);
	if (dx > 0 && speed.x < 0 || dx < 0 && speed.x > 0)
		speed.x *= -1;
	if (dy > 0 && speed.y < 0 || dy < 0 && speed.y > 0)
		speed.y *= -1;

	location_iterator.x = (float)src_stats->pos.x;
	location_iterator.y = (float)src_stats->pos.y;
	delay_iterator = 0;

	for (int i=0; i<6; i++) {
		haz[i] = new Hazard();
	
		location_iterator.x += speed.x;
		location_iterator.y += speed.y;
	
		haz[i]->pos.x = location_iterator.x;
		haz[i]->pos.y = location_iterator.y;
		haz[i]->delay_frames = delay_iterator;
		delay_iterator += 3;
		
		haz[i]->crit_chance = src_stats->crit;
		haz[i]->accuracy = src_stats->accuracy;
		if (src_stats->hero)
			haz[i]->source = SRC_HERO;
		else
			haz[i]->source = SRC_ENEMY;
		haz[i]->rendered = true;
		haz[i]->frame_offset.y = 64;
		
		// specific powers have different stats here
		if (power_index == POWER_FREEZE) {
			haz[i]->lifespan = 20;
			haz[i]->active_frame = 0;
			haz[i]->frame = 16; // start at bottom frame
			haz[i]->frame_loop = 20;
			haz[i]->frame_duration = 4;
			haz[i]->radius = 64;
			haz[i]->dmg_min = src_stats->dmg_magic_min;
			haz[i]->dmg_max = src_stats->dmg_magic_max;
			haz[i]->sprites = freeze;
			haz[i]->direction = rand() % 3;
			haz[i]->complete_animation = true;
			haz[i]->slow_duration = 90;
			haz[i]->trait_ice = true;
			Mix_PlayChannel(-1, sfx_freeze, 0);
		}
		
		hazards.push(haz[i]);
	}
	
	// Hazard memory is now the responsibility of HazardManager
	return true;
}





/**
 * Basic single-frame area hazard
 */
bool PowerManager::single(int power_index, StatBlock *src_stats, Point target) {
	
	Hazard *haz = new Hazard();
	
	// common to all singles
	haz->pos.x = target.x;
	haz->pos.y = target.y;
	haz->lifespan = 1;
	haz->crit_chance = src_stats->crit;
	haz->accuracy = src_stats->accuracy;
	if (src_stats->hero)
		haz->source = SRC_HERO;
	else
		haz->source = SRC_ENEMY;

	// specific powers have different stats here
	if (power_index == POWER_SWING) {
		haz->dmg_min = src_stats->dmg_melee_min;
		haz->dmg_max = src_stats->dmg_melee_max;
		haz->radius = 64;
	}
	if (power_index == POWER_BLOOD) {
		haz->dmg_min = src_stats->dmg_melee_min;
		haz->dmg_max = src_stats->dmg_melee_max;
		haz->bleed_duration = 90;
		haz->radius = 64;
		src_stats->mp--;
	}
	if (power_index == POWER_VENGEANCE) {
		haz->dmg_min = src_stats->dmg_melee_min;
		haz->dmg_max = src_stats->dmg_melee_max;
		haz->radius = 64;
		src_stats->mp--;
		
		// use vengeance stacks
		haz->accuracy += src_stats->vengeance_stacks * 25;
		haz->crit_chance += src_stats->vengeance_stacks * 25;
		src_stats->vengeance_stacks = 0;
	}
	else if (power_index == POWER_CLEAVE) {
		haz->dmg_min = src_stats->dmg_melee_min;
		haz->dmg_max = src_stats->dmg_melee_max;
		haz->radius = 128;
		haz->multitarget = true;
		src_stats->mp--;
	}
	else if (power_index == POWER_BURN) {
		haz->rendered = true;
		haz->dmg_min = src_stats->dmg_magic_min;
		haz->dmg_max = src_stats->dmg_magic_max;
		haz->direction = 0;
		haz->multitarget = true;
		haz->radius = 128;
		haz->active_frame = 4;
		haz->lifespan = 18;
		haz->frame_duration = 3;
		haz->frame_loop = -1;
		haz->sprites = blast;
		haz->frame_size.x = 256;
		haz->frame_size.y = 128;
		haz->frame_offset.x = 128;
		haz->frame_offset.y = 64;
		haz->floor = true;
		haz->trait_fire = true;
		src_stats->mp--;
		Mix_PlayChannel(-1, sfx_burn, 0);
	}
	else if (power_index == POWER_QUAKE) {
		haz->pos.x = src_stats->pos.x;
		haz->pos.y = src_stats->pos.y;
		haz->rendered = true;
		haz->dmg_min = src_stats->dmg_magic_min;
		haz->dmg_max = src_stats->dmg_magic_max;
		haz->direction = 0;
		haz->multitarget = true;
		haz->radius = 128;
		haz->active_frame = 4;
		haz->lifespan = 12;
		haz->frame_duration = 2;
		haz->frame_loop = 32;
		haz->sprites = quake;
		haz->frame_size.x = 256;
		haz->frame_size.y = 128;
		haz->frame_offset.x = 128;
		haz->frame_offset.y = 64;
		haz->floor = true;
		haz->stun_duration = 30;
		haz->trait_earth = true;
		src_stats->mp--;
	}
	
	hazards.push(haz);

	// Hazard memory is now the responsibility of HazardManager
	return true;
}


/**
 * Activate is basically a switch/redirect to the appropriate function
 */
bool PowerManager::activate(int power_index, StatBlock *src_stats, Point target) {
	
	if (powers[power_index].type == POWTYPE_SINGLE)
		return single(power_index, src_stats, target);
	else if (powers[power_index].type == POWTYPE_NONDAMAGE)
		return nonDamage(power_index, src_stats, target);
	else if (powers[power_index].type == POWTYPE_CONSUME)
		return consume(power_index, src_stats);
	else if (powers[power_index].type == POWTYPE_MISSILE)
		return missile(power_index, src_stats, target);
	else if (powers[power_index].type == POWTYPE_MISSILE_X3)
		return missileX3(power_index, src_stats, target);
	else if (powers[power_index].type == POWTYPE_GROUNDRAY)
		return groundRay(power_index, src_stats, target);
	
	return false;
}

PowerManager::~PowerManager() {
	SDL_FreeSurface(arrows);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(lightning);
	SDL_FreeSurface(blast);
	SDL_FreeSurface(quake);	
	SDL_FreeSurface(heal);
	SDL_FreeSurface(shield);
	SDL_FreeSurface(sparks);
	SDL_FreeSurface(freeze);
	SDL_FreeSurface(runes);
	Mix_FreeChunk(sfx_shock);
	Mix_FreeChunk(sfx_freeze);
	Mix_FreeChunk(sfx_burn);
	Mix_FreeChunk(sfx_heal);
	Mix_FreeChunk(sfx_teleport);
	Mix_FreeChunk(sfx_timestop);
	
}

