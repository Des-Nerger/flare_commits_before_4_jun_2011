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

	powers[POWER_SHOCK].name = "Shock";
	powers[POWER_SHOCK].type = POWTYPE_MISSILE;
	powers[POWER_SHOCK].icon = 6;
	powers[POWER_SHOCK].description = "Shoot a bolt of electricity";
	powers[POWER_SHOCK].new_state = POWSTATE_CAST;
	powers[POWER_SHOCK].face = true;
	powers[POWER_SHOCK].requires_mana = true;
	powers[POWER_SHOCK].aim_assist = 32;	

	powers[POWER_BURN].name = "Burn";
	powers[POWER_BURN].type = POWTYPE_SINGLE;
	powers[POWER_BURN].icon = 18;
	powers[POWER_BURN].description = "Blast enemies in a distant area with fire";
	powers[POWER_BURN].new_state = POWSTATE_CAST;
	powers[POWER_BURN].face = true;
	powers[POWER_BURN].requires_mana = true;
	
	powers[POWER_QUAKE].name = "Quake";
	powers[POWER_QUAKE].type = POWTYPE_SINGLE;
	powers[POWER_QUAKE].icon = 10;
	powers[POWER_QUAKE].description = "Stun nearby enemies";
	powers[POWER_QUAKE].new_state = POWSTATE_CAST;
	powers[POWER_QUAKE].face = false;
	powers[POWER_QUAKE].requires_mana = true;

	powers[POWER_HEAL].name = "Heal";
	powers[POWER_HEAL].type = POWTYPE_NONDAMAGE;
	powers[POWER_HEAL].icon = 7;
	powers[POWER_HEAL].description = "Restore Health";
	powers[POWER_HEAL].new_state = POWSTATE_CAST;
	powers[POWER_HEAL].face = false;
	powers[POWER_HEAL].requires_mana = true;

	powers[POWER_TIMESTOP].name = "Time Stop";
	powers[POWER_TIMESTOP].type = POWTYPE_NONDAMAGE;
	powers[POWER_TIMESTOP].icon = 19;
	powers[POWER_TIMESTOP].description = "Stop time for 3 seconds";
	powers[POWER_TIMESTOP].new_state = POWSTATE_CAST;
	powers[POWER_TIMESTOP].face = false;
	powers[POWER_TIMESTOP].requires_mana = true;
			
	loadGraphics();
}

void PowerManager::loadGraphics() {
	
	arrows = IMG_Load("images/powers/arrows.png");
	stone = IMG_Load("images/powers/stone.png");
	lightning = IMG_Load("images/powers/lightning.png");
	blast = IMG_Load("images/powers/blast.png");
	quake = IMG_Load("images/powers/quake.png");
	heal = IMG_Load("images/powers/heal.png");		
	
	if(!arrows || !stone || !lightning || !blast || !quake || !heal) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
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
	}
	else if (power_index == POWER_TIMESTOP) {
		haz->active = true;
		haz->dmg_min = haz->dmg_max = 0;
		haz->accuracy = src_stats->accuracy;
		haz->rendered = false;
		haz->lifespan = 1;
		haz->radius = 512;
		haz->stun_duration = 90;
		haz->multitarget = true;
		src_stats->mp--;
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
	if (power_index == POWER_SHOOT) {
		haz->lifespan = 7;
		haz->radius = 96;
		haz->dmg_min = src_stats->dmg_ranged_min;
		haz->dmg_max = src_stats->dmg_ranged_max;
		speed = 64;
		if (src_stats->ammo_stones) {
			haz->sprites = stone;
			haz->direction = 0;
		}
		else {
			haz->sprites = arrows;
			haz->direction = calcDirection(src_stats->pos.x, src_stats->pos.y, target.x, target.y);
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
		src_stats->mp--;
		speed = 32;
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
	else if (power_index == POWER_BURN) {
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
		haz->sprites = blast;
		haz->frame_size.x = 256;
		haz->frame_size.y = 128;
		haz->frame_offset.x = 128;
		haz->frame_offset.y = 64;
		haz->floor = true;
		src_stats->mp--;
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
	else
		return false;
}

PowerManager::~PowerManager() {
	SDL_FreeSurface(arrows);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(lightning);
	SDL_FreeSurface(blast);
	SDL_FreeSurface(quake);	
	SDL_FreeSurface(heal);
}

