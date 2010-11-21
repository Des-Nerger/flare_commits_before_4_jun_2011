/**
 * class Avatar
 *
 * Contains logic and rendering routines for the player avatar.
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "Avatar.h"

Avatar::Avatar(PowerManager *_powers, InputState *_inp, MapIso *_map) {
	powers = _powers;
	inp = _inp;
	map = _map;
	
	// other init
	stats.cur_state = AVATAR_STANCE;
	stats.pos.x = map->spawn.x;
	stats.pos.y = map->spawn.y;
	stats.direction = map->spawn_dir;
	current_power = -1;
		
	stats.cur_frame = 1;
	stats.disp_frame = 0;
	lockSwing = false;
	lockCast = false;
	lockShoot = false;
	
	stats.name = "Unknown";
	stats.hero = true;
	stats.level = 1;
	stats.xp = 0;
	stats.physical = 1;
	stats.magical = 1;
	stats.offense = 1;
	stats.defense = 1;
	stats.speed = 10;
	stats.dspeed = 7;
	stats.recalc();
	
	log_msg = "";

	stats.cooldown_ticks = 0;
	
	haz = NULL;

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
	sound_melee = Mix_LoadWAV("soundfx/melee_attack.ogg");
	sound_ranged = Mix_LoadWAV("soundfx/ranged_attack.ogg");
	sound_hit = Mix_LoadWAV("soundfx/male_hit.ogg");
	sound_die = Mix_LoadWAV("soundfx/male_die.ogg");
	sound_block = Mix_LoadWAV("soundfx/powers/block.ogg");	
	sound_steps[0] = Mix_LoadWAV("soundfx/step_echo1.ogg");
	sound_steps[1] = Mix_LoadWAV("soundfx/step_echo2.ogg");
	sound_steps[2] = Mix_LoadWAV("soundfx/step_echo3.ogg");
	sound_steps[3] = Mix_LoadWAV("soundfx/step_echo4.ogg");
	level_up = Mix_LoadWAV("soundfx/level_up.ogg");
				
	if (!sound_melee || !sound_ranged || !sound_hit || !sound_die || !sound_steps[0] || !level_up) {
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

	int speed_factor = 1;
	if (stats.slow_duration > 0) speed_factor = 2;
	
	switch (stats.direction) {
		case 0:
			return map->collider.move(stats.pos.x, stats.pos.y, -1, 1, stats.dspeed/speed_factor);
		case 1:
			return map->collider.move(stats.pos.x, stats.pos.y, -1, 0, stats.speed/speed_factor);
		case 2:
			return map->collider.move(stats.pos.x, stats.pos.y, -1, -1, stats.dspeed/speed_factor);
		case 3:
			return map->collider.move(stats.pos.x, stats.pos.y, 0, -1, stats.speed/speed_factor);
		case 4:
			return map->collider.move(stats.pos.x, stats.pos.y, 1, -1, stats.dspeed/speed_factor);
		case 5:
			return map->collider.move(stats.pos.x, stats.pos.y, 1, 0, stats.speed/speed_factor);
		case 6:
			return map->collider.move(stats.pos.x, stats.pos.y, 1, 1, stats.dspeed/speed_factor);
		case 7:
			return map->collider.move(stats.pos.x, stats.pos.y, 0, 1, stats.speed/speed_factor);
	}
	return true;
}

void Avatar::set_direction() {
	// handle direction changes
	if(inp->pressing[UP] && inp->pressing[LEFT]) stats.direction = 1;
	else if(inp->pressing[UP] && inp->pressing[RIGHT]) stats.direction = 3;
	else if(inp->pressing[DOWN] && inp->pressing[RIGHT]) stats.direction = 5;
	else if(inp->pressing[DOWN] && inp->pressing[LEFT]) stats.direction = 7;
	else if(inp->pressing[LEFT]) stats.direction = 0;
	else if(inp->pressing[UP]) stats.direction = 2;
	else if(inp->pressing[RIGHT]) stats.direction = 4;
	else if(inp->pressing[DOWN]) stats.direction = 6;
}


/**
 * Change direction to face the target map location
 */
int Avatar::face(int mapx, int mapy) {

	// inverting Y to convert map coordinates to standard cartesian coordinates
	int dx = mapx - stats.pos.x;
	int dy = stats.pos.y - mapy;

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
	return stats.direction;
}


/**
 * logic()
 * Handle a single frame.  This includes:
 * - move the avatar based on buttons pressed
 * - calculate the next frame of animation
 * - calculate camera position based on avatar position
 *
 * @param power_index The actionbar power activated.  -1 means no power.
 */
void Avatar::logic(int actionbar_power) {
	Point target;
	int stepfx;
	stats.logic();
	if (stats.stun_duration > 0) return;
	
	// check level up
	if (stats.level < 9 && stats.xp >= stats.xp_table[stats.level]) {
		stats.level++;
		stringstream ss;
		ss << "Congratulations, you have reached level " << stats.level << "! You may increase one attribute through the Character Menu.";
		log_msg = ss.str();
		Mix_PlayChannel(-1, level_up, 0);
	}

	// check for bleeding spurt
	if (stats.bleed_duration % 30 == 1) {
		powers->activate(POWER_SPARK_BLOOD, &stats, stats.pos);
	}
	// check for bleeding to death
	if (stats.hp == 0 && !(stats.cur_state == AVATAR_DEAD)) {
		stats.cur_state = AVATAR_DEAD;
		stats.cur_frame = 0;
	}
	
	switch(stats.cur_state) {
		case AVATAR_STANCE:
		
			// handle animation
		    stats.cur_frame++;
			if (stats.cur_frame >= 48) stats.cur_frame = 0;
			if (stats.cur_frame >= 24)
				stats.disp_frame = (47 - stats.cur_frame) / 6;
			else
				stats.disp_frame = stats.cur_frame / 6;
			
			// handle transitions to RUN
			set_direction();
			if (pressing_move()) {
				if (move()) { // no collision
					stats.cur_frame = 1;
					stats.cur_state = AVATAR_RUN;
					break;
				}
			}
			
			// handle power usage
			if (actionbar_power != -1 && stats.cooldown_ticks == 0) {

				target = screen_to_map(inp->mouse.x,  inp->mouse.y + powers->powers[actionbar_power].aim_assist, stats.pos.x, stats.pos.y);

				// check requirements
				if (powers->powers[actionbar_power].requires_mana && stats.mp <= 0)
					break;
				if (powers->powers[actionbar_power].requires_ammo && !(stats.ammo_arrows || stats.ammo_stones))
					break;
				if (powers->powers[actionbar_power].requires_los && !map->collider.line_of_sight(stats.pos.x, stats.pos.y, target.x, target.y))
					break;
				if (powers->powers[actionbar_power].requires_empty_target && !map->collider.is_empty(target.x, target.y))
					break;
												
				current_power = actionbar_power;
			
				// is this a power that requires changing direction?
				if (powers->powers[current_power].face) {
					act_target.x = target.x;
					act_target.y = target.y;
					stats.direction = face(target.x, target.y);
				}
			
				// handle melee powers
				if (powers->powers[current_power].new_state == POWSTATE_SWING) {
					stats.cur_frame = 0;
					stats.cur_state = AVATAR_MELEE;
					break;
				}
				// handle ranged powers
				if (powers->powers[current_power].new_state == POWSTATE_SHOOT) {
					stats.cur_frame = 0;
					stats.cur_state = AVATAR_SHOOT;
					break;
				}
				// handle magic powers
				if (powers->powers[current_power].new_state == POWSTATE_CAST) {
					stats.cur_frame = 0;
					stats.cur_state = AVATAR_CAST;
					break;
				}
				if (powers->powers[current_power].new_state == POWSTATE_BLOCK) {
					stats.cur_frame = 0;
					stats.cur_state = AVATAR_BLOCK;
					stats.blocking = true;
					break;
				}
			}
			
			break;
			
		case AVATAR_RUN:
		
			// handle animation
			stats.cur_frame++;
			if (stats.cur_frame >= 24) stats.cur_frame = 0;
			stats.disp_frame = (stats.cur_frame /3) + 4;
			
			stepfx = rand() % 4;
			
			if (stats.cur_frame == 4 || stats.cur_frame == 16) {
				Mix_PlayChannel(-1, sound_steps[stepfx], 0);
			}

			// handle direction changes
			set_direction();

			// handle transition to STANCE
			if (!pressing_move()) {
				stats.cur_state = AVATAR_STANCE;
				break;
			} 
			else if (!move()) { // collide with wall
				stats.cur_state = AVATAR_STANCE;
				break;
			}

			// handle power usage
			if (actionbar_power != -1 && stats.cooldown_ticks == 0) {

				target = screen_to_map(inp->mouse.x,  inp->mouse.y + powers->powers[actionbar_power].aim_assist, stats.pos.x, stats.pos.y);
			
				// check requirements
				if (powers->powers[actionbar_power].requires_mana && stats.mp <= 0)
					break;
				if (powers->powers[actionbar_power].requires_ammo && !(stats.ammo_arrows || stats.ammo_stones))
					break;
				if (powers->powers[actionbar_power].requires_los && !map->collider.line_of_sight(stats.pos.x, stats.pos.y, target.x, target.y))
					break;
				if (powers->powers[actionbar_power].requires_empty_target && !map->collider.is_empty(target.x, target.y))
					break;
			
				current_power = actionbar_power;
			
				// is this a power that requires changing direction?
				if (powers->powers[current_power].face) {
					act_target.x = target.x;
					act_target.y = target.y;
					stats.direction = face(target.x, target.y);
				}
			
				// handle melee powers
				if (powers->powers[current_power].new_state == POWSTATE_SWING) {
					stats.cur_frame = 0;
					stats.cur_state = AVATAR_MELEE;
					break;
				}
				// handle ranged powers
				if (powers->powers[current_power].new_state == POWSTATE_SHOOT) {
					stats.cur_frame = 0;
					stats.cur_state = AVATAR_SHOOT;
					break;
				}
				// handle magic powers
				if (powers->powers[current_power].new_state == POWSTATE_CAST) {
					stats.cur_frame = 0;
					stats.cur_state = AVATAR_CAST;
					break;
				}
				if (powers->powers[current_power].new_state == POWSTATE_BLOCK) {
					stats.cur_frame = 0;
					stats.cur_state = AVATAR_BLOCK;
					stats.blocking = true;
					break;
				}				
			}
							
			break;
			
		case AVATAR_MELEE:

			// handle animation
			stats.cur_frame++;
			stats.disp_frame = (stats.cur_frame / 4) + 12;
			
			if (stats.cur_frame == 1) {
				Mix_PlayChannel(-1, sound_melee, 0);
			}
			
			// do power
			if (stats.cur_frame == 8) {
				powers->activate(current_power, &stats, act_target);
			}
			
			if (stats.cur_frame == 15) {
				stats.cur_frame = 0;
				stats.cur_state = AVATAR_STANCE;
				stats.cooldown_ticks = 8;
			}
			break;

		case AVATAR_CAST:
		
			// handle animation
			stats.cur_frame++;
			stats.disp_frame = (stats.cur_frame / 4) + 24;

			// do power
			if (stats.cur_frame == 8) {
				powers->activate(current_power, &stats, act_target);
				
				// TEMP
				if (current_power == POWER_QUAKE) {
					map->shaky_cam_ticks = 8;
				}
			}

			if (stats.cur_frame == 15) {
				stats.cur_frame = 0;
				stats.cur_state = AVATAR_STANCE;
				stats.cooldown_ticks = 8;
			}
			break;

			
		case AVATAR_SHOOT:
		
			// handle animation
			stats.cur_frame++;
			stats.disp_frame = (stats.cur_frame / 4) + 28;

			if (stats.cur_frame == 1) {
				Mix_PlayChannel(-1, sound_ranged, 0);
			}

			// do power
			if (stats.cur_frame == 8) {
				powers->activate(current_power, &stats, act_target);
			}

			if (stats.cur_frame == 15) {
				stats.cur_frame = 0;
				stats.cur_state = AVATAR_STANCE;
				stats.cooldown_ticks = 8;
			}
			break;

		case AVATAR_BLOCK:
			if (stats.cur_frame < 4) stats.cur_frame++;
			stats.disp_frame = (stats.cur_frame / 4) + 16;
			
			if (actionbar_power != POWER_BLOCK) {
				stats.cur_frame = 0;
				stats.cur_state = AVATAR_STANCE;
				stats.cooldown_ticks = 8;
				stats.blocking = false;
			}
			break;
			
		case AVATAR_HIT:
			stats.cur_frame++;
			
			
			if (stats.cur_frame < 3) stats.disp_frame = 18;
			else if (stats.cur_frame < 6) stats.disp_frame = 19;
			else stats.disp_frame = 18;
			
			if (stats.cur_frame >= 8) {
				stats.cur_state = AVATAR_STANCE;
				stats.cur_frame = 0;
			}
			
			break;
		case AVATAR_DEAD:
			if (stats.cur_frame < 17) stats.cur_frame++;
			stats.disp_frame = (stats.cur_frame /3) + 18;

			if (stats.cur_frame == 1) {
				Mix_PlayChannel(-1, sound_die, 0);
				log_msg = "You are defeated.  Press Enter to continue.";
			}
			
			// allow respawn with Accept
			if (inp->pressing[ACCEPT]) {
				stats.hp = stats.maxhp;
				stats.mp = stats.maxmp;
				stats.alive = true;
				stats.corpse = false;
				stats.cur_frame = 0;
				stats.cur_state = AVATAR_STANCE;
				
				// set teleportation variables.  GameEngine acts on these.
				map->teleportation = true;
				map->teleport_mapname = map->respawn_map;
				map->teleport_destination.x = map->respawn_point.x;
				map->teleport_destination.y = map->respawn_point.y;
			}
			
			break;
		
		default:
			break;
	}
	
	// calc new cam position from player position
	// cam is focused at player position
	map->cam.x = stats.pos.x;
	map->cam.y = stats.pos.y;
	map->hero_tile.x = stats.pos.x / 32;
	map->hero_tile.y = stats.pos.y / 32;
	
	// check for map events
	map->checkEvents(stats.pos);
}

/**
 * Called by HazardManager
 * Return false on a miss
 */
bool Avatar::takeHit(Hazard h) {

	if (stats.cur_state != AVATAR_DEAD) {
	
		// auto-miss if recently attacked
		// this is mainly to prevent slow, wide missiles from getting multiple attack attempts
		if (stats.targeted > 0) return false;
		stats.targeted = 5;	
	
		// check miss
	    if (rand() % 100 > (h.accuracy - stats.avoidance + 25)) return false; 
	
		int dmg;
		if (h.dmg_min == h.dmg_max) dmg = h.dmg_min;
		else dmg = h.dmg_min + (rand() % (h.dmg_max - h.dmg_min + 1));
	
		int absorption;
		if (!h.trait_armor_penetration) { // armor penetration ignores all absorption
			if (stats.absorb_min == stats.absorb_max) absorption = stats.absorb_min;
			else absorption = stats.absorb_min + (rand() % (stats.absorb_max - stats.absorb_min + 1));
			
			if (stats.blocking) absorption += absorption; // blocking doubles your absorb amount
			
			dmg = dmg - absorption;
			if (dmg < 1 && !stats.blocking) dmg = 1; // when blocking, dmg can be reduced to 0
			if (dmg <= 0) {
				dmg = 0;
				Mix_PlayChannel(-1, sound_block, 0);
				stats.cur_frame = 0; // shield stutter
			}
			
		}
	
		
		int prev_hp = stats.hp;
		stats.takeDamage(dmg);
		
		// after effects
		if (stats.hp > 0 && stats.immunity_duration == 0) {
			if (h.stun_duration > stats.stun_duration) stats.stun_duration = h.stun_duration;
			if (h.slow_duration > stats.slow_duration) stats.slow_duration = h.slow_duration;
			if (h.bleed_duration > stats.bleed_duration) stats.bleed_duration = h.bleed_duration;
			if (h.immobilize_duration > stats.immobilize_duration) stats.immobilize_duration = h.immobilize_duration;
		}
		
		// create a blood spark upon bloody attack
		Point pt;
		if (h.bleed_duration > 0 && dmg > 0) {
			powers->activate(POWER_SPARK_BLOOD, &stats, pt);
		}
		
		// Power-specific: Vengeance gains stacks when blocking
		if (stats.blocking && stats.physdef >= 9) {
			if (stats.vengeance_stacks < 3)
				stats.vengeance_stacks++;
		}
		
		
		if (stats.hp <= 0) {
			stats.cur_frame = 0;
			stats.disp_frame = 18;
			stats.cur_state = AVATAR_DEAD;		
		}
		else if (prev_hp > stats.hp) { // only interrupt if damage was taken
			Mix_PlayChannel(-1, sound_hit, 0);
			stats.cur_frame = 0;
			stats.disp_frame = 18;
			stats.cur_state = AVATAR_HIT;
		}
		
		return true;
	}
	return false;
}

/**
 * getRender()
 * Map objects need to be drawn in Z order, so we allow a parent object (GameEngine)
 * to collect all mobile sprites each frame.
 */
Renderable Avatar::getRender() {
	Renderable r;
	r.map_pos.x = stats.pos.x;
	r.map_pos.y = stats.pos.y;
	r.sprite = sprites;
	r.src = new SDL_Rect();
	r.src->x = 128 * stats.disp_frame;
	r.src->y = 128 * stats.direction;
	r.src->w = 128;
	r.src->h = 128;
	r.offset.x = 64;
	r.offset.y = 96; // 112
	r.object_layer = true;
	return r;	
}

Avatar::~Avatar() {
	SDL_FreeSurface(sprites);
	Mix_FreeChunk(sound_melee);
	Mix_FreeChunk(sound_ranged);	
	Mix_FreeChunk(sound_hit);
	Mix_FreeChunk(sound_die);
	Mix_FreeChunk(sound_block);
	Mix_FreeChunk(sound_steps[0]);
	Mix_FreeChunk(sound_steps[1]);
	Mix_FreeChunk(sound_steps[2]);
	Mix_FreeChunk(sound_steps[3]);
	Mix_FreeChunk(level_up);
			
	if (haz != NULL) delete(haz);	
}
