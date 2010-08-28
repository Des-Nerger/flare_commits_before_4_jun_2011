/**
 * class GameEngine
 *
 * Hands off the logic and rendering for the current game mode
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "SDL.h"
#include "SDL_image.h"
#include "InputState.h"
#include "Avatar.h"
#include "Enemy.h"
#include "MapIso.h"
#include "Utils.h"
#include "HazardManager.h"
#include "EnemyManager.h"
#include "FontEngine.h"
#include "MenuManager.h"
#include "LootManager.h"

class GameEngine {
private:
	SDL_Surface *screen;
	
	InputState *inp;
	Avatar *pc;
	MapIso *map;
	Renderable r[1024];
	int renderableCount;
	HazardManager *hazards;
	EnemyManager *enemies;
	FontEngine *font;
	MenuManager *menu;
	LootManager *loot;
	
	bool cancel_lock;
	
public:
	bool done;
	GameEngine(SDL_Surface *screen, InputState *inp);
	~GameEngine();
	
	void logic();
	void render();
};

#endif
