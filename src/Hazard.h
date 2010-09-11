/**
 * class Hazard
 *
 * Stand-alone object that can harm the hero or creatures
 * These are generated whenever something makes any attack
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef HAZARD_H
#define HAZARD_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "MapCollision.h"

// Hazard Sources
const int SRC_HERO = 0;
const int SRC_ENEMY = 1;
const int SRC_NEUTRAL = 2;

class Hazard {
private:
	MapCollision *collider;
public:
	Hazard();
	
	SDL_Surface *sprites;
	void setCollision(MapCollision *_collider);
	void logic();

	int source;
	int enemyIndex;
	
	int dmg_min;
	int dmg_max;
	int crit_chance;
	int accuracy;
	
	FPoint pos;
	FPoint speed;
	int lifespan; // ticks down to zero
	int radius;
	int frame;
	int frame_loop;
	int frame_duration;
	int active_frame; // some hazards are only dangerous on a single frame of their existence
	int power_index;
	int direction; // helps when choosing animations
	bool rendered;
	
	// these work in conjunction
	// if the attack is not multitarget, set active=false
	// only process active hazards for collision
	bool multitarget;
	bool active;
};

#endif
