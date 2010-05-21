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
	
	handleNewMap();
	loadGraphics();
	loadSounds();
}

/**
 * Enemies share graphic/sound resources (usually there are groups of similar enemies)
 */
void EnemyManager::loadGraphics() {
	
	sprites = IMG_Load("images/zombie.png");
	if(!sprites) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
	}
}

void EnemyManager::loadSounds() {
	sound_attack = Mix_LoadWAV("soundfx/zombie_attack.ogg");
	sound_hit = Mix_LoadWAV("soundfx/zombie_hit.ogg");
	sound_die = Mix_LoadWAV("soundfx/zombie_die.ogg");
	sound_critdie = Mix_LoadWAV("soundfx/zombie_critdie.ogg");
	
	if (!sound_attack || !sound_hit || !sound_die || !sound_critdie) {
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
		SDL_Quit();
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
	
	while (!map->enemies.empty()) {
		me = map->enemies.front();
		map->enemies.pop();
		enemies[enemy_count] = new Enemy(map);
		enemies[enemy_count]->pos.x = me.pos.x;
		enemies[enemy_count]->pos.y = me.pos.y;
		enemies[enemy_count]->direction = me.direction;
		
		enemy_count++;
	}
}

/**
 * perform logic() for all enemies
 */
void EnemyManager::logic() {
	for (int i=0; i<enemy_count; i++) {
		enemies[i]->heroPos = heroPos;
		enemies[i]->logic();
		
		// check sound effects
		if (enemies[i]->sfx_hit) Mix_PlayChannel(-1, sound_hit, 0);
		if (enemies[i]->sfx_attack) Mix_PlayChannel(-1, sound_attack, 0);
		if (enemies[i]->sfx_die) Mix_PlayChannel(-1, sound_die, 0);		
		if (enemies[i]->sfx_critdie) Mix_PlayChannel(-1, sound_critdie, 0);		
		
		// clear sound flags
		enemies[i]->sfx_hit = false;
		enemies[i]->sfx_attack = false;
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
	r.sprite = sprites;
	return r;	
}

EnemyManager::~EnemyManager() {
	for (int i=0; i<enemy_count; i++) {
		delete(enemies[i]);
	}
	SDL_FreeSurface(sprites);
	Mix_FreeChunk(sound_attack);
	Mix_FreeChunk(sound_hit);
	Mix_FreeChunk(sound_die);
	Mix_FreeChunk(sound_critdie);
}

