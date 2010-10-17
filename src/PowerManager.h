/**
 * class PowerManager
 *
 * Special code for handling spells, special powers, magic item effects, etc.
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include <string>
#include <queue>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "StatBlock.h"
#include "Hazard.h"
#include "MapCollision.h"

#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

using namespace std;

const int POWER_COUNT = 32;

const int POWTYPE_CONSUME = 0;
const int POWTYPE_SINGLE = 1;
const int POWTYPE_MISSILE = 2;
const int POWTYPE_NONDAMAGE = 3;
const int POWTYPE_MISSILE_X3 = 4; // used by MultiShot
const int POWTYPE_GROUNDRAY = 5; // used by Freeze

const int POWSTATE_SWING = 0;
const int POWSTATE_CAST = 1;
const int POWSTATE_SHOOT = 2;
const int POWSTATE_BLOCK = 3;

// first 20 powers coincide with power tree
// TODO: remove this restriction
const int POWER_SHOOT = 0;
const int POWER_SWING = 1;
const int POWER_LORE = 2;
const int POWER_RETURN = 3;
const int POWER_BLOOD = 4;
const int POWER_BLOCK = 5;
const int POWER_SHOCK = 6;
const int POWER_HEAL = 7;
const int POWER_MULTISHOT = 8;
const int POWER_WARCRY = 9;
const int POWER_QUAKE = 10;
const int POWER_SHIELD = 11;
const int POWER_CLEAVE = 12;
const int POWER_CHARGE = 13;
const int POWER_FREEZE = 14;
const int POWER_TELEPORT = 15;
const int POWER_PIERCING = 16;
const int POWER_VENGEANCE = 17;
const int POWER_BURN = 18;
const int POWER_TIMESTOP = 19;
const int POWER_HEALTH_POTION = 20;
const int POWER_MANA_POTION = 21;
const int POWER_HEALTH_FOOD = 22;
const int POWER_MANA_FOOD = 23;
const int POWER_SPARK_BLOOD = 24;
const int POWER_SPARK_FIRE = 25;
const int POWER_SPARK_ICE = 26;


struct Power {
	int type; // what kind of activate() this is
	int icon; // just the number.  The caller menu will have access to the surface.
	int new_state; // when using this power the user (avatar/enemy) starts a new state
	bool face; // does the user turn to face the mouse cursor when using this power?
	string name;
	string description;
	bool requires_ammo;
	bool requires_mana;
	bool requires_los; // line of sight
	bool requires_empty_target; // target square must be empty
	int aim_assist;
	
	Power() {
		type = -1;
		icon = -1;
		name = "";
		description = "";
		new_state = -1;
		face=false;
		requires_ammo = false;
		requires_mana = false;
		requires_los = false;
		requires_empty_target = false;
		aim_assist = 0;
	}	
	
};

class PowerManager {
private:
	
	MapCollision *collider;
	int calcDirection(int origin_x, int origin_y, int target_x, int target_y) ;	
	bool consume(int powernum, StatBlock *src_stats);
	bool nonDamage(int powernum, StatBlock *src_stats, Point target);
	bool missile(int powernum, StatBlock *src_stats, Point target);
	bool single(int powernum, StatBlock *src_stats, Point target);
	bool missileX3(int powernum, StatBlock *src_stats, Point target);
	bool groundRay(int powernum, StatBlock *src_stats, Point target);
	
public:
	PowerManager();
	~PowerManager();
	
	void handleNewMap(MapCollision *_collider);	
	bool activate(int power_index, StatBlock *src_stats, Point target);
	void loadGraphics();
	void loadSounds();
		
	Power powers[POWER_COUNT];
	queue<Hazard *> hazards; // output; read by HazardManager
	
	// shared images/sounds for power special effects
	// TODO: when power definitions move to a config file, change these to an array
	SDL_Surface *arrows;
	SDL_Surface *stone;
	SDL_Surface *lightning;
	SDL_Surface *blast;
	SDL_Surface *quake;
	SDL_Surface *heal;
	SDL_Surface *shield;
	SDL_Surface *sparks;
	SDL_Surface *freeze;
	SDL_Surface *runes;
	
	Mix_Chunk *sfx_warcry;
	Mix_Chunk *sfx_shock;
	Mix_Chunk *sfx_freeze;
	Mix_Chunk *sfx_quake;
	Mix_Chunk *sfx_burn;
	Mix_Chunk *sfx_heal;
	Mix_Chunk *sfx_shield;
	Mix_Chunk *sfx_teleport;
	Mix_Chunk *sfx_timestop;
};

#endif
