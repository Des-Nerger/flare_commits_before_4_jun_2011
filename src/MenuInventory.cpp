/**
 * class MenuInventory
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuInventory.h"

MenuInventory::MenuInventory(SDL_Surface *_screen, FontEngine *_font, ItemDatabase *_items, StatBlock *_stats, PowerManager *_powers) {
	screen = _screen;
	font = _font;
	items = _items;
	stats = _stats;
	powers = _powers;
	
	visible = false;
	loadGraphics();

	inventory[EQUIPMENT] = new ItemStack[MAX_EQUIPPED];
	inventory[CARRIED] = new ItemStack[MAX_CARRIED];

	for( int i=0; i<MAX_EQUIPPED; i++) {
		inventory[EQUIPMENT][i].item = 0;
		inventory[EQUIPMENT][i].quantity = 0;
	}

	for( int i=0; i<MAX_CARRIED; i++) {
		inventory[CARRIED][i].item = 0;
		inventory[CARRIED][i].quantity = 0;
	}

	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;

	equipped_area.x = offset_x+32;
	equipped_area.y = offset_y+48;
	equipped_area.w = 256;
	equipped_area.h = MAX_CARRIED;
	
	carried_area.x = offset_x+32;
	carried_area.y = offset_y+128;
	carried_area.w = 256;
	carried_area.h = 256;
	
	gold = 0;
	
	drag_prev_slot = -1;
	changed_equipment = true;
	changed_artifact = true;
}

void MenuInventory::loadGraphics() {

	background = IMG_Load("images/menus/inventory.png");
	if(!background) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
	// optimize
	SDL_Surface *cleanup = background;
	background = SDL_DisplayFormatAlpha(background);
	SDL_FreeSurface(cleanup);	
}

void MenuInventory::logic() {
	
	// if the player has just died, the penalty is half his current gold.
	if (stats->death_penalty) {
		gold = gold/2;
		stats->death_penalty = false;
	}
	
	// a copy of gold is kept in stats, to help with various situations
	stats->gold = gold;
}

void MenuInventory::render() {
	if (!visible) return;
	SDL_Rect src;
	SDL_Rect dest;
	stringstream ss;
	
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	
	// background
	src.x = 0;
	src.y = 0;
	dest.x = offset_x;
	dest.y = offset_y;
	src.w = dest.w = 320;
	src.h = dest.h = 416;
	SDL_BlitSurface(background, &src, screen, &dest);
	
	
	// text overlay
	// TODO: translate()
	font->render("Inventory", offset_x+160, offset_y+8, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Main Hand", offset_x+64, offset_y+34, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Body", offset_x+128, offset_y+34, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Off Hand", offset_x+192, offset_y+34, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Artifact", offset_x+256, offset_y+34, JUSTIFY_CENTER, screen, FONT_WHITE);
	
	ss << gold << " Gold";
	font->render(ss.str(), offset_x+288, offset_y+114, JUSTIFY_RIGHT, screen, FONT_WHITE);
	
	// equipped items
	for (int i=0; i<MAX_EQUIPPED; i++) {
		if (inventory[EQUIPMENT][i].item > 0) {
			dest.x = offset_x+32 + i * 64;
			dest.y = offset_y+48;
			items->renderIcon(inventory[EQUIPMENT][i].item, dest.x, dest.y, ICON_SIZE_64);
			if( items->items[inventory[EQUIPMENT][i].item].max_quantity > 1) {
				// stackable item : show the quantity
				ss.str("");
				ss << inventory[EQUIPMENT][i].quantity;
				font->render(ss.str(), dest.x, dest.y, JUSTIFY_LEFT, screen, FONT_WHITE);
			}
		}
	}
	
	// carried items
	for (int i=0; i<MAX_CARRIED; i++) {
		if (inventory[CARRIED][i].item > 0) {
			dest.x = offset_x+32 + (i % 8 * 32);
			dest.y = offset_y+128 + (i / 8 * 32);
			items->renderIcon(inventory[CARRIED][i].item, dest.x, dest.y, ICON_SIZE_32);
			if( items->items[inventory[CARRIED][i].item].max_quantity > 1) {
				// stackable item : show the quantity
				ss.str("");
				ss << inventory[CARRIED][i].quantity;
				font->render(ss.str(), dest.x, dest.y, JUSTIFY_LEFT, screen, FONT_WHITE);
			}
		}	
	}
}

int MenuInventory::areaClicked(Point mouse) {
	if (isWithin(equipped_area, mouse)) {
		return EQUIPMENT;
	}
	else if (isWithin(carried_area, mouse)) {
		return CARRIED;
	}
	return -1;
}

int MenuInventory::slotClicked(int area, Point mouse) {
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;

	if (area == CARRIED) {
		return (mouse.x - (offset_x+32)) / 32 + ((mouse.y - (offset_y+128)) / 32) * 8;
	}
	else if (area == EQUIPMENT) {
		return (mouse.x - (offset_x+32)) / 64;
	}
	return -1;
}

/**
 * Click-start dragging in the inventory
 */
ItemStack MenuInventory::click(Point mouse) {
	ItemStack item;
	item.item = 0;
	item.quantity = 0;
	
	// can't use items while dead
	if (stats->hp <= 0) return item;

	drag_prev_src = areaClicked(mouse);
	drag_prev_slot = slotClicked( drag_prev_src, mouse);
	item = inventory[drag_prev_src][drag_prev_slot];
	substract( drag_prev_src, drag_prev_slot, item.quantity);

	return item;
}


/**
 * CTRL-click to sell an item
 */
void MenuInventory::sell(Point mouse) {
	int value;
	int area;
	int slot;

	// can't sell items while dead
	if (stats->hp <= 0) return;
	
	area = areaClicked( mouse);
	if (area == CARRIED) { // Add here where you want to be able to sell from
		slot = slotClicked( area, mouse);
		if (inventory[area][slot].item > 0) {
			if (items->items[inventory[area][slot].item].price > 0) {
				// Sell only one item from a stack
				value = items->items[inventory[area][slot].item].price / items->vendor_ratio;
				if (value == 0) value = 1;
				gold += value;
				substract( area, slot);
				items->playCoinsSound();
			}
		}
	}
}

/**
 * Sell a specific item (called when dragging an item to the vendor screen)
 */
void MenuInventory::sell(ItemStack stack) {
	int value = items->items[stack.item].price * stack.quantity / items->vendor_ratio;
	if (value == 0) value = 1;
	gold += value;
	items->playCoinsSound();
}

/**
 * Return dragged item to previous slot
 */
void MenuInventory::itemReturn( ItemStack stack) {
	inventory[drag_prev_src][drag_prev_slot] = stack;
}

/**
 * Check requirements on an item
 */
bool MenuInventory::requirementsMet(int item) {
	if (items->items[item].req_stat == REQUIRES_PHYS) {
		return (stats->physical >= items->items[item].req_val);
	}
	else if (items->items[item].req_stat == REQUIRES_MENT) {
		return (stats->mental >= items->items[item].req_val);
	}
	else if (items->items[item].req_stat == REQUIRES_OFF) {
		return (stats->offense >= items->items[item].req_val);
	}
	else if (items->items[item].req_stat == REQUIRES_DEF) {
		return (stats->defense >= items->items[item].req_val);
	}
	// otherwise there is no requirement, so it is usable.
	return true;
}

/**
 * Dragging and dropping an item can be used to rearrange the inventory
 * and equip items
 */
void MenuInventory::drop(Point mouse, ItemStack stack) {
	int area;
	int slot;

	items->playSound(stack.item);

	area = areaClicked( mouse);
	slot = slotClicked( area, mouse);

	if (area == EQUIPMENT) {
		// dropped onto equipped item

		if (drag_prev_src == CARRIED) {
			// make sure the item is going to the correct slot
			// note: equipment slots 0-3 correspond with item types 0-3
			// also check to see if the hero meets the requirements
			if (slot == items->items[stack.item].type && requirementsMet(stack.item)) {
				if( inventory[area][slot].item == stack.item && items->items[stack.item].max_quantity > 1) {
					// try to merge the stacks
					add( stack, area, slot);
				}
				else {
					// Swap the two stacks
					inventory[drag_prev_src][drag_prev_slot] = inventory[area][slot];
					inventory[area][slot] = stack;
				}
				update_equipment( slot);
			}
			else {
				itemReturn( stack); // cancel
			}
		}
		else {
			// equippable items only belong to one slot
			itemReturn( stack); // cancel
		}
	}
	else if (area == CARRIED) {
		// dropped onto carried item
		
		if (drag_prev_src == CARRIED) {
			if (slot != drag_prev_slot) {
				if( inventory[area][slot].item == stack.item && items->items[stack.item].max_quantity > 1) {
					// Try to merge the stacks
					add( stack, area, slot);
				}
				else {
					// Swap the two stacks
					inventory[drag_prev_src][drag_prev_slot] = inventory[area][slot];
					inventory[area][slot] = stack;
				}
			}
			else {
				itemReturn( stack); // cancel
			}
		}
		else {
		    // note: equipment slots 0-3 correspond with item types 0-3
			// also check to see if the hero meets the requirements
			if (inventory[area][slot].item == stack.item && items->items[stack.item].max_quantity > 1) {
				// Try to merge the stacks
				add( stack, area, slot);
			} else if (
				inventory[CARRIED][slot].item == 0
				|| (
					inventory[CARRIED][slot].item != inventory[EQUIPMENT][drag_prev_slot].item
					&& items->items[inventory[CARRIED][slot].item].type == drag_prev_slot
					&& requirementsMet(inventory[CARRIED][slot].item)
				)
			) { // The equipped item is dropped on an empty carried slot or on a wearable item
				// Swap the two stacks
				inventory[drag_prev_src][drag_prev_slot] = inventory[area][slot];
				inventory[area][slot] = stack;
				update_equipment( drag_prev_slot);
			}
			else {
				itemReturn( stack); // cancel
			}
		}
	}
	else {
		itemReturn( stack); // not dropped in a slot. Just return it to the previous slot
	}

	drag_prev_src = -1;
	drag_prev_slot = -1;
}

/**
 * Right-clicking on a usable item in the inventory causes it to activate.
 * e.g. drink a potion
 * e.g. equip an item
 */
void MenuInventory::activate(Point mouse) {
	int slot;
	int equip_slot;
	ItemStack stack;
	Point nullpt;
	nullpt.x = nullpt.y = 0;
	
	if (isWithin(carried_area, mouse)) {
		// clicked a carried item
		slot = slotClicked( CARRIED, mouse);
	
		// use a consumable item, but only if alive
		if (items->items[inventory[CARRIED][slot].item].type == ITEM_TYPE_CONSUMABLE && stats->alive) {
		
			powers->activate(items->items[inventory[CARRIED][slot].item].power, stats, nullpt);
			// intercept used_item flag.  We will destroy the item here.
			powers->used_item = -1;
			substract( CARRIED, slot);
			
		}
		// equip an item
		else if (items->items[inventory[CARRIED][slot].item].type == ITEM_TYPE_MAIN ||
		         items->items[inventory[CARRIED][slot].item].type == ITEM_TYPE_BODY ||
				 items->items[inventory[CARRIED][slot].item].type == ITEM_TYPE_OFF ||
				 items->items[inventory[CARRIED][slot].item].type == ITEM_TYPE_ARTIFACT) {
			if (requirementsMet(inventory[CARRIED][slot].item)) {
				equip_slot = items->items[inventory[CARRIED][slot].item].type;
				stack = inventory[CARRIED][slot];
				if( inventory[EQUIPMENT][equip_slot].item == inventory[CARRIED][slot].item) {
					// Try to merge the stacks
					substract( CARRIED, slot, stack.quantity);
					drag_prev_src = CARRIED;
					drag_prev_slot = slot;
					add( stack, EQUIPMENT, equip_slot);
					drag_prev_src = -1;
					drag_prev_slot = -1;
				}
				else {
					// Swap the two stacks
					inventory[CARRIED][slot] = inventory[EQUIPMENT][equip_slot];
					inventory[EQUIPMENT][equip_slot] = stack;
				}
				update_equipment( equip_slot);
				items->playSound(inventory[EQUIPMENT][equip_slot].item);
			}
		}
	}
}

/**
 * If mousing-over an item with a tooltip, return that tooltip data.
 *
 * @param mouse The x,y screen coordinates of the mouse cursor
 */
TooltipData MenuInventory::checkTooltip(Point mouse) {
	int area;
	int slot;
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	TooltipData tip;
	
	area = areaClicked( mouse);
	if( area == CARRIED || area == EQUIPMENT) {
		slot = slotClicked( area, mouse);
		tip = items->getTooltip(inventory[area][slot].item, stats, false);
	} else if (mouse.x >= offset_x + 224 && mouse.y >= offset_y+96 && mouse.x < offset_x+288 && mouse.y < offset_y+128) {
		tip.lines[tip.num_lines++] = "ctrl-click a carried item to sell it";
	}
	
	return tip;
}

/**
 * Cannot pick up new items if the inventory is full.
 * Full means no more carrying capacity (equipped capacity is ignored)
 *
 * TODO: handle stackable items
 */
bool MenuInventory::full() {
	bool result = true;
	for (int i=0; i<MAX_CARRIED; i++) {
		if (inventory[CARRIED][i].item == 0) result = false;
	}
	return result;
}

/**
 * Insert item into first available carried slot, preferably in the optionnal specified slot
 *
 * @param ItemStack Stack of items
 * @param area Area number where it will try to store the item
 * @param slot Slot number where it will try to store the item
 */
void MenuInventory::add(ItemStack stack, int area, int slot) {
	int max_quantity;
	int quantity_added;
	int i;

	items->playSound(stack.item);

	if( stack.item != 0) {
		if( area < 0) {
			area = CARRIED;
		}
		max_quantity = items->items[stack.item].max_quantity;
		if( slot > -1 && inventory[area][slot].item != 0 && inventory[area][slot].item != stack.item) {
			// the proposed slot isn't available, search for another one
			slot = -1;
		}
		if( area == CARRIED) {
			// first search of stack to complete if the item is stackable
			i = 0;
			while( max_quantity > 1 && slot == -1 && i < MAX_CARRIED) {
				if (inventory[area][i].item == stack.item && inventory[area][i].quantity < max_quantity) {
					slot = i;
				}
				i++;
			}
			// then an empty slot
			i = 0;
			while( slot == -1 && i < MAX_CARRIED) {
				if (inventory[area][i].item == 0) {
					slot = i;
				}
				i++;
			}
		}
		if( slot != -1) {
			// Add
			quantity_added = min( stack.quantity, max_quantity - inventory[area][slot].quantity);
			inventory[area][slot].item = stack.item;
			inventory[area][slot].quantity += quantity_added;
			stack.quantity -= quantity_added;
			// Add back the remaining
			if( stack.quantity > 0) {
				add( stack, drag_prev_src, drag_prev_slot);
			}
		}
		else {
			// No available slot, drop
			// TODO: We should drop on the floor an item we can't store
		}
	}
}

/**
 * Substract an item from the specified slot, or remove it if it's the last
 *
 * @param slot Slot number
 */
void MenuInventory::substract(int area, int slot, int quantity) {
	inventory[area][slot].quantity -= quantity;
	if (inventory[area][slot].quantity <= 0) {
		inventory[area][slot].item = 0;
	}
}

/**
 * Add gold to the current total
 */
void MenuInventory::addGold(int count) {
	gold += count;
	items->playCoinsSound();
}
 
/**
 * Get the number of the specified item carried (not equipped)
 */
int MenuInventory::getItemCountCarried(int item) {
	int count=0;
	for (int i=0; i<MAX_CARRIED; i++) {
		if (inventory[CARRIED][i].item == item) {
			count += inventory[CARRIED][i].quantity;
		}
	}
	return count;
}

/**
 * Check to see if the given item is equipped
 */
bool MenuInventory::isItemEquipped(int item) {
	for (int i=0; i<MAX_EQUIPPED; i++) {
		if (inventory[EQUIPMENT][i].item == item)
			return true;
	}
	return false;
}

/**
 * Remove one given item from the player's inventory.
 */
void MenuInventory::remove(int item) {
	int i;

	for (i=0; i<MAX_CARRIED; i++) {
		if (inventory[CARRIED][i].item == item) {
			substract(CARRIED, i, 1);
			return;
		}
	}
	for (i=0; i<MAX_EQUIPPED; i++) {
		if (inventory[EQUIPMENT][i].item == item) {
			substract(EQUIPMENT, i, 1);
			update_equipment( i);
			return;
		}
	}
}

/**
 * Attempt to purchase item dragged to the inventory
 */
bool MenuInventory::purchase(Point mouse, int item_id) {

	ItemStack item;
	item.item = item_id;
	item.quantity = 1;

	if (full() || gold < items->items[item_id].price)
		return false;
	
	if (isWithin(carried_area, mouse)) {
		add(item, CARRIED, slotClicked( CARRIED, mouse));
	}
	else {
		add(item, CARRIED);
	}
	
	gold -= items->items[item_id].price;
	items->playCoinsSound();
	
	return true;
}

void MenuInventory::update_equipment( int slot) {
	if (slot < SLOT_ARTIFACT) {
		changed_equipment = true;
	}
	else {
		changed_artifact = true;
	}
}

MenuInventory::~MenuInventory() {
	delete[] inventory[EQUIPMENT];
	delete[] inventory[CARRIED];
	SDL_FreeSurface(background);
}
