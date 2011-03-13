/**
 * class MenuVendor
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuVendor.h"

MenuVendor::MenuVendor(SDL_Surface *_screen, FontEngine *_font, ItemDatabase *_items, StatBlock *_stats) {
	screen = _screen;
	font = _font;
	items = _items;
	stats = _stats;
	
	int offset_y = (VIEW_H - 416)/2;
	
	for (int i=0; i<VENDOR_SLOTS; i++) {

		// empty the inventory
		stock[i] = -1;
		
		// set slot width and height
		slots[i].w = 32;
		slots[i].h = 32;
		
		// set slot position.  TODO: put this in a menu config file.
		slots[i].x = (i%8) * 32 + 32;
		slots[i].y = (i/8) * 32 + offset_y + 64;
	}
	
	slots_area.x = 32;
	slots_area.y = offset_y + 64;
	slots_area.w = 256;
	slots_area.h = 320;
	
	visible = false;
	loadGraphics();
	loadMerchant("");
	
}

void MenuVendor::loadGraphics() {

	background = IMG_Load("images/menus/vendor.png");
	if(!background) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
	// optimize
	SDL_Surface *cleanup = background;
	background = SDL_DisplayFormatAlpha(background);
	SDL_FreeSurface(cleanup);	
	
}

void MenuVendor::loadMerchant(string filename) {
	// TEMP: hardcoded items
	stock[0] = 1; // health potion
	stock[1] = 2; // mana potion
	stock[2] = 100; // dagger
	stock[3] = 300; // wand
	stock[4] = 500; // slingshot
	stock[5] = 700; // leather armor
}


void MenuVendor::logic() {

}

void MenuVendor::render() {
	if (!visible) return;
	SDL_Rect src;
	SDL_Rect dest;
	
	int offset_y = (VIEW_H - 416)/2;
	
	// background
	src.x = 0;
	src.y = 0;
	dest.x = 0;
	dest.y = offset_y;
	src.w = dest.w = 320;
	src.h = dest.h = 416;
	SDL_BlitSurface(background, &src, screen, &dest);
		
	// text overlay
	// TODO: translate()
	font->render("Vendor", 160, offset_y+8, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render(npc->name, 160, offset_y+24, JUSTIFY_CENTER, screen, FONT_WHITE);
	
	// show stock
	for (int i=0; i<VENDOR_SLOTS; i++) {
		if (stock[i] != -1) {
			items->renderIcon(stock[i], slots[i].x, slots[i].y, ICON_SIZE_32);
		}
	}

}

/**
 * Buy vendor item under cursor if enough gold.
 * Subtracts item price from gold.
 *
 * Called on CTRL-click
 *
 * @param mouse The cursor location
 * @param gold The player's current gold.  Passed by reference.
 * @return ID of the item bought, or -1 if no item.
 */
int MenuVendor::buy(Point mouse, int &gold) {
	for (int i=0; i<VENDOR_SLOTS; i++) {
		if (isWithin(slots[i], mouse)) {
			if (stock[i] != -1) {
				if (items->items[stock[i]].price <= gold) {
					gold -= items->items[stock[i]].price;
					items->playCoinsSound();
					return stock[i];
				}
			}
		}
	}
	return -1;
}

/**
 * Start dragging a vendor item
 * Players can drag an item to their inventory to purchase.
 */
int MenuVendor::click(Point mouse) {
	for (int i=0; i<VENDOR_SLOTS; i++) {
		if (isWithin(slots[i], mouse)) {
			if (stock[i] != -1) {
				return stock[i];
			}
		}
	}
	return -1;
}

TooltipData MenuVendor::checkTooltip(Point mouse) {

	TooltipData tip;
	for (int i=0; i<VENDOR_SLOTS; i++) {
		if (isWithin(slots[i], mouse)) {
			if (stock[i] != -1) {
				return items->getTooltip(stock[i], stats, true);
			}
		}
	}
	return tip;
}

void MenuVendor::setInventory() {
	for (int i=0; i<VENDOR_SLOTS; i++) {
		stock[i] = npc->stock[i];
	}
}

MenuVendor::~MenuVendor() {
	SDL_FreeSurface(background);
}
