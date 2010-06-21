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
	loot = new LootManager(menu->items, menu->tip);
	
	cancel_lock = false;

}

/**
 * Mouse clicks are context sensitive depending on the click location
 * and current game state.  E.g. clicking on loot should pick up loot
 * instead of attacking that direction
 */
void GameEngine::assignMouseClick() {

	// there should be a click state and mouse lock
	// e.g. attacking a creature and holding the button should keep
	// attacking, even if you move the mouse over loot or buttons
	// so the mouse state is only assigned on a new click
	// and upon mouse release the lock and state are cleared
	
}


/**
 * Process all actions for a single frame
 * This includes some message passing between child object
 */
void GameEngine::logic() {
	
	// the game action is paused if any menus are opened
	if (!menu->pause) {
		pc->logic();
		enemies->heroPos = pc->pos;
		enemies->logic();
		hazards->logic();
		loot->logic();
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
	
	// check change equipment
	if (menu->inv->changed_equipment) {
		pc->loadGraphics(menu->items->items[menu->inv->equipped[0]].gfx, 
		                 menu->items->items[menu->inv->equipped[1]].gfx, 
		                 menu->items->items[menu->inv->equipped[2]].gfx);
		menu->inv->changed_equipment = false;
	}
	
}

/**
 * Render all graphics for a single frame
 */
void GameEngine::render() {

	// Create a list of Renderables from all objects not already on the map.
	renderableCount = 0;

	r[renderableCount++] = pc->getRender(); // Avatar
	
	for (int i=0; i<enemies->enemy_count; i++) { // Enemies
		r[renderableCount++] = enemies->getRender(i);
	}
	
	for (int i=0; i<loot->loot_count; i++) { // Loot
		r[renderableCount++] = loot->getRender(i);
	}
	
	for (int i=0; i<hazards->hazard_count; i++) { // Hazards
		// TODO: hazard effects renderables
		// Not all hazards have a renderable component
	}
		
	zsort(r,renderableCount);

	// render the static map layers plus the renderables
	map->render(r, renderableCount);
	
	// display the name of the map in the upper-right hand corner
	font->render(map->title, VIEW_W-2, 2, JUSTIFY_RIGHT, screen, FONT_WHITE);
	
	// mouseover loot tooltips
	loot->renderTooltips(map->cam);
	
	menu->render();
}

GameEngine::~GameEngine() {
	delete(hazards);
	delete(enemies);
	delete(pc);
	delete(map);
	delete(font);
	delete(menu);
	delete(loot);
}
