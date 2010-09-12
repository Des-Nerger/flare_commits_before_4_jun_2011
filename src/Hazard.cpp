/**
 * class Hazard
 *
 * Stand-alone object that can harm the hero or creatures
 * These are generated whenever something makes any attack
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "Hazard.h"

Hazard::Hazard() {
	speed.x = 0.0;
	speed.y = 0.0;
	multitarget = false;
	active = true;
	dmg_min = 0;
	dmg_max = 0;
	crit_chance = 0;
	power_index = -1;
	rendered = false;
	lifespan=1;
	frame=0;
	frame_duration=1;
	frame_loop=1;
	active_frame=-1;
	frame_size.x = 64;
	frame_size.y = 64;
	frame_offset.x = 32;
	frame_offset.y = 32;
	floor=false;
}

void Hazard::setCollision(MapCollision *_collider) {
	collider = _collider;
}

void Hazard::logic() {
	
	// handle tickers
	if (lifespan > 0) lifespan--;
	frame++;
	if (frame == frame_loop) frame=0;
	
	// handle movement
	if (!(round(speed.x) == 0 && round(speed.y) == 0)) {
		pos.x += speed.x;
		pos.y += speed.y;
		
		// very simplified collider, could skim around corners
		// or even pass through thin walls if speed > tilesize
		if (collider->is_wall(round(pos.x), round(pos.y))) {
			lifespan = 0;
		}
	}

}
