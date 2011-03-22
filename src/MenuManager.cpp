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
	powers = _powers;

	loadIcons();

	items = new ItemDatabase(screen, icons);
	inv = new MenuInventory(screen, font, items, stats, powers);
	pow = new MenuPowers(screen, font, stats, powers);
	chr = new MenuCharacter(screen, font, stats);
	log = new MenuLog(screen, font);
	act = new MenuActionBar(screen, font, inp, powers, icons);
	hpmp = new MenuHPMP(screen, font);
	tip = new MenuTooltip(font, screen);
	mini = new MenuMiniMap(screen);
	xp = new MenuExperience(screen, font);
	enemy = new MenuEnemy(screen, font);
	vendor = new MenuVendor(screen, font, items, stats);
	talker = new MenuTalker(screen, font);
	
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
	
	// optimize
	SDL_Surface *cleanup = icons;
	icons = SDL_DisplayFormatAlpha(icons);
	SDL_FreeSurface(cleanup);	
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

	bool clicking_character = false;
	bool clicking_inventory = false;
	bool clicking_powers = false;
	bool clicking_log = false;
	
	log->logic();
	enemy->logic();
	inv->logic();

	if (!inp->pressing[INVENTORY] && !inp->pressing[POWERS] && !inp->pressing[CHARACTER] && !inp->pressing[LOG])
		key_lock = false;
	
	// check if mouse-clicking a menu button
	act->checkMenu(inp->mouse, clicking_character, clicking_inventory, clicking_powers, clicking_log);

	// inventory menu toggle
	if ((inp->pressing[INVENTORY] && !key_lock && !dragging) || clicking_inventory) {
		key_lock = true;
		inv->visible = !inv->visible;
		if (inv->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			pow->visible = false;
		}
		else {
			Mix_PlayChannel(-1, sfx_close, 0);
			vendor->visible = false;
		}
		
	}

	// powers menu toggle
	if ((inp->pressing[POWERS] && !key_lock && !dragging) || clicking_powers) {
		key_lock = true;
		pow->visible = !pow->visible;
		if (pow->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			inv->visible = false;
			vendor->visible = false;
		}
		else
			Mix_PlayChannel(-1, sfx_close, 0);
	}

	// character menu toggle
	if ((inp->pressing[CHARACTER] && !key_lock && !dragging) || clicking_character) {
		key_lock = true;
		chr->visible = !chr->visible;
		if (chr->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			log->visible = false;
			vendor->visible = false;
			talker->visible = false;
		}
		else
			Mix_PlayChannel(-1, sfx_close, 0);
	}
	
	// log menu toggle
	if ((inp->pressing[LOG] && !key_lock && !dragging) || clicking_log) {
		key_lock = true;
		log->visible = !log->visible;
		if (log->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			chr->visible = false;
			vendor->visible = false;
			talker->visible = false;
		}
		else
			Mix_PlayChannel(-1, sfx_close, 0);
	}
		
	if (MENUS_PAUSE) {
		pause = (inv->visible || pow->visible || chr->visible || log->visible || vendor->visible || talker->visible);
	}
	menus_open = (inv->visible || pow->visible || chr->visible || log->visible || vendor->visible || talker->visible);
	
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;

	// handle right-click activate inventory item (must be alive)
	if (!dragging && inp->pressing[MAIN2] && !inp->mouse2_lock) {
		if (inp->mouse.x >= offset_x && inp->mouse.y >= offset_y && inp->mouse.y <= offset_y+416) {
			if (inv->visible) {

				inv->activate(inp->mouse);
				inp->mouse2_lock = true;
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
			else if (talker->visible) {
			
			}
			else if (vendor->visible) {
			
				if (inp->pressing[CTRL]) {
					inp->mouse_lock = true;
					
					// buy item from a vendor
					if (!inv->full()) {
						int bought_item;
						bought_item = vendor->buy(inp->mouse, inv->gold);
						if (bought_item != -1) {
							inv->add(bought_item);
						}
					}
				}
				else {
					
					// start dragging a vendor item
					drag_item = vendor->click(inp->mouse);
					if (drag_item > 0) {
						dragging = true;
						drag_src = DRAG_SRC_VENDOR;
						inp->mouse_lock=true;
					}
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
						inp->mouse_lock=true;
					}
				}
			}
			// pick up a power
			else if (pow->visible) {
				drag_power = pow->click(inp->mouse);
				if (drag_power > -1) {
					dragging = true;
					drag_src = DRAG_SRC_POWERS;
					inp->mouse_lock=true;
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
			// allow drag-to-rearrange action bar
			else if (!isWithin(act->menuArea, inp->mouse)) {
				drag_power = act->checkDrag(inp->mouse);
				if (drag_power > -1) {
					dragging = true;
					drag_src = DRAG_SRC_ACTIONBAR;
					inp->mouse_lock=true;
				}
			}
			
			// else, clicking action bar to use a power?
			// this check is done by GameEngine when calling Avatar::logic()


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
			else if (isWithin(act->numberArea,inp->mouse) || isWithin(act->mouseArea,inp->mouse)) {
				
				// The action bar is not storage!
				inv->itemReturn(drag_item);

				// put an item with a power on the action bar
				if (items->items[drag_item].power != -1) {
					act->drop(inp->mouse, items->items[drag_item].power, false);
				}
			
			}
			else if (vendor->visible && isWithin(vendor->slots_area, inp->mouse)) {
				
				// vendor sell item
				inv->sell(drag_item);
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
		
		else if (drag_src == DRAG_SRC_VENDOR) {

			if (inv->visible && inp->mouse.x >= offset_x && inp->mouse.y >= offset_y && inp->mouse.y <= offset_y+416) {
				inv->purchase(inp->mouse, drag_item);
				drag_item = 0;
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

	// for action-bar powers that represent items, lookup the current item count
	for (int i=0; i<12; i++) {
		act->slot_enabled[i] = true;
		act->slot_item_count[i] = -1;
		
		if (act->hotkeys[i] != -1) {
			int item_id = powers->powers[act->hotkeys[i]].requires_item;
			if (item_id != -1 && items->items[item_id].type == ITEM_TYPE_CONSUMABLE) {
				act->slot_item_count[i] = inv->getItemCountCarried(item_id);
				if (act->slot_item_count[i] == 0) {
					act->slot_enabled[i] = false;
				}
			}
			else if (item_id != -1) {
			
				// if a non-consumable item power is unequipped, disable that slot
				if (!inv->isItemEquipped(item_id)) {
					act->slot_enabled[i] = false;
				}
			}
		}
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
	vendor->render();
	talker->render();
	enemy->render();
	
	TooltipData tooltip;
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;

	// Find tooltips depending on mouse position	
	if (inp->mouse.x < 320 && inp->mouse.y >= offset_y && inp->mouse.y <= offset_y+416) {
		if (chr->visible) {
			tooltip = chr->checkTooltip(inp->mouse);
		}
		else if (vendor->visible) {
			tooltip = vendor->checkTooltip(inp->mouse);
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
		if (drag_src == DRAG_SRC_INVENTORY || drag_src == DRAG_SRC_VENDOR)
			items->renderIcon(drag_item, inp->mouse.x - 16, inp->mouse.y - 16, ICON_SIZE_32);
		else if (drag_src == DRAG_SRC_POWERS || drag_src == DRAG_SRC_ACTIONBAR)
			renderIcon(powers->powers[drag_power].icon, inp->mouse.x-16, inp->mouse.y-16);
	}
	
}

void MenuManager::closeAll() {
	if (!dragging) {
		inv->visible = false;
		pow->visible = false;
		chr->visible = false;
		log->visible = false;
		vendor->visible = false; 
		talker->visible = false;
		
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
	delete(vendor);
	delete(talker);
	
	Mix_FreeChunk(sfx_open);
	Mix_FreeChunk(sfx_close);
}
