/*
 * class EnemyManager
 *
 * @author Clint Bellanger
 * @license GPL
 *
 */

#include "EnemyManager.h"

EnemyManager::EnemyManager(MapIso *_map) {
	map = _map;
	enemy_count = 0;
	file_prefix_count = 0;
	
	handleNewMap();
}

/**
 * Enemies share graphic/sound resources (usually there are groups of similar enemies)
 */
void EnemyManager::loadGraphics(string type_id) {
	
	// TODO: throw an error if a map tries to use too many monsters
	if (file_prefix_count == max_classes) return;
	
	// first check to make sure the sprite isn't already loaded
	for (int i=0; i<file_prefix_count; i++) {
		if (file_prefixes[i] == type_id) {
			return; // already have this one
		}
	}

	sprites[file_prefix_count] = IMG_Load(("images/enemies/" + type_id + ".png").c_str());
	if(!sprites[file_prefix_count]) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
	file_prefixes[file_prefix_count] = type_id;
	file_prefix_count++;
}

void EnemyManager::loadSounds(string type_id) {

	int i = file_prefix_count-1;
	
	sound_phys_melee[i] = Mix_LoadWAV(("soundfx/enemies/" + type_id + "_phys_melee.ogg").c_str());
	sound_hit[i] = Mix_LoadWAV(("soundfx/enemies/" + type_id + "_hit.ogg").c_str());
	sound_die[i] = Mix_LoadWAV(("soundfx/enemies/" + type_id + "_die.ogg").c_str());
	sound_critdie[i] = Mix_LoadWAV(("soundfx/enemies/" + type_id + "_critdie.ogg").c_str());
		
	if (!sound_phys_melee[i] 
	 || !sound_hit[i]
	 || !sound_die[i] 
	 || !sound_critdie[i]) {
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
		SDL_Quit();
	}	
}

void EnemyManager::loadAssets(string type_id) {
	int current = file_prefix_count;
	loadGraphics(type_id);
	if (file_prefix_count > current) {
		// new enemy type, so also load on this id
		loadSounds(type_id);
	}
}

/**
 * When loading a new map, we eliminate existing enemies and load the new ones.
 * The map will have loaded Entity blocks into an array; retrieve the Enemies and init them
 */
void EnemyManager::handleNewMap () {
	// TODO: if we want dead monsters to stay dead, or active monsters to stay in new positions,
	// commit this data back to the map.  For now, entering a new map resets it.
	
	Map_Enemy me;
	
	enemy_count = 0;
	
	for (int j=0; j<file_prefix_count; j++) {
		SDL_FreeSurface(sprites[j]);
		Mix_FreeChunk(sound_phys_melee[j]);
		Mix_FreeChunk(sound_hit[j]);
		Mix_FreeChunk(sound_die[j]);
		Mix_FreeChunk(sound_critdie[j]);
	}
	file_prefix_count = 0;
	
	while (!map->enemies.empty()) {
		me = map->enemies.front();
		map->enemies.pop();
		
		enemies[enemy_count] = new Enemy(map);
		enemies[enemy_count]->stats.pos.x = me.pos.x;
		enemies[enemy_count]->stats.pos.y = me.pos.y;
		enemies[enemy_count]->stats.direction = me.direction;
		enemies[enemy_count]->stats.load("enemies/" + me.type + ".txt");
		loadAssets(enemies[enemy_count]->stats.file_prefix);
		enemy_count++;
	}
}

/**
 * perform logic() for all enemies
 */
void EnemyManager::logic() {
	int pref_id;

	for (int i=0; i<enemy_count; i++) {
		enemies[i]->stats.hero_pos = heroPos;
		enemies[i]->logic();
		
		for (int j=0; j<file_prefix_count; j++) {
			if (file_prefixes[j] == enemies[i]->stats.file_prefix)
				pref_id = j;
		}
		
		// check sound effects
		if (enemies[i]->sfx_hit) Mix_PlayChannel(-1, sound_hit[pref_id], 0);
		if (enemies[i]->sfx_phys_melee) Mix_PlayChannel(-1, sound_phys_melee[pref_id], 0);
		if (enemies[i]->sfx_die) Mix_PlayChannel(-1, sound_die[pref_id], 0);		
		if (enemies[i]->sfx_critdie) Mix_PlayChannel(-1, sound_critdie[pref_id], 0);		
		
		// clear sound flags
		enemies[i]->sfx_hit = false;
		enemies[i]->sfx_phys_melee = false;
		enemies[i]->sfx_die = false;
		enemies[i]->sfx_critdie = false;
	}
}

/**
 * getRender()
 * Map objects need to be drawn in Z order, so we allow a parent object (GameEngine)
 * to collect all mobile sprites each frame.
 */
Renderable EnemyManager::getRender(int enemyIndex) {
	Renderable r = enemies[enemyIndex]->getRender();
	for (int i=0; i<file_prefix_count; i++) {
		if (file_prefixes[i] == enemies[enemyIndex]->stats.file_prefix)
			r.sprite = sprites[i];
	}
	return r;	
}

EnemyManager::~EnemyManager() {
	for (int i=0; i<enemy_count; i++) {
		delete(enemies[i]);
	}
	
	for (int i=0; i<file_prefix_count; i++) {
		SDL_FreeSurface(sprites[i]);
		Mix_FreeChunk(sound_phys_melee[i]);
		Mix_FreeChunk(sound_hit[i]);
		Mix_FreeChunk(sound_die[i]);
		Mix_FreeChunk(sound_critdie[i]);
	}

}

