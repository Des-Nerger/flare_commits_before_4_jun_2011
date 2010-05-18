/*
 * class EnemyManager
 *
 * @author Clint Bellanger
 * @license GPL
 *
 */
 
#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H

#include "MapIso.h"
#include "Enemy.h"
#include "Utils.h"

// Enemies of the same name share the same behavior type
struct EnemyBehavior {
	int melee_range;
	int threat_range;

	int direction_duration;
	int patrol_duration;

	int chance_pursue;
	int chance_flee;
	int chance_melee_phys;
	int chance_melee_mag;
	int chance_ranged_phys;
	int chance_ranged_mag;

	int cooldown_melee_phys;
	int cooldown_melee_mag;
	int cooldown_ranged_phys;
	int cooldown_ranged_mag;
};

class EnemyManager {
private:
	MapIso *map;
	void loadGraphics();
	void loadSounds();

	SDL_Surface *sprites;
	Mix_Chunk *sound_attack;
	Mix_Chunk *sound_hit;
	Mix_Chunk *sound_die;
	Mix_Chunk *sound_critdie;
	
public:
	EnemyManager(MapIso *_map);
	~EnemyManager();
	void handleNewMap();
	void logic();
	Renderable getRender(int enemyIndex);

	// vars
	Enemy *enemies[256]; // TODO: change to dynamic list without limits
	Point heroPos;
	int enemy_count;
};


#endif
