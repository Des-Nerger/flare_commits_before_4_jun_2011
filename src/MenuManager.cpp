/**
 * class MenuManager
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuManager.h"

MenuManager::MenuManager(PowerManager *_powers, SDL_Surface *_screen, InputState *_inp, FontEngine *_font, StatBlock *_stats) {
	powers = _powers;
	screen = _screen;
	inp = _inp;
	font = _font;
	stats = _stats;

	loadIcons();

	items = new ItemDatabase(screen, icons);
	inv = new MenuInventory(screen, font, items, stats);
	pow = new MenuPowers(screen, font, stats, powers);
	chr = new MenuCharacter(screen, font, stats);
	log = new MenuLog(screen, font);
	act = new MenuActionBar(powers, screen, inp, icons);
	hpmp = new MenuHealthMana(screen, font);
	tip = new MenuTooltip(font, screen);
	mini = new MenuMiniMap(screen);
	xp = new MenuExperience(screen, font);

	
	pause = false;
	dragging = false;
	drag_item = 0;
	drag_power = -1;
	drag_src = 0;
	drop_item = 0;
	
	loadSounds();

}

/**
 * Icon set shared throughout the game
 */
void MenuManager::loadIcons() {
	
	icons = IMG_Load("images/icons/icons.png");
	if(!icons) {
		fprintf(stderr, "Couldn't load icons: %s\n", IMG_GetError());
		SDL_Quit();
	}
}

void MenuManager::loadSounds() {
	sfx_open = Mix_LoadWAV("soundfx/inventory/inventory_page.ogg");
	sfx_close = Mix_LoadWAV("soundfx/inventory/inventory_book.ogg");
	
	if (!sfx_open || !sfx_close) {
		fprintf(stderr, "Mix_LoadWAV: %s\n", Mix_GetError());
		SDL_Quit();
	}	
}


void MenuManager::renderIcon(int icon_id, int x, int y) {
	SDL_Rect src;
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	src.w = src.h = dest.w = dest.h = 32;
	src.x = (icon_id % 16) * 32;
	src.y = (icon_id / 16) * 32;
	SDL_BlitSurface(icons, &src, screen, &dest);		
}

void MenuManager::logic() {

	log->logic();

	if (!inp->pressing[INVENTORY] && !inp->pressing[POWERS] && !inp->pressing[CHARACTER] && !inp->pressing[LOG])
		key_lock = false;

	if (!inp->pressing[MAIN2])
		rightclick_lock = false;

	// inventory menu toggle
	if (inp->pressing[INVENTORY] && !key_lock && !dragging) {
		key_lock = true;
		inv->visible = !inv->visible;
		if (inv->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			pow->visible = false;
		}
		else
			Mix_PlayChannel(-1, sfx_close, 0);
		
	}

	// powers menu toggle
	if (inp->pressing[POWERS] && !key_lock && !dragging) {
		key_lock = true;
		pow->visible = !pow->visible;
		if (pow->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			inv->visible = false;
		}
		else
			Mix_PlayChannel(-1, sfx_close, 0);
	}

	// character menu toggle
	if (inp->pressing[CHARACTER] && !key_lock && !dragging) {
		key_lock = true;
		chr->visible = !chr->visible;
		if (chr->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			log->visible = false;
		}
		else
			Mix_PlayChannel(-1, sfx_close, 0);
	}
	
	// log menu toggle
	if (inp->pressing[LOG] && !key_lock && !dragging) {
		key_lock = true;
		log->visible = !log->visible;
		if (log->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			chr->visible = false;
		}
		else
			Mix_PlayChannel(-1, sfx_close, 0);
	}
	
	pause = (inv->visible || pow->visible || chr->visible || log->visible);
	
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	
	// handle right-click activate inventory item (must be alive)
	if (!dragging && inp->pressing[MAIN2] && !rightclick_lock) {
		if (inp->mouse.x >= offset_x && inp->mouse.y >= offset_y && inp->mouse.y <= offset_y+416) {
			if (inv->visible) {
				inv->activate(inp->mouse);
				rightclick_lock = true;
			}
		}	
	}
	
	// handle left-click
	if (!dragging && inp->pressing[MAIN1] && !inp->mouse_lock) {
	
		// left side menu
		if (inp->mouse.x <= 320 && inp->mouse.y >= offset_y && inp->mouse.y <= offset_y+416) {
			if (chr->visible) {
			
				// applied a level-up
				if (chr->checkUpgrade(inp->mouse)) {
					inp->mouse_lock = true;
					// apply equipment and max hp/mp
					items->applyEquipment(stats, inv->equipped);
					stats->hp = stats->maxhp;
					stats->mp = stats->maxmp;
				}
			}
		}
	
		// right side menu
		else if (inp->mouse.x >= offset_x && inp->mouse.y >= offset_y && inp->mouse.y <= offset_y+416) {
		
			// pick up an inventory item
			if (inv->visible) {
			
				if (inp->pressing[CTRL]) {
					inp->mouse_lock = true;
					inv->sell(inp->mouse);
				}
				else {
					drag_item = inv->click(inp->mouse);
					if (drag_item > 0) {
						dragging = true;
						drag_src = DRAG_SRC_INVENTORY;
					}
				}
			}
			// pick up a power
			else if (pow->visible) {
				drag_power = pow->click(inp->mouse);
				if (drag_power > -1) {
					dragging = true;
					drag_src = DRAG_SRC_POWERS;
				}
			}
		}
		// action bar
		else if (isWithin(act->numberArea,inp->mouse) || isWithin(act->mouseArea,inp->mouse) || isWithin(act->menuArea, inp->mouse)) {
		
			// ctrl-click action bar to clear that slot
			if (inp->pressing[CTRL]) {
				act->remove(inp->mouse);
				inp->mouse_lock = true;
			}
			// if any menu is open, allow drag-to-rearrange action bar
			else if (pause && !isWithin(act->menuArea, inp->mouse)) {
				drag_power = act->checkDrag(inp->mouse);
				if (drag_power > -1) {
					dragging = true;
					drag_src = DRAG_SRC_ACTIONBAR;
				}
			}
			// click action bar to use that power
			else {
				act->checkAction(inp->mouse);
				inp->mouse_lock = true;
			}
		}
	}
	// handle dropping
	if (dragging && !inp->pressing[MAIN1]) {
		
		// putting a power on the Action Bar
		if (drag_src == DRAG_SRC_POWERS) {
			if (isWithin(act->numberArea,inp->mouse) || isWithin(act->mouseArea,inp->mouse)) {
				act->drop(inp->mouse, drag_power, 0);
			}
		}
		
		// rearranging the action bar
		else if (drag_src == DRAG_SRC_ACTIONBAR) {
			if (isWithin(act->numberArea,inp->mouse) || isWithin(act->mouseArea,inp->mouse)) {
				act->drop(inp->mouse, drag_power, 1);
			}
		}
	
		// rearranging inventory or dropping items
		else if (drag_src == DRAG_SRC_INVENTORY) {
			if (inv->visible && inp->mouse.x >= offset_x && inp->mouse.y >= offset_y && inp->mouse.y <= offset_y+416) {
				inv->drop(inp->mouse, drag_item);
				drag_item = 0;
			}
			else if (stats->hp > 0) {
				// if dragging and the source was inventory, drop item to the floor
				drop_item = drag_item;
				drag_item = 0;
			
				// if dropping equipment, prepare to change stats/sprites
				if (inv->drag_prev_src == SRC_EQUIPPED) {
					if (inv->drag_prev_slot < 3)
						inv->changed_equipment = true;
					else
						inv->changed_artifact = true;
				}
			}
			else { // prevent dropping items while dead
				inv->itemReturn(drag_item);
			}
		}

		dragging = false;
	}
	
	// handle equipment changes affecting hero stats
	if (inv->changed_equipment || inv->changed_artifact) {
		items->applyEquipment(stats, inv->equipped);
		inv->changed_artifact = false;
		// the equipment flag is reset after the new sprites are loaded
	}
}

void MenuManager::render() {
	hpmp->render(stats, inp->mouse);
	xp->render(stats, inp->mouse);
	act->render();
	inv->render();
	pow->render();
	chr->render();
	log->render();
	
	TooltipData tooltip;
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;

	// Find tooltips depending on mouse position	
	if (inp->mouse.x < 320 && inp->mouse.y >= offset_y && inp->mouse.y <= offset_y+416) {
		if (chr->visible) {
			tooltip = chr->checkTooltip(inp->mouse);
		}
	}
	else if (inp->mouse.x >= offset_x && inp->mouse.y >= offset_y && inp->mouse.y <= offset_y+416) {
		if (pow->visible) {
			tooltip = pow->checkTooltip(inp->mouse);
		}
		else if (inv->visible && !dragging) {
			tooltip = inv->checkTooltip(inp->mouse);
		}
	}
	else if (inp->mouse.y >= VIEW_H-32) {
		tooltip = act->checkTooltip(inp->mouse);
	}
	
	if (tooltip.num_lines > 0) {
		tip->render(tooltip, inp->mouse, STYLE_FLOAT);
	}
	
	// draw icon under cursor if dragging
	if (dragging) {
		if (drag_src == DRAG_SRC_INVENTORY)
			items->renderIcon(drag_item, inp->mouse.x - 16, inp->mouse.y - 16, ICON_SIZE_32);
		else if (drag_src == DRAG_SRC_POWERS || drag_src == DRAG_SRC_ACTIONBAR)
			renderIcon(drag_power, inp->mouse.x-16, inp->mouse.y-16);
	}
	
}

void MenuManager::closeAll() {
	if (!dragging) {
		inv->visible = false;
		pow->visible = false;
		chr->visible = false;
		log->visible = false;
		
		Mix_PlayChannel(-1, sfx_close, 0);
	}
}

MenuManager::~MenuManager() {
	delete(xp);
	delete(mini);
	delete(items);
	delete(inv);
	delete(pow);
	delete(chr);
	delete(log);
	delete(act);
	delete(tip);
	Mix_FreeChunk(sfx_open);
	Mix_FreeChunk(sfx_close);
}
