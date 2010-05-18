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
	speed = 3;
	dspeed = 2;
	curState = ENEMY_STANCE;
	curFrame = 0;
	dispFrame = 0;
	direction = 7;
	pos.x = 200;
	pos.y = 200;
	
	meleeRange = UNITS_PER_TILE;
	threatRange = UNITS_PER_TILE *8;
	dirFavor = 12;
	dirTicks = 0;
	patrolTicks = 0;
	
	chance_melee_phys = 6;
	chance_melee_mag = 2;
	chance_ranged_phys = 0;
	chance_ranged_mag = 0;
	cooldown = 0;
	stats.hp = 3;
	
	haz = NULL;
	
	sfx_attack = false;
	sfx_hit = false;
	sfx_die = false;
	sfx_critdie = false;
	
	lastSeen.x = -1;
	lastSeen.y = -1;
	in_combat = false;
}

/**
 * move()
 * Apply speed to the direction faced.
 *
 * @return Returns false if wall collision, otherwise true.
 */
bool Enemy::move() {
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

/**
 * Change direction to face the target map location
 */
int Enemy::face(int mapx, int mapy) {

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
 * The current direction leads to a wall.  Try the next best direction, if one is available.
 */
int Enemy::faceNextBest(int mapx, int mapy) {
	int dx = abs(mapx - pos.x);
	int dy = abs(mapy - pos.y);
	switch (direction) {
		case 0:
			if (dy > dx) return 7;
			else return 1;
		case 1:
			if (mapy > pos.y) return 0;
			else return 2;
		case 2:
			if (dx > dy) return 1;
			else return 3;
		case 3:
			if (mapx < pos.x) return 2;
			else return 4;
		case 4:
			if (dy > dx) return 3;
			else return 5;
		case 5:
			if (mapy < pos.y) return 4;
			else return 6;
		case 6:
			if (dx > dy) return 5;
			else return 7;
		case 7:
			if (mapx > pos.x) return 6;
			else return 0;
	}
	return 0;
}

/**
 * Calculate distance between the enemy and the hero
 */
int Enemy::getDistance(Point dest) {
	int dx = dest.x - pos.x;
	int dy = dest.y - pos.y;
	double step1 = (double)dx * (double)dx + (double)dy * (double)dy;
	double step2 = sqrt(step1);
	return int(step2);
}

void Enemy::newState(int state) {
	curState = state;
	curFrame = 0;
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
	
	// general info about this enemy vs. the hero
	dist = getDistance(heroPos);
	if (dist < threatRange)
		los = map->collider.line_of_sight(pos.x, pos.y, heroPos.x, heroPos.y);
	else
		los = false;
		
	// set combat status and lastseen
	if (los && dist < threatRange) {
		in_combat = true;
		lastSeen.x = heroPos.x;
		lastSeen.y = heroPos.y;
	}
	else if (lastSeen.x >= 0 && lastSeen.y >= 0) {
		if (getDistance(lastSeen) <= (speed+speed) && patrolTicks == 0) {
			lastSeen.x = -1;
			lastSeen.y = -1;
			patrolTicks = 8;
		}		
	}
	if (dist > threatRange+threatRange) {
		in_combat = false;
		patrolTicks = 0;
		lastSeen.x = -1;
		lastSeen.y = -1;
	}
	
	// where is the creature heading?
	// TODO: add fleeing for X ticks
	if (los) {
		pursue_pos.x = lastSeen.x = heroPos.x;
		pursue_pos.y = lastSeen.y = heroPos.y;
		patrolTicks = 0;
	}
	else if (in_combat) {
		if (patrolTicks > 0) {
			patrolTicks--;
			if (patrolTicks == 0) {
				in_combat = false;
			}			
		}
		pursue_pos.x = lastSeen.x;
		pursue_pos.y = lastSeen.y;
	}

	if (cooldown > 0) cooldown--;
	
	switch(curState) {
		case ENEMY_STANCE:
		
			// handle animation
		    curFrame++;
			if (curFrame >= 80) curFrame = 0;
			if (curFrame >= 40)
				dispFrame = (79 - curFrame) / 10;
			else
				dispFrame = curFrame / 10;
			
			if (in_combat) {

				// update direction to face the target
				if (++dirTicks > dirFavor && patrolTicks == 0) {
					direction = face(pursue_pos.x, pursue_pos.y);				
					dirTicks = 0;
				}
		
				// performed ranged actions
				if (dist > meleeRange) {
				
					// CHECK: flee!
					// CHECK: pursue!
					if (move()) { // no collision
						newState(ENEMY_MOVE);
					}
					else {
						// hit an obstacle, try the next best angle
						prev_direction = direction;
						direction = faceNextBest(pursue_pos.x, pursue_pos.y);
						if (move()) {
							newState(ENEMY_MOVE);
						}
						else direction = prev_direction;
					}
					
				}
				// perform melee actions
				else if (dist <= meleeRange && cooldown == 0) {
				
					// CHECK: melee attack!
					if ((rand() % 100) < chance_melee_phys) {
						newState(ENEMY_MELEE_PHYS);
					}
					else if ((rand() % 100) < chance_melee_mag) {
						newState(ENEMY_MELEE_MAG);
					}
				}
			}
			
			break;
		
		case ENEMY_MOVE:
		
			// handle animation
			curFrame++;
			if (curFrame >= 32) curFrame = 0;
			dispFrame = (curFrame /4) + 4;
			
			if (in_combat) {

				if (++dirTicks > dirFavor && patrolTicks == 0) {
					direction = face(pursue_pos.x, pursue_pos.y);				
					dirTicks = 0;
				}
				
				if (dist > meleeRange) {
				
					if (!move()) {
						// hit an obstacle.  Try the next best angle
						prev_direction = direction;
						direction = faceNextBest(pursue_pos.x, pursue_pos.y);
						if (!move()) {
							newState(ENEMY_STANCE);
							direction = prev_direction;
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
			curFrame++;
			dispFrame = (curFrame / 4) + 12;

			if (curFrame == 1) {
				sfx_attack = true;
			}

			// the attack hazard is alive for a single frame
			if (curFrame == 8 && haz == NULL) {
				haz = new Hazard();
				haz->setCollision(&(map->collider));
				haz->pos = calcVector(pos, direction, (UNITS_PER_TILE*3)/4 );
				haz->radius = UNITS_PER_TILE;
				haz->source = SRC_ENEMY;
				haz->lifespan = 1;
			}

			if (curFrame == 15) {
				newState(ENEMY_STANCE);
				cooldown = 30;
			}
			break;
		
		case ENEMY_MELEE_MAG:
	
			// handle animation
			curFrame++;
			dispFrame = (curFrame / 4) + 16;
			
			if (curFrame == 15) {
				newState(ENEMY_STANCE);
				cooldown = 30;
			}
			break;
		
		case ENEMY_HIT:
			// enemy has taken damage.  cycle through 
			curFrame++;

			if (curFrame == 1) {
				sfx_hit = true;
			}
			
			if (curFrame < 3) dispFrame = 22;
			else if (curFrame < 6) dispFrame = 23;
			else dispFrame = 22;
			
			if (curFrame == 8) {
				newState(ENEMY_STANCE);
			}
			
			break;
		case ENEMY_DEAD:
			if (curFrame < 17) curFrame++;
			dispFrame = (curFrame /3) + 22;
		
			if (curFrame == 12) {
				sfx_die = true;
				sfx_hit = true;
			}
			
			break;
			
		case ENEMY_CRITDEAD:
			if (curFrame < 31) curFrame++;
			dispFrame = (curFrame /4) + 28;
			
			if (curFrame == 1) {
				sfx_critdie = true;
			}
			
			break;
	}

}

void Enemy::takeHit() {
	if (curState != ENEMY_DEAD && curState != ENEMY_CRITDEAD) {
		curFrame = 0;
		
		bool crit = (rand() % 100) < 30;
		
		if (crit)
			stats.hp = stats.hp -2;
		else
			stats.hp = stats.hp -1;
			
		if (stats.hp <= 0 && crit) {
			dispFrame = 28;
			curState = ENEMY_CRITDEAD;
		}
		else if (stats.hp <= 0) {
			dispFrame = 22;
			curState = ENEMY_DEAD;		
		}
		else {
			dispFrame = 22;
			curState = ENEMY_HIT;
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
	r.map_pos.x = pos.x;
	r.map_pos.y = pos.y;
	r.src = new SDL_Rect();
	r.src->x = 128 * dispFrame;
	r.src->y = 128 * direction;
	r.src->w = 128;
	r.src->h = 128;
	r.offset.x = 64;
	r.offset.y = 96;
	return r;	
}

Enemy::~Enemy() {

	if (haz != NULL) delete(haz);
}

