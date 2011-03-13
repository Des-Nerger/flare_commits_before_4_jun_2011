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
	
	for (int i=0; i<4; i++)
		equipped[i] = 0;
		
	for (int i=0; i<64; i++)
		carried[i] = 0;
	
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;

	equipped_area.x = offset_x+32;
	equipped_area.y = offset_y+48;
	equipped_area.w = 256;
	equipped_area.h = 64;
	
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
	stats->gold = gold;
}

void MenuInventory::render() {
	if (!visible) return;
	SDL_Rect src;
	SDL_Rect dest;
	
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
	
	stringstream ss;
	ss << gold << " Gold";
	font->render(ss.str(), offset_x+288, offset_y+114, JUSTIFY_RIGHT, screen, FONT_WHITE);
	
	// equipped items
	for (int i=0; i<4; i++) {
		if (equipped[i] > 0) {
			dest.x = offset_x+32 + i * 64;
			dest.y = offset_y+48;
			items->renderIcon(equipped[i], dest.x, dest.y, ICON_SIZE_64);
		}
	}
	
	// carried items
	for (int i=0; i<64; i++) {
		if (carried[i] > 0) {
			dest.x = offset_x+32 + (i % 8 * 32);
			dest.y = offset_y+128 + (i / 8 * 32);
			items->renderIcon(carried[i], dest.x, dest.y, ICON_SIZE_32);
		}	
	}
}

/**
 * Click-start dragging in the inventory
 */
int MenuInventory::click(Point mouse) {

	// can't use items while dead
	if (stats->hp <= 0) return 0;

	int item;
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	
	if (isWithin(equipped_area, mouse)) {
	
		// clicked an equipped item
		drag_prev_slot = (mouse.x - (offset_x+32)) / 64;
		drag_prev_src = SRC_EQUIPPED;
		
		item = equipped[drag_prev_slot];
		equipped[drag_prev_slot] = 0;

		return item;
	}
	else if (isWithin(carried_area, mouse)) {
		// clicked a carried item
		drag_prev_slot = (mouse.x - (offset_x+32)) / 32 + ((mouse.y - (offset_y+128)) / 32) * 8;
		drag_prev_src = SRC_CARRIED;
		
		item = carried[drag_prev_slot];
		carried[drag_prev_slot] = 0;
		
		return item;
	}

	return 0;
}


/**
 * CTRL-click to sell an item
 */
void MenuInventory::sell(Point mouse) {

	// can't sell items while dead
	if (stats->hp <= 0) return;

	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	int value;
	int slot;
	
	if (isWithin(carried_area, mouse)) {
		// clicked a carried item
		slot = (mouse.x - (offset_x+32)) / 32 + ((mouse.y - (offset_y+128)) / 32) * 8;
		
		if (carried[slot] > 0) {
			if (items->items[carried[slot]].price > 0) {
				value = items->items[carried[slot]].price / items->vendor_ratio;
				if (value == 0) value = 1;
				gold += value;
				carried[slot] = 0;
				items->playCoinsSound();
			}
		}
	}

}

/**
 * Sell a specific item (called when dragging an item to the vendor screen)
 */
void MenuInventory::sell(int item_id) {
	int value = items->items[item_id].price / items->vendor_ratio;
	if (value == 0) value = 1;
	gold += value;
	items->playCoinsSound();
}

/**
 * Return dragged item to previous slot
 */
void MenuInventory::itemReturn(int item) {
	if (drag_prev_src == SRC_CARRIED)
		carried[drag_prev_slot] = item;
	else if (drag_prev_src == SRC_EQUIPPED)
		equipped[drag_prev_slot] = item;
}

/**
 * Check requirements on an item
 */
bool MenuInventory::requirementsMet(int item) {
	if (items->items[item].req_stat == REQUIRES_PHYS) {
		return (stats->physical >= items->items[item].req_val);
		
	}
	else if (items->items[item].req_stat == REQUIRES_MAG) {
		return (stats->magical >= items->items[item].req_val);
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
void MenuInventory::drop(Point mouse, int item) {
	items->playSound(item);

	int index;
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;

	if (isWithin(equipped_area, mouse)) {
	
		// dropped onto equipped item
		index = (mouse.x - (offset_x+32)) / 64;	

		if (drag_prev_src == SRC_CARRIED) {
			
			// make sure the item is going to the correct slot
			// note: equipment slots 0-3 correspond with item types 0-3
			// also check to see if the hero meets the requirements
			if (index == items->items[item].type && requirementsMet(item)) {
				carried[drag_prev_slot] = equipped[index];
				equipped[index] = item;
				if (index < 3) changed_equipment = true;
				else changed_artifact = true;
			}
			else {
				carried[drag_prev_slot] = item; // cancel
			}
		}
		else {
			// equippable items only belong to one slot
			equipped[drag_prev_slot] = item; // cancel
		}
	}
	else if (isWithin(carried_area, mouse)) {
	
		// dropped onto carried item
		index = (mouse.x - (offset_x+32)) / 32 + ((mouse.y - (offset_y+128)) / 32) * 8;
		
		if (drag_prev_src == SRC_CARRIED) {
			if (index != drag_prev_slot) {
				carried[drag_prev_slot] = carried[index];
				carried[index] = item;
			}
			else
				carried[drag_prev_slot] = item; // cancel
		}
		else {
		
		    // note: equipment slots 0-3 correspond with item types 0-3
			// also check to see if the hero meets the requirements
			if (carried[index] == 0 || (items->items[carried[index]].type == drag_prev_slot && requirementsMet(carried[index]))) {
				equipped[drag_prev_slot] = carried[index];
				carried[index] = item;
				if (drag_prev_slot < 3) changed_equipment = true;
				else changed_artifact = true;
			}
			else {
				equipped[drag_prev_slot] = item; // cancel
			}
		}
	}
	else {
	
		// not dropped in a slot.  Just return it to the previous slot
		itemReturn(item);
	}
 
}

/**
 * Right-clicking on a usable item in the inventory causes it to activate.
 * e.g. drink a potion
 * e.g. equip an item
 */
void MenuInventory::activate(Point mouse) {

	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	int slot;
	int swap;
	int equip_slot;
	Point nullpt;
	
	if (isWithin(carried_area, mouse)) {

		// clicked a carried item
		slot = (mouse.x - (offset_x+32)) / 32 + ((mouse.y - (offset_y+128)) / 32) * 8;
	
		// use a consumable item, but only if alive
		if (items->items[carried[slot]].type == ITEM_TYPE_CONSUMABLE && stats->alive) {
		
			powers->activate(items->items[carried[slot]].power, stats, nullpt);
			// intercept used_item flag.  We will destroy the item here.
			powers->used_item = -1;
			carried[slot] = 0;
			
		}
		// equip an item
		else if (items->items[carried[slot]].type == ITEM_TYPE_MAIN ||
		         items->items[carried[slot]].type == ITEM_TYPE_BODY ||
				 items->items[carried[slot]].type == ITEM_TYPE_OFF ||
				 items->items[carried[slot]].type == ITEM_TYPE_ARTIFACT) {
			if (requirementsMet(carried[slot])) {
				equip_slot = items->items[carried[slot]].type;
				swap = equipped[equip_slot];
				equipped[equip_slot] = carried[slot];
				carried[slot] = swap;
				items->playSound(equipped[equip_slot]);
			
				if (equip_slot < 3) changed_equipment = true;
				else changed_artifact = true;
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
	int index;
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	TooltipData tip;
	
	if (isWithin(equipped_area, mouse)) {
		
		// equipped item
		index = (mouse.x - (offset_x+32)) / 64;	
		return items->getTooltip(equipped[index], stats, false);
		
	}
	else if (isWithin(carried_area, mouse)) {
		
		// carried item
		index = (mouse.x - (offset_x+32)) / 32 + ((mouse.y - (offset_y+128)) / 32) * 8;
		return items->getTooltip(carried[index], stats, false);
		
	}

	if (mouse.x >= offset_x + 224 && mouse.y >= offset_y+96 && mouse.x < offset_x+288 && mouse.y < offset_y+128) {
		tip.lines[tip.num_lines++] = "ctrl-click a carried item to sell it";
	}
	
	return tip;
}

/**
 * Cannot pick up new items if the inventory is full.
 * Full means no more carrying capacity (equipped capacity is ignored)
 */
bool MenuInventory::full() {
	bool result = true;
	for (int i=0; i<64; i++) {
		if (carried[i] == 0) result = false;
	}
	return result;
}

/**
 * Insert item into first available carried slot
 *
 * @param item Item ID
 */
void MenuInventory::add(int item) {
	for (int i=0; i<64; i++) {
		if (carried[i] == 0) {
			carried[i] = item;
			items->playSound(item);
			return;
		}
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
	for (int i=0; i<64; i++) {
		if (carried[i] == item)
			count++;	
	}
	return count;
}

/**
 * Check to see if the given item is equipped
 */
bool MenuInventory::isItemEquipped(int item) {
	for (int i=0; i<4; i++) {
		if (equipped[i] == item)
			return true;
	}
	return false;
}

/**
 * Remove the given item from the player's inventory.
 */
void MenuInventory::remove(int item) {
	int i;
	for (i=0; i<64; i++) {
		if (carried[i] == item) {
			carried[i] = 0;
			return;
		}
	}
	for (i=0; i<4; i++) {
		if (equipped[i] == item) {
			equipped[i] = 0;
			if (i < SLOT_ARTIFACT) changed_equipment = true;
			if (i == SLOT_ARTIFACT) changed_artifact = true;
			return;
		}
	}
}

/**
 * Attempt to purchase item dragged to the inventory
 */
bool MenuInventory::purchase(Point mouse, int item_id) {

	if (full() || gold < items->items[item_id].price)
		return false;

	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	int slot;
	
	if (isWithin(carried_area, mouse)) {
		slot = (mouse.x - (offset_x+32)) / 32 + ((mouse.y - (offset_y+128)) / 32) * 8;
		if (carried[slot] == 0) {
			carried[slot] = item_id;
			items->playSound(item_id);
		}
		else add(item_id);
	}
	else add(item_id);
	
	gold -= items->items[item_id].price;
	items->playCoinsSound();
	
	return true;
}
		
MenuInventory::~MenuInventory() {
	SDL_FreeSurface(background);
}
