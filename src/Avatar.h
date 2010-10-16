/**
 * class Avatar
 *
 * Contains logic and rendering routines for the player avatar.
 *
 * @author Clint Bellanger
 * @license GPL
 */
#ifndef AVATAR_H
#define AVATAR_H

#include <sstream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "InputState.h"
#include "MapIso.h"
#include "StatBlock.h"
#include "Hazard.h"
#include "PowerManager.h"

// AVATAR State enum
const int AVATAR_STANCE = 0;
const int AVATAR_RUN = 1;
const int AVATAR_MELEE = 2;
const int AVATAR_BLOCK = 3;
const int AVATAR_HIT = 4;
const int AVATAR_DEAD = 5;
const int AVATAR_CAST = 6;
const int AVATAR_SHOOT = 7;

class Avatar {
private:
	
	PowerManager *powers;
	SDL_Surface *sprites;
	InputState *inp;
	
	MapIso *map;
	int curState;
	bool lockSwing;
	bool lockCast;
	bool lockShoot;
	int curFrame;
	int dispFrame;
	bool animFwd;
	int cooldown_power;
	
	Mix_Chunk *sound_weapon1;
	Mix_Chunk *sound_hit;
	Mix_Chunk *sound_die;
	Mix_Chunk *sound_block;
	Mix_Chunk *sound_steps[4];
	Mix_Chunk *level_up;
			
public:
	Avatar(PowerManager *_powers, InputState *_inp, MapIso *_map);
	~Avatar();
	void loadGraphics(string img_main, string img_body, string img_off);
	void loadSounds();
	
	void logic(int actionbar_power);
	bool pressing_move();	
	bool move();
	void set_direction();
	int face(int mapx, int mapy);
	Renderable getRender();
	bool takeHit(Hazard h);
	string log_msg;
	
	// vars
	StatBlock stats;
	Hazard *haz;
	int current_power;
	Point act_target;

};

#endif
