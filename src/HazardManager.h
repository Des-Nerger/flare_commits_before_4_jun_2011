/**
 * class HazardManager
 *
 * Holds the collection of hazards (active attacks, spells, etc) and handles group operations
 *
 * @author Clint Bellanger
 * @license GPL
 */
 
#ifndef HAZARD_MANAGER_H
#define HAZARD_MANAGER_H

#include "Avatar.h"
#include "EnemyManager.h"
#include "Utils.h"
#include "Hazard.h"

class HazardManager {
private:
	Avatar *hero;
	EnemyManager *enemies;
public:
	HazardManager(Avatar *_hero, EnemyManager *_enemies);
	~HazardManager();
	void logic();
	void expire(int index);
	void checkNewHazards();
	void handleNewMap();
	
	int hazard_count;
	Hazard *h[256];
};

#endif
