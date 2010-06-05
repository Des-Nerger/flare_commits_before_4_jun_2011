/**
 * class Avatar
 *
 * Contains logic and rendering routines for the player avatar.
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "Avatar.h"

Avatar::Avatar(InputState *_inp, MapIso *_map) {
	inp = _inp;
	map = _map;
	
	// other init
	curState = AVATAR_STANCE;
	pos.x = map->spawn.x;
	pos.y = map->spawn.y;
	direction = map->spawn_dir;
		
	curFrame = 1;
	dispFrame = 0;
	lockSwing = false;
	lockCast = false;
	lockShoot = false;
	
	speed = 9;
	dspeed = 6; // try to keep this number even; probably a rounding error causing jittering
	
	stats.name = "Unknown";
	stats.level = 17;
	stats.physical = 5;
	stats.magical = 5;
	stats.offense = 5;
	stats.defense = 5;
	stats.dmg_melee_min = 1;
	stats.dmg_melee_max = 4;
	stats.dmg_magic_min = 0;
	stats.dmg_magic_max = 0;
	stats.dmg_ranged_min = 0;
	stats.dmg_ranged_max = 0;
	stats.recalc();
	
	stats.hp_per_minute = 4 + stats.physical;
	stats.mp_per_minute = 4 + stats.magical;
	cooldown_melee = 0;
	
	// when aiming attacks (not ground spells) adjust for players aiming at
	// the creature center of mass instead of at the floor
	aim_height = 0;
	
	haz = NULL;
	
	//loadGraphics("shortsword","clothes","buckler");
	loadSounds();
}

void Avatar::loadGraphics(string img_main, string img_body, string img_off) {
	SDL_Surface *gfx_main = NULL;
	SDL_Surface *gfx_off = NULL;
	SDL_Rect src;
	SDL_Rect dest;
	
	string img_armor = img_body;
	if (img_armor == "") img_armor = "clothes";
	
	// composite the hero graphic
	sprites = IMG_Load(("images/avatar/male/" + img_armor + ".png").c_str());
	if (img_main != "") gfx_main = IMG_Load(("images/avatar/male/" + img_main + ".png").c_str());
	if (img_off != "") gfx_off = IMG_Load(("images/avatar/male/" + img_off + ".png").c_str());

	SDL_SetColorKey( sprites, SDL_SRCCOLORKEY, SDL_MapRGB(sprites->format, 255, 0, 255) ); 
	if (gfx_main) SDL_SetColorKey( gfx_main, SDL_SRCCOLORKEY, SDL_MapRGB(gfx_main->format, 255, 0, 255) ); 
	if (gfx_off) SDL_SetColorKey( gfx_off, SDL_SRCCOLORKEY, SDL_MapRGB(gfx_off->format, 255, 0, 255) ); 
	
	// assuming the hero is right-handed, we know the layer z-order
	src.w = dest.w = 4096;
	src.h = dest.h = 256;
	src.x = dest.x = 0;
	src.y = dest.y = 0;
	if (gfx_main) SDL_BlitSurface(gfx_main, &src, sprites, &dest);
	src.y = dest.y = 768;
	if (gfx_main) SDL_BlitSurface(gfx_main, &src, sprites, &dest);
	src.h = dest.h = 1024;
	src.y = dest.y = 0;
	if (gfx_off) SDL_BlitSurface(gfx_off, &src, sprites, &dest);
	src.h = dest.h = 512;
	src.y = dest.y = 256;
	if (gfx_main) SDL_BlitSurface(gfx_main, &src, sprites, &dest);
	
	if (gfx_main) SDL_FreeSurface(gfx_main);
	if (gfx_off) SDL_FreeSurface(gfx_off);
}

void Avatar::loadSounds() {
	sound_weapon1 = Mix_LoadWAV("soundfx/melee_attack.ogg");
	sound_hit = Mix_LoadWAV("soundfx/male_hit.ogg");
	sound_die = Mix_LoadWAV("soundfx/male_die.ogg");
	sound_steps[0] = Mix_LoadWAV("soundfx/step_echo1.ogg");
	sound_steps[1] = Mix_LoadWAV("soundfx/step_echo2.ogg");
	sound_steps[2] = Mix_LoadWAV("soundfx/step_echo3.ogg");
	sound_steps[3] = Mix_LoadWAV("soundfx/step_echo4.ogg");
			
	if (!sound_weapon1 || !sound_hit || !sound_die || !sound_steps[0]) {
	  printf("Mix_LoadWAV: %s\n", Mix_GetError());
	  SDL_Quit();
	}
	
}

bool Avatar::pressing_move() {
	if(inp->pressing[UP] ||
	   inp->pressing[DOWN] ||
	   inp->pressing[LEFT] ||
	   inp->pressing[RIGHT])
		return true;
	return false;
}

/**
 * move()
 * Apply speed to the direction faced.
 *
 * @return Returns false if wall collision, otherwise true.
 */
bool Avatar::move() {
	switch (direction) {
		case 0:
			return map->collider.move(pos.x, pos.y, -1, 1, dspeed);
		case 1:
			return map->collider.move(pos.x, pos.y, -1, 0, speed);
		case 2:
			return map->collider.move(pos.x, pos.y, -1, -1, dspeed);
		case 3:
			return map->collider.move(pos.x, pos.y, 0, -1, speed);
		case 4:
			return map->collider.move(pos.x, pos.y, 1, -1, dspeed);
		case 5:
			return map->collider.move(pos.x, pos.y, 1, 0, speed);
		case 6:
			return map->collider.move(pos.x, pos.y, 1, 1, dspeed);
		case 7:
			return map->collider.move(pos.x, pos.y, 0, 1, speed);
	}
	return true;
}

void Avatar::set_direction() {
	// handle direction changes
	if(inp->pressing[UP] && inp->pressing[LEFT]) direction = 1;
	else if(inp->pressing[UP] && inp->pressing[RIGHT]) direction = 3;
	else if(inp->pressing[DOWN] && inp->pressing[RIGHT]) direction = 5;
	else if(inp->pressing[DOWN] && inp->pressing[LEFT]) direction = 7;
	else if(inp->pressing[LEFT]) direction = 0;
	else if(inp->pressing[UP]) direction = 2;
	else if(inp->pressing[RIGHT]) direction = 4;
	else if(inp->pressing[DOWN]) direction = 6;
}


/**
 * Change direction to face the target map location
 */
int Avatar::face(int mapx, int mapy) {

	// inverting Y to convert map coordinates to standard cartesian coordinates
	int dx = mapx - pos.x;
	int dy = pos.y - mapy;

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
	return direction;
}


/**
 * logic()
 * Handle a single frame.  This includes:
 * - move the avatar based on buttons pressed
 * - calculate the next frame of animation
 * - calculate camera position based on avatar position
 */
void Avatar::logic() {
	Point target;
	int stepfx;
	stats.logic();

	// handle internal cooldowns
	if (cooldown_melee > 0) cooldown_melee--;
	
	switch(curState) {
		case AVATAR_STANCE:
		
			// handle animation
		    curFrame++;
			if (curFrame >= 48) curFrame = 0;
			if (curFrame >= 24)
				dispFrame = (47 - curFrame) / 6;
			else
				dispFrame = curFrame / 6;
			
			// handle transitions to RUN
			set_direction();
			if (pressing_move()) {
				if (move()) { // no collision
					curFrame = 1;
					curState = AVATAR_RUN;
					break;
				}
			}
			if (inp->pressing[BAR_1] && cooldown_melee == 0) {
				curFrame = 0;
				curState = AVATAR_MELEE;
				break;
			}
			if (inp->pressing[MAIN1] && cooldown_melee == 0) {
				// change direction to mouse pointer direction
				target.x = inp->mouse.x;
				target.y = inp->mouse.y + aim_height;
				target = screen_to_map(target.x, target.y, pos.x, pos.y);
				direction = face(target.x, target.y);
								
				curFrame = 0;
				curState = AVATAR_MELEE;
				break;
			}
			break;
			
		case AVATAR_RUN:
		
			// handle animation
			curFrame++;
			if (curFrame >= 24) curFrame = 0;
			dispFrame = (curFrame /3) + 4;
			
			stepfx = rand() % 4;
			
			if (curFrame == 4 || curFrame == 16) {
				Mix_PlayChannel(-1, sound_steps[stepfx], 0);
			}

			// handle direction changes
			set_direction();

			// handle transition to STANCE
			if (!pressing_move()) {
				curState = AVATAR_STANCE;
				break;
			} 
			else if (!move()) { // collide with wall
				curState = AVATAR_STANCE;
				break;
			}
			if (inp->pressing[BAR_1] && cooldown_melee == 0) {
				curFrame = 0;
				curState = AVATAR_MELEE;
				break;
			}
			if (inp->pressing[MAIN1] && cooldown_melee == 0) {
				// change direction to mouse pointer direction
				// change direction to mouse pointer direction
				target.x = inp->mouse.x;
				target.y = inp->mouse.y + aim_height; // target helper
				target = screen_to_map(target.x, target.y, pos.x, pos.y);
				direction = face(target.x, target.y);
				
				curFrame = 0;
				curState = AVATAR_MELEE;
				break;
			}
							
			break;
			
		case AVATAR_MELEE:

			// handle animation
			curFrame++;
			dispFrame = (curFrame / 4) + 12;
			
			if (curFrame == 1) {
				Mix_PlayChannel(-1, sound_weapon1, 0);
			}
			
			// the attack hazard is alive for a single frame
			if (curFrame == 8 && haz == NULL) {
				haz = new Hazard();
				haz->setCollision(&(map->collider));
				haz->pos = calcVector(pos, direction, (UNITS_PER_TILE*3)/4);
				haz->radius = UNITS_PER_TILE;
				haz->source = SRC_HERO;
				haz->lifespan = 1;
			}
			
			if (curFrame == 15) {
				curFrame = 0;
				curState = AVATAR_STANCE;
				cooldown_melee = 8;
			}
			break;
			
		case AVATAR_HIT:
			curFrame++;
			
			if (curFrame == 1) {
				Mix_PlayChannel(-1, sound_hit, 0);
			}
			
			if (curFrame < 3) dispFrame = 18;
			else if (curFrame < 6) dispFrame = 19;
			else dispFrame = 18;
			
			if (curFrame >= 8) {
				curState = AVATAR_STANCE;
				curFrame = 0;
			}
			
			break;
		case AVATAR_DEAD:
			if (curFrame < 17) curFrame++;
			dispFrame = (curFrame /3) + 18;

			if (curFrame == 1) {
				Mix_PlayChannel(-1, sound_die, 0);
			}
			
			break;
		
		default:
			break;
	}
	
	// calc new cam position from player position
	// cam is focused at player position
	map->cam.x = pos.x;
	map->cam.y = pos.y;
	map->hero_tile.x = pos.x / 32;
	map->hero_tile.y = pos.y / 32;
	
	// check for map events
	map->checkEvents(pos);
}

void Avatar::takeHit(int dmg_min, int dmg_max) {
	int dmg;
	if (dmg_min == dmg_max) dmg = dmg_min;
	else dmg = dmg_min + (rand() % (dmg_max - dmg_min));

	if (curState != AVATAR_DEAD) {
		curFrame = 0;
		
		stats.hp = stats.hp - dmg;
			
		if (stats.hp <= 0) {
			dispFrame = 18;
			curState = AVATAR_DEAD;		
		}
		else {
			dispFrame = 18;
			curState = AVATAR_HIT;
		}
	}
}

/**
 * getRender()
 * Map objects need to be drawn in Z order, so we allow a parent object (GameEngine)
 * to collect all mobile sprites each frame.
 */
Renderable Avatar::getRender() {
	Renderable r;
	r.map_pos.x = pos.x;
	r.map_pos.y = pos.y;
	r.sprite = sprites;
	r.src = new SDL_Rect();
	r.src->x = 128 * dispFrame;
	r.src->y = 128 * direction;
	r.src->w = 128;
	r.src->h = 128;
	r.offset.x = 64;
	r.offset.y = 96; // 112
	return r;	
}

Avatar::~Avatar() {
	SDL_FreeSurface(sprites);
	Mix_FreeChunk(sound_weapon1);
	Mix_FreeChunk(sound_hit);
	Mix_FreeChunk(sound_die);
	Mix_FreeChunk(sound_steps[0]);
	Mix_FreeChunk(sound_steps[1]);
	Mix_FreeChunk(sound_steps[2]);
	Mix_FreeChunk(sound_steps[3]);
			
	if (haz != NULL) delete(haz);	
}
