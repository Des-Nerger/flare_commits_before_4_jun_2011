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
}

void Hazard::setCollision(MapCollision *_collider) {
	collider = _collider;
}

void Hazard::logic() {
	
	// handle tickers
	if (lifespan > 0) lifespan--;
	frame++;
	
	// handle movement
	if (!(round(speed.x) == 0 && round(speed.y) == 0)) {
		pos.x += speed.x;
		pos.y += speed.y;
		
		// very simplified collider, could skim around corners
		// or even pass through thin walls if speed > tilesize
		if (!collider->is_empty(round(pos.x), round(pos.y))) {
			lifespan = 0;
		}
	}

}
