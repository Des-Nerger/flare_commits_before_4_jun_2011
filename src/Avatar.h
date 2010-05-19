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

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "InputState.h"
#include "MapIso.h"
#include "StatBlock.h"
#include "Hazard.h"

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
	void loadGraphics();
	void loadSounds();
	
	SDL_Surface *sprites;
	InputState *inp;
	MapIso *map;
	int curState;
	int direction;
	bool lockSwing;
	bool lockCast;
	bool lockShoot;
	int curFrame;
	int dispFrame;
	bool animFwd;
	int speed;
	int dspeed;
	int cooldown_melee;
	int aim_height;
	
	Mix_Chunk *sound_weapon1;
	Mix_Chunk *sound_hit;
	Mix_Chunk *sound_die;
	Mix_Chunk *sound_steps;
			
public:
	Avatar(InputState *_inp, MapIso *_map);
	~Avatar();
	void logic();
	bool pressing_move();	
	bool move();
	void set_direction();
	int face(int mapx, int mapy);
	Renderable getRender();
	void takeHit();
	
	// vars
	Point pos;
	StatBlock stats;
	Hazard *haz;

};

#endif
