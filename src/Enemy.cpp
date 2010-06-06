/*
 * class Enemy
 *
 * @author Clint Bellanger
 * @license GPL
 *
 */

#include "Enemy.h"

Enemy::Enemy(MapIso *_map) {
	map = _map;

	stats.cur_state = ENEMY_STANCE;
	stats.cur_frame = 0;
	stats.disp_frame = 0;	
	stats.dir_ticks = 0;
	stats.patrol_ticks = 0;
	stats.cooldown = 0;
	stats.last_seen.x = -1;
	stats.last_seen.y = -1;
	stats.in_combat = false;
	
	haz = NULL;
	
	sfx_phys_melee = false;
	sfx_mag_melee = false;
	sfx_hit = false;
	sfx_die = false;
	sfx_critdie = false;
}

/**
 * move()
 * Apply speed to the direction faced.
 *
 * @return Returns false if wall collision, otherwise true.
 */
bool Enemy::move() {
	switch (stats.direction) {
		case 0:
			return map->collider.move(stats.pos.x, stats.pos.y, -1, 1, stats.dspeed);
		case 1:
			return map->collider.move(stats.pos.x, stats.pos.y, -1, 0, stats.speed);
		case 2:
			return map->collider.move(stats.pos.x, stats.pos.y, -1, -1, stats.dspeed);
		case 3:
			return map->collider.move(stats.pos.x, stats.pos.y, 0, -1, stats.speed);
		case 4:
			return map->collider.move(stats.pos.x, stats.pos.y, 1, -1, stats.dspeed);
		case 5:
			return map->collider.move(stats.pos.x, stats.pos.y, 1, 0, stats.speed);
		case 6:
			return map->collider.move(stats.pos.x, stats.pos.y, 1, 1, stats.dspeed);
		case 7:
			return map->collider.move(stats.pos.x, stats.pos.y, 0, 1, stats.speed);
	}
	return true;
}

/**
 * Change direction to face the target map location
 */
int Enemy::face(int mapx, int mapy) {

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
 * The current direction leads to a wall.  Try the next best direction, if one is available.
 */
int Enemy::faceNextBest(int mapx, int mapy) {
	int dx = abs(mapx - stats.pos.x);
	int dy = abs(mapy - stats.pos.y);
	switch (stats.direction) {
		case 0:
			if (dy > dx) return 7;
			else return 1;
		case 1:
			if (mapy > stats.pos.y) return 0;
			else return 2;
		case 2:
			if (dx > dy) return 1;
			else return 3;
		case 3:
			if (mapx < stats.pos.x) return 2;
			else return 4;
		case 4:
			if (dy > dx) return 3;
			else return 5;
		case 5:
			if (mapy < stats.pos.y) return 4;
			else return 6;
		case 6:
			if (dx > dy) return 5;
			else return 7;
		case 7:
			if (mapx > stats.pos.x) return 6;
			else return 0;
	}
	return 0;
}

/**
 * Calculate distance between the enemy and the hero
 */
int Enemy::getDistance(Point dest) {
	int dx = dest.x - stats.pos.x;
	int dy = dest.y - stats.pos.y;
	double step1 = (double)dx * (double)dx + (double)dy * (double)dy;
	double step2 = sqrt(step1);
	return int(step2);
}

void Enemy::newState(int state) {
	stats.cur_state = state;
	stats.cur_frame = 0;
}
	
/**
 * logic()
 * Handle a single frame.  This includes:
 * - move the enemy based on AI % chances
 * - calculate the next frame of animation
 */
void Enemy::logic() {

	stats.logic();
	int dist;
	int prev_direction;
	bool los = false;
	Point pursue_pos;
	int max_frame;
	int mid_frame;
	
	// general info about this enemy vs. the hero
	dist = getDistance(stats.hero_pos);
	if (dist < stats.threat_range)
		los = map->collider.line_of_sight(stats.pos.x, stats.pos.y, stats.hero_pos.x, stats.hero_pos.y);
	else
		los = false;
		
	// set combat status and lastseen
	if (los && dist < stats.threat_range) {
		stats.in_combat = true;
		stats.last_seen.x = stats.hero_pos.x;
		stats.last_seen.y = stats.hero_pos.y;
	}
	else if (stats.last_seen.x >= 0 && stats.last_seen.y >= 0) {
		if (getDistance(stats.last_seen) <= (stats.speed+stats.speed) && stats.patrol_ticks == 0) {
			stats.last_seen.x = -1;
			stats.last_seen.y = -1;
			stats.patrol_ticks = 8;
		}		
	}
	if (dist > stats.threat_range+stats.threat_range) {
		stats.in_combat = false;
		stats.patrol_ticks = 0;
		stats.last_seen.x = -1;
		stats.last_seen.y = -1;
	}
	
	// where is the creature heading?
	// TODO: add fleeing for X ticks
	if (los) {
		pursue_pos.x = stats.last_seen.x = stats.hero_pos.x;
		pursue_pos.y = stats.last_seen.y = stats.hero_pos.y;
		stats.patrol_ticks = 0;
	}
	else if (stats.in_combat) {
		if (stats.patrol_ticks > 0) {
			stats.patrol_ticks--;
			if (stats.patrol_ticks == 0) {
				stats.in_combat = false;
			}			
		}
		pursue_pos.x = stats.last_seen.x;
		pursue_pos.y = stats.last_seen.y;
	}

	if (stats.cooldown_ticks > 0) stats.cooldown_ticks--;
	
	switch(stats.cur_state) {
		case ENEMY_STANCE:
		
			// handle animation
		    stats.cur_frame++;
			
			// stance is a back/forth animation
			mid_frame = stats.anim_stance_frames * stats.anim_stance_duration;
			max_frame = mid_frame + mid_frame;
			if (stats.cur_frame >= max_frame) stats.cur_frame = 0;
			if (stats.cur_frame >= mid_frame)
				stats.disp_frame = (max_frame -1 - stats.cur_frame) / stats.anim_stance_duration + stats.anim_stance_position;
			else
				stats.disp_frame = stats.cur_frame / stats.anim_stance_duration + stats.anim_stance_position;
			
			if (stats.in_combat) {

				// update direction to face the target
				if (++stats.dir_ticks > stats.dir_favor && stats.patrol_ticks == 0) {
					stats.direction = face(pursue_pos.x, pursue_pos.y);				
					stats.dir_ticks = 0;
				}
		
				// performed ranged actions
				if (dist > stats.melee_range) {
				
					// CHECK: flee!
					
					// CHECK: pursue!
					if (move()) { // no collision
						newState(ENEMY_MOVE);
					}
					else {
						// hit an obstacle, try the next best angle
						prev_direction = stats.direction;
						stats.direction = faceNextBest(pursue_pos.x, pursue_pos.y);
						if (move()) {
							newState(ENEMY_MOVE);
						}
						else stats.direction = prev_direction;
					}
					
				}
				// perform melee actions
				else if (dist <= stats.melee_range && stats.cooldown_ticks == 0) {
				
					// CHECK: melee attack!
					if ((rand() % 100) < stats.chance_melee_phys) {
						newState(ENEMY_MELEE_PHYS);
					}
					else if ((rand() % 100) < stats.chance_melee_mag) {
						newState(ENEMY_MELEE_MAG);
					}
				}
			}
			
			break;
		
		case ENEMY_MOVE:
		
			// handle animation
			stats.cur_frame++;
			
			// run is a looped animation
			max_frame = stats.anim_run_frames * stats.anim_run_duration;
			if (stats.cur_frame >= max_frame) stats.cur_frame = 0;
			stats.disp_frame = (stats.cur_frame / stats.anim_run_duration) + stats.anim_run_position;
			
			if (stats.in_combat) {

				if (++stats.dir_ticks > stats.dir_favor && stats.patrol_ticks == 0) {
					stats.direction = face(pursue_pos.x, pursue_pos.y);				
					stats.dir_ticks = 0;
				}
				
				if (dist > stats.melee_range) {
				
					if (!move()) {
						// hit an obstacle.  Try the next best angle
						prev_direction = stats.direction;
						stats.direction = faceNextBest(pursue_pos.x, pursue_pos.y);
						if (!move()) {
							newState(ENEMY_STANCE);
							stats.direction = prev_direction;
						}
					}
				}
				else {
					newState(ENEMY_STANCE);
				}
			}
			else {
				newState(ENEMY_STANCE);
			}
			break;
			
		case ENEMY_MELEE_PHYS:
		
			// handle animation
			stats.cur_frame++;
			
			// melee is a play-once animation
			max_frame = stats.anim_melee_frames * stats.anim_melee_duration;
			stats.disp_frame = (stats.cur_frame / stats.anim_melee_duration) + stats.anim_melee_position;

			if (stats.cur_frame == 1) {
				sfx_phys_melee = true;
			}

			// the attack hazard is alive for a single frame
			if (stats.cur_frame == max_frame/2 && haz == NULL) {
				haz = new Hazard();
				haz->setCollision(&(map->collider));
				haz->pos = calcVector(stats.pos, stats.direction, (UNITS_PER_TILE*3)/4 );
				haz->radius = UNITS_PER_TILE;
				haz->source = SRC_ENEMY;
				haz->lifespan = 1;
				haz->dmg_min = stats.dmg_melee_min;
				haz->dmg_max = stats.dmg_melee_max;
				haz->accuracy = stats.accuracy;
			}

			if (stats.cur_frame == max_frame-1) {
				newState(ENEMY_STANCE);
				stats.cooldown_ticks = stats.cooldown;
			}
			break;
		
		case ENEMY_MELEE_MAG:
	
			// handle animation
			stats.cur_frame++;
			max_frame = stats.anim_magic_frames * stats.anim_magic_duration;
			stats.disp_frame = (stats.cur_frame / stats.anim_magic_duration) + stats.anim_magic_position;

			if (stats.cur_frame == 1) {
				// TODO: update
				sfx_mag_melee = true;
			}
			
			// the attack hazard is alive for a single frame
			if (stats.cur_frame == max_frame/2 && haz == NULL) {
				haz = new Hazard();
				haz->setCollision(&(map->collider));
				haz->pos = calcVector(stats.pos, stats.direction, (UNITS_PER_TILE*3)/4 );
				haz->radius = UNITS_PER_TILE;
				haz->source = SRC_ENEMY;
				haz->lifespan = 1;
				haz->dmg_min = stats.dmg_magic_min;
				haz->dmg_max = stats.dmg_magic_max;
				haz->accuracy = stats.accuracy;
			}
			
			if (stats.cur_frame == max_frame-1) {
				newState(ENEMY_STANCE);
				stats.cooldown_ticks = stats.cooldown;
			}
			break;
		
		case ENEMY_HIT:
			// enemy has taken damage.  cycle through 
			stats.cur_frame++;

			// hit is a back/forth animation
			mid_frame = stats.anim_hit_frames * stats.anim_hit_duration;
			max_frame = mid_frame + mid_frame;
			if (stats.cur_frame >= mid_frame)
				stats.disp_frame = (max_frame -1 - stats.cur_frame) / stats.anim_hit_duration + stats.anim_hit_position;
			else
				stats.disp_frame = stats.cur_frame / stats.anim_hit_duration + stats.anim_hit_position;
			
			if (stats.cur_frame == 1) {
				sfx_hit = true;
			}
			
			if (stats.cur_frame == max_frame-1) {
				newState(ENEMY_STANCE);
			}
			
			break;
		case ENEMY_DEAD:
			max_frame = (stats.anim_die_frames-1) * stats.anim_die_duration;
			if (stats.cur_frame < max_frame) stats.cur_frame++;
			stats.disp_frame = (stats.cur_frame / stats.anim_die_duration) + stats.anim_die_position;
			
			if (stats.cur_frame == 1) sfx_hit = true;
			if (stats.cur_frame == 1) sfx_die = true;
			
			break;
			
		case ENEMY_CRITDEAD:
			max_frame = (stats.anim_critdie_frames-1) * stats.anim_critdie_duration;
			if (stats.cur_frame < max_frame) stats.cur_frame++;
			stats.disp_frame = (stats.cur_frame / stats.anim_critdie_duration) + stats.anim_critdie_position;
			
			if (stats.cur_frame == 1) {
				sfx_critdie = true;
			}
			
			break;
	}

}

void Enemy::takeHit(int dmg_min, int dmg_max, int accuracy, int crit) {
	if (stats.cur_state != ENEMY_DEAD && stats.cur_state != ENEMY_CRITDEAD) {
		
		// check miss
	    if (rand() % 100 > (accuracy - stats.avoidance + 25)) return; 
		
		stats.cur_frame = 0;
		
		bool crit = (rand() % 100) < 30;
		int dmg;
		if (dmg_max > dmg_min) dmg = rand() % (dmg_max - dmg_min + 1) + dmg_min;
		else dmg = dmg_min;
		
		int absorption;
		if (stats.absorb_min == stats.absorb_max) absorption = stats.absorb_min;
		else absorption = stats.absorb_min + (rand() % (stats.absorb_max - stats.absorb_min + 1));
	
		dmg = dmg - absorption;
		if (dmg < 1) dmg = 1; // TODO: when blocking, dmg can be reduced to 0
		
		if (crit) dmg = dmg * 2;
		
		stats.hp = stats.hp - dmg;
			
		if (stats.hp <= 0 && crit) {
			stats.disp_frame = 28;
			stats.cur_state = ENEMY_CRITDEAD;
		}
		else if (stats.hp <= 0) {
			stats.disp_frame = 22;
			stats.cur_state = ENEMY_DEAD;		
		}
		else {
			stats.disp_frame = 22;
			stats.cur_state = ENEMY_HIT;
		}
	}
}

/**
 * getRender()
 * Map objects need to be drawn in Z order, so we allow a parent object (GameEngine)
 * to collect all mobile sprites each frame.
 */
Renderable Enemy::getRender() {
	Renderable r;
	r.map_pos.x = stats.pos.x;
	r.map_pos.y = stats.pos.y;
	r.src = new SDL_Rect();
	r.src->x = stats.render_size.x * stats.disp_frame;
	r.src->y = stats.render_size.y * stats.direction;
	r.src->w = stats.render_size.x;
	r.src->h = stats.render_size.y;
	r.offset.x = stats.render_offset.x;
	r.offset.y = stats.render_offset.y;
	return r;	
}

Enemy::~Enemy() {

	if (haz != NULL) delete(haz);
}

