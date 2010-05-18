/*
 * class Enemy
 *
 * @author Clint Bellanger
 * @license GPL
 *
 */

#ifndef ENEMY_H
#define ENEMY_H

#include <math.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "InputState.h"
#include "MapIso.h"
#include "StatBlock.h"
#include "Hazard.h"
#include "Settings.h"

// active states
const int ENEMY_STANCE = 0;
const int ENEMY_MOVE = 1;
const int ENEMY_CHARGE = 2;
const int ENEMY_MELEE_PHYS = 3;
const int ENEMY_MELEE_MAG = 4;
const int ENEMY_RANGED_PHYS = 5;
const int ENEMY_RANGED_MAG = 6;

// interrupt states
const int ENEMY_BLOCK = 7;
const int ENEMY_HIT = 8;
const int ENEMY_DEAD = 9;
const int ENEMY_CRITDEAD = 10;

class Enemy {
private:
	MapIso *map;
	
	int curState;
	int curFrame;
	int dispFrame;
	int speed;
	int dspeed;

	int dirFavor;
	int dirTicks;
	int patrolTicks;
	
	bool in_combat;
	
	// behaviors
	int chance_melee_phys;
	int chance_melee_mag;
	int chance_ranged_phys;
	int chance_ranged_mag;
	
	int cooldown;
	
public:
	Enemy(MapIso *_map);
	~Enemy();
	bool lineOfSight();
	bool move();
	void logic();
	int face(int mapx, int mapy);
	int faceNextBest(int mapx, int mapy);
	void newState(int state);
	int getDistance(Point dest);
	void takeHit();
	
	Renderable getRender();
	Point heroPos;
	Point lastSeen; // hero
	Point pos;
	int direction;
	
	int meleeRange;
	int threatRange;
	Hazard *haz;
	StatBlock stats;

	bool sfx_attack;
	bool sfx_hit;
	bool sfx_die;
	bool sfx_critdie;
};


#endif
