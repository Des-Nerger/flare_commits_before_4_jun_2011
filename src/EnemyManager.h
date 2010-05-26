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

const int max_classes = 8;

class EnemyManager {
private:
	MapIso *map;
	void loadGraphics(string type_id);
	void loadSounds(string type_id);
	void loadAssets(string type_id);

	string file_prefixes[max_classes];
	int file_prefix_count;
	SDL_Surface *sprites[max_classes];	
	Mix_Chunk *sound_phys_melee[max_classes];
	Mix_Chunk *sound_hit[max_classes];
	Mix_Chunk *sound_die[max_classes];
	Mix_Chunk *sound_critdie[max_classes];
	
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
