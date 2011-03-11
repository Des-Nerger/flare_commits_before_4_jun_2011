/**
 * class GameEngine
 *
 * Hands off the logic and rendering for the current game mode.
 * Also handles message passing between child objects, often to avoid circular dependencies.
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "GameEngine.h"

GameEngine::GameEngine(SDL_Surface *_screen, InputState *_inp) {
	screen = _screen;
	inp = _inp;
	done = false;

	powers = new PowerManager();
	font = new FontEngine();	
	map = new MapIso(_screen);
	pc = new Avatar(powers, _inp, map);
	enemies = new EnemyManager(powers, map);
	hazards = new HazardManager(powers, pc, enemies);
	menu = new MenuManager(powers, _screen, _inp, font, &pc->stats);
	loot = new LootManager(menu->items, menu->tip, enemies, map);
		
	cancel_lock = false;
	loadGame();
}

/**
 * Check mouseover for enemies.
 * class variable "enemy" contains a live enemy on mouseover.
 * This function also sets enemy mouseover for Menu Enemy.
 */
void GameEngine::checkEnemyFocus() {
	// determine enemies mouseover
	// only check alive enemies for targeting
	enemy = enemies->enemyFocus(inp->mouse, map->cam, true);
	
	if (enemy != NULL) {
	
		// if there's a living creature in focus, display its stats
		menu->enemy->enemy = enemy;
		menu->enemy->timeout = MENU_ENEMY_TIMEOUT;
	}
	else {
		
		// if there's no living creature in focus, look for a dead one instead
		Enemy *temp_enemy = enemies->enemyFocus(inp->mouse, map->cam, false);
		if (temp_enemy != NULL) {
			menu->enemy->enemy = temp_enemy;
			menu->enemy->timeout = MENU_ENEMY_TIMEOUT;
		}
	}
}

/**
 * If mouse_move is enabled, and the mouse is over a live enemy,
 * Do not allow power use with button MAIN1
 */
bool GameEngine::restrictPowerUse() {
	if(pc->stats.mouse_move) {
		if(enemy == NULL && inp->pressing[MAIN1] && !inp->pressing[SHIFT] && !(isWithin(menu->act->numberArea,inp->mouse) || isWithin(menu->act->mouseArea,inp->mouse) || isWithin(menu->act->menuArea, inp->mouse))) {
			return true;
		}
	}
	return false;
}

/**
 * Check to see if the player is picking up loot on the ground
 */
void GameEngine::checkLoot() {
	if (inp->pressing[MAIN1] && !inp->mouse_lock && pc->stats.alive) {

		int pickup;
		int gold;
		
		pickup = loot->checkPickup(inp->mouse, map->cam, pc->stats.pos, gold, menu->inv->full());
		if (pickup > 0) {
			inp->mouse_lock = true;
			menu->inv->add(pickup);
		}
		else if (gold > 0) {
			inp->mouse_lock = true;
			menu->inv->addGold(gold);
		}
		if (loot->full_msg) {
			inp->mouse_lock = true;
			menu->log->add("Inventory is full.");
			loot->full_msg = false;
		}
	}
}

void GameEngine::checkTeleport() {
	if (map->teleportation || pc->stats.teleportation) {
		
		if (map->teleportation) {
			map->cam.x = pc->stats.pos.x = map->teleport_destination.x;
			map->cam.y = pc->stats.pos.y = map->teleport_destination.y;
		}
		else {
			map->cam.x = pc->stats.pos.x = pc->stats.teleport_destination.x;
			map->cam.y = pc->stats.pos.y = pc->stats.teleport_destination.y;		
		}
		
		// process intermap teleport
		if (map->teleportation && map->teleport_mapname != "") {
			map->load(map->teleport_mapname);
			enemies->handleNewMap();
			hazards->handleNewMap(&map->collider);
			loot->handleNewMap();
			powers->handleNewMap(&map->collider);
			menu->enemy->handleNewMap();
			
			// store this as the new respawn point
			map->respawn_map = map->teleport_mapname;
			map->respawn_point.x = pc->stats.pos.x;
			map->respawn_point.y = pc->stats.pos.y;
		}

		map->teleportation = false;
		pc->stats.teleportation = false; // teleport spell
		
	}
}

/**
 * Check for cancel key to exit menus or exit the game.
 * Also check closing the game window entirely.
 */
void GameEngine::checkCancel() {
	if (!inp->pressing[CANCEL]) cancel_lock = false;
	else if (inp->pressing[CANCEL] && !cancel_lock) {
		cancel_lock = true;
		if (menu->menus_open) {
			menu->closeAll();
		}
		else {
			saveGame();
			done = true;
		}
	}

	// if user closes the window
	if (inp->done) {
		saveGame();
		done = true;
	}	
}

/**
 * Check for log messages from various child objects
 */
void GameEngine::checkLog() {
	if (map->log_msg != "") {
		menu->log->add(map->log_msg);
		map->log_msg = "";
	}
	if (pc->log_msg != "") {
		menu->log->add(pc->log_msg);
		pc->log_msg = "";
	}
}

void GameEngine::checkEquipmentChange() {
	if (menu->inv->changed_equipment) {
		pc->loadGraphics(menu->items->items[menu->inv->equipped[0]].gfx, 
		                 menu->items->items[menu->inv->equipped[1]].gfx, 
		                 menu->items->items[menu->inv->equipped[2]].gfx);
		menu->inv->changed_equipment = false;
	}
}

void GameEngine::checkLootDrop() {
	// if the player has dropped an item from the inventory
	if (menu->drop_item > 0) {
		loot->addLoot(menu->drop_item, pc->stats.pos);
		menu->drop_item = 0;
	}
}

/**
 * When a consumable-based power is used, we need to remove it from the inventory.
 */
void GameEngine::checkConsumable() {
	if (powers->used_item != -1) {
		if (menu->items->items[powers->used_item].type == ITEM_TYPE_CONSUMABLE) {
			menu->inv->remove(powers->used_item);
			powers->used_item = -1;
		}
	}
}

/**
 * Process all actions for a single frame
 * This includes some message passing between child object
 */
void GameEngine::logic() {
	
	if (!inp->pressing[MAIN1]) inp->mouse_lock = false;
	if (!inp->pressing[MAIN2]) inp->mouse2_lock = false;

	// check menus first (top layer gets mouse click priority)
	menu->logic();
	
	if (!menu->pause) {
	
		// these actions only occur when the game isn't paused		
		checkLoot();
		checkEnemyFocus();
		pc->logic(menu->act->checkAction(inp->mouse), restrictPowerUse());
		
		enemies->heroPos = pc->stats.pos;
		enemies->logic();
		hazards->logic();
		loot->logic();
		enemies->checkEnemiesforXP(&pc->stats);
		
	}
	
	// these actions occur whether the game is paused or not.
	checkLootDrop();
	checkTeleport();
	checkLog();
	checkEquipmentChange();
	checkConsumable();
	map->logic();

	checkCancel();
	
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
		if (enemies->enemies[i]->stats.shield_hp > 0) {
			r[renderableCount] = enemies->enemies[i]->stats.getEffectRender(STAT_EFFECT_SHIELD);
			r[renderableCount++].sprite = powers->gfx[powers->powers[POWER_SHIELD].gfx_index]; // TODO: parameter
		}
	}
	
	for (int i=0; i<loot->loot_count; i++) { // Loot
		r[renderableCount++] = loot->getRender(i);
	}
	
	for (int i=0; i<hazards->hazard_count; i++) { // Hazards
		if (hazards->h[i]->rendered && hazards->h[i]->delay_frames == 0) {
			r[renderableCount++] = hazards->getRender(i);
		}
	}
	
	// get additional hero overlays
	if (pc->stats.shield_hp > 0) {
		r[renderableCount] = pc->stats.getEffectRender(STAT_EFFECT_SHIELD);
		r[renderableCount++].sprite = powers->gfx[powers->powers[POWER_SHIELD].gfx_index]; // TODO: parameter
	}
	if (pc->stats.vengeance_stacks > 0) {
		r[renderableCount] = pc->stats.getEffectRender(STAT_EFFECT_VENGEANCE);
		r[renderableCount++].sprite = powers->runes;		
	}
		
	sort_by_tile(r,renderableCount);

	// render the static map layers plus the renderables
	map->render(r, renderableCount);
	
	// display the name of the map in the upper-right hand corner
	font->render(map->title, VIEW_W-2, 2, JUSTIFY_RIGHT, screen, FONT_WHITE);
	
	// mouseover loot tooltips
	loot->renderTooltips(map->cam);
	
	menu->log->renderHUDMessages();
	menu->mini->render(&map->collider, pc->stats.pos, map->w, map->h);
	menu->render();

}

void GameEngine::showFPS(int fps) {
	stringstream ss;
	ss << fps << "fps";
	font->render(ss.str(), VIEW_W >> 1, 2, JUSTIFY_CENTER, screen, FONT_GRAY); 
}

GameEngine::~GameEngine() {
	delete(hazards);
	delete(enemies);
	delete(pc);
	delete(map);
	delete(font);
	delete(menu);
	delete(loot);
	delete(powers);
}
