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

#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

using namespace std;

const int POWER_COUNT = 32;

const int POWTYPE_CONSUME = 0;
const int POWTYPE_SINGLE = 1;
const int POWTYPE_MISSILE = 2;

const int POWSTATE_SWING = 0;
const int POWSTATE_CAST = 1;
const int POWSTATE_SHOOT = 2;

const int POWER_SHOOT = 0;
const int POWER_SWING = 1;
const int POWER_SHOCK = 6;
const int POWER_QUAKE = 10;
const int POWER_BURN = 18;

struct Power {
	int type; // what kind of activate() this is
	int icon; // just the number.  The caller menu will have access to the surface.
	int new_state; // when using this power the user (avatar/enemy) starts a new state
	bool face; // does the user turn to face the mouse cursor when using this power?
	string name;
	string description;
	bool requires_ammo;
	bool requires_mana;
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
		aim_assist = 0;
	}	
	
};

class PowerManager {
private:
	
	int calcDirection(int origin_x, int origin_y, int target_x, int target_y) ;	
	bool consume(int powernum, StatBlock *src_stats);
	bool missile(int powernum, StatBlock *src_stats, Point target);
	bool single(int powernum, StatBlock *src_stats, Point target);
	
public:
	PowerManager();
	~PowerManager();
	bool activate(int power_index, StatBlock *src_stats, Point target);
	void loadGraphics();
		
	Power powers[POWER_COUNT];
	queue<Hazard *> hazards; // output; read by HazardManager
	
	// shared images/sounds for power special effects
	SDL_Surface *arrows;
	SDL_Surface *stone;
	SDL_Surface *lightning;
	SDL_Surface *blast;
	SDL_Surface *quake;
	
};

#endif
