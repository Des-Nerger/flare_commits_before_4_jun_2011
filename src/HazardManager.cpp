/**
 * class HazardManager
 *
 * Holds the collection of hazards (active attacks, spells, etc) and handles group operations
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "HazardManager.h"

HazardManager::HazardManager(Avatar *_hero, EnemyManager *_enemies) {
	hero = _hero;
	enemies = _enemies;
	hazard_count = 0;
}

void HazardManager::logic() {
	// remove all hazards with lifespan 0
	for (int i=0; i<hazard_count; i++) {
		if (h[i]->lifespan == 0) {
			expire(i);
		}
	}
	
	checkNewHazards();
	
	// handle single-frame transforms
	for (int i=0; i<hazard_count; i++) {
		h[i]->logic();
	}
	
	// handle collisions
	for (int i=0; i<hazard_count; i++) {
		if (h[i]->active) {
	
			// process hazards that can hurt enemies
			if (h[i]->source == SRC_HERO || h[i]->source == SRC_NEUTRAL) {
				for (int eindex = 0; eindex < enemies->enemy_count; eindex++) {
			
					// only check living enemies
					if (enemies->enemies[eindex]->stats.hp > 0 && h[i]->active) {
						if (isWithin(round(h[i]->pos), h[i]->radius, enemies->enemies[eindex]->pos)) {
							// hit!
							enemies->enemies[eindex]->takeHit();
							if (!h[i]->multitarget) {
								h[i]->active = false;
								h[i]->lifespan = 0;
							}
						}
					}
				
				}
			}
		
			// process hazards that can hurt the hero
			if (h[i]->source == SRC_ENEMY || h[i]->source == SRC_NEUTRAL) {
				if (hero->stats.hp > 0 && h[i]->active) {
					if (isWithin(round(h[i]->pos), h[i]->radius, hero->pos)) {
						// hit!
						hero->takeHit();
						if (!h[i]->multitarget) {
							h[i]->active = false;
							h[i]->lifespan = 0;
						}
					}
				}
			}
			
		}
	}
}

void HazardManager::checkNewHazards() {
	if (hero->haz != NULL) {
		h[hazard_count] = hero->haz;
		hazard_count++;
		hero->haz = NULL;
	}
	// TODO: check monster hazards
	for (int eindex = 0; eindex < enemies->enemy_count; eindex++) {
		if (enemies->enemies[eindex]->haz != NULL) {
			h[hazard_count] = enemies->enemies[eindex]->haz;
			hazard_count++;
			enemies->enemies[eindex]->haz = NULL;
		}
	}
}

void HazardManager::expire(int index) {
	// TODO: assert this instead?
	if (index >= 0 && index < hazard_count) {
		delete(h[index]);
		for (int i=index; i<hazard_count-1; i++) {
			h[i] = h[i+1];
		}
		hazard_count--;
	}
}

void HazardManager::handleNewMap() {
	hazard_count = 0;
}

HazardManager::~HazardManager() {
	for (int i=0; i<hazard_count; i++) {
		delete(h[i]);
	}
}
