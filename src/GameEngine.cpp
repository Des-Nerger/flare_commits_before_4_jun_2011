/**
 * class GameEngine
 *
 * Hands off the logic and rendering for the current game mode
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "GameEngine.h"

GameEngine::GameEngine(SDL_Surface *_screen, InputState *_inp) {
	screen = _screen;
	inp = _inp;
	done = false;

	font = new FontEngine();	
	map = new MapIso(_screen);
	pc = new Avatar(_inp, map);
	enemies = new EnemyManager(map);
	hazards = new HazardManager(pc, enemies);
	menu = new MenuManager(_screen, _inp, font, &pc->stats);
	
	cancel_lock = false;

}

/**
 * Process all actions for a single frame
 */
void GameEngine::logic() {
	
	
	if (!menu->pause) {
		pc->logic();
		enemies->heroPos = pc->pos;
		enemies->logic();
		hazards->logic();
	}
	
	// check teleport
	if (map->teleportation) {
		map->teleportation = false;

		map->cam.x = pc->pos.x = map->teleport_desination.x;
		map->cam.y = pc->pos.y = map->teleport_desination.y;
		
		// process intermap teleport
		if (map->teleport_mapname != "") {
			map->load(map->teleport_mapname);
			enemies->handleNewMap();
			hazards->handleNewMap();
			
		}
	}
	
	// handle cancel key
	if (!inp->pressing[CANCEL]) cancel_lock = false;
	else if (inp->pressing[CANCEL] && !cancel_lock) {
		cancel_lock = true;
		if (menu->pause) {
			menu->closeAll();
		}
		else {
			done = true;
		}
	}
	
	// if user closes the window
	if (inp->done) done = true;
	
	menu->logic();
	
	
}

/**
 * Render all graphics for a single frame
 */
void GameEngine::render() {

	// The strategy here is to make a list of Renderables from all objects not already on the map.
	// Pass this list to the map, which will draw them in z-order with the map object layer
	renderableCount = 1 + enemies->enemy_count;
	
	r[0] = pc->getRender();
	for (int i=1; i<= enemies->enemy_count; i++) {
		r[i] = enemies->getRender(i-1);
	}
	
	zsort(r,renderableCount);
	map->render(r, renderableCount);
	
	// display the name of the map in the upper-right hand corner
	font->render(map->title, 638, 2, JUSTIFY_RIGHT, screen);
	
	//font->render("This is a test of the word wrap and newline system, overly long so we really get a good test in.\nsingle line\n\nTest again\n\nfinally.", 256, 32, JUSTIFY_RIGHT, screen, 256);
	
	menu->render();
}

GameEngine::~GameEngine() {
	delete(hazards);
	delete(enemies);
	delete(pc);
	delete(map);
	delete(font);
	delete(menu);
}
