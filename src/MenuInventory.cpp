/**
 * class MenuInventory
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuInventory.h"

MenuInventory::MenuInventory(SDL_Surface *_screen, FontEngine *_font, ItemDatabase *_items) {
	screen = _screen;
	font = _font;
	items = _items;
	
	visible = false;
	loadGraphics();
	
	for (int i=0; i<4; i++)
		equipped[i] = 0;
		
	for (int i=0; i<64; i++)
		carried[i] = 0;
		
	drag_prev_slot = -1;
	
	// testing
	// TEMP: right now equipped[] and carrying[] refer to icons.  They should refer
	// to an item index instead, which has public variables icon and icon_large. 
	equipped[0] = 4;
	equipped[1] = 15;
	equipped[2] = 16;
	
	carried[0] = 1;
	carried[1] = 2;
	carried[2] = 3;
	carried[3] = 5;
	carried[4] = 6;
	carried[5] = 7;
	carried[6] = 8;
	carried[7] = 9;
	carried[8] = 10;
	carried[9] = 11;
	carried[10] = 12;
	carried[11] = 13;
	carried[12] = 14;
	carried[13] = 17;
	carried[14] = 18;
	carried[15] = 19;
	carried[16] = 33;
	carried[17] = 34;
	carried[18] = 35;
	carried[19] = 36;
	carried[20] = 37;
	carried[21] = 38;
	carried[22] = 39;	
	carried[23] = 40;
	carried[24] = 41;
	carried[25] = 42;
	carried[26] = 41;
	carried[27] = 42;
	carried[28] = 41;
	carried[29] = 42;	

}

void MenuInventory::loadGraphics() {

	background = IMG_Load("images/menus/inventory.png");
	if(!background) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
}

void MenuInventory::render() {
	if (!visible) return;
	SDL_Rect src;
	SDL_Rect dest;
	
	// background
	src.x = 0;
	src.y = 0;
	dest.x = 320;
	dest.y = 32;
	src.w = dest.w = 320;
	src.h = dest.h = 416;
	SDL_BlitSurface(background, &src, screen, &dest);
	
	
	// text overlay
	// TODO: translate()
	font->render("Inventory", 480, 40, JUSTIFY_CENTER, screen);
	font->render("Main Hand", 384, 66, JUSTIFY_CENTER, screen);
	font->render("Body", 448, 66, JUSTIFY_CENTER, screen);
	font->render("Off Hand", 512, 66, JUSTIFY_CENTER, screen);
	font->render("Artifact", 576, 66, JUSTIFY_CENTER, screen);
	
	
	// equipped items
	for (int i=0; i<4; i++) {
		if (equipped[i] > 0) {
			dest.x = 352 + i * 64;
			dest.y = 80;
			items->renderIcon(equipped[i], dest.x, dest.y, ICON_SIZE_64);
		}
	}
	
	// carried items
	for (int i=0; i<64; i++) {
		if (carried[i] > 0) {
			dest.x = 352 + (i % 8 * 32);
			dest.y = 160 + (i / 8 * 32);
			items->renderIcon(carried[i], dest.x, dest.y, ICON_SIZE_32);
		}	
	}
}

int MenuInventory::click(Point mouse) {

	int item;
	
	if (mouse.x >= 352 && mouse.y >= 80 && mouse.x < 608 && mouse.y < 144) {
		// clicked an equipped item
		drag_prev_slot = (mouse.x - 352) / 64;
		drag_prev_src = SRC_EQUIPPED;
		
		item = equipped[drag_prev_slot];
		equipped[drag_prev_slot] = 0;

		return item;
	}
	else if (mouse.x >= 352 && mouse.y >= 160 && mouse.x < 608 && mouse.y < 416) {
		// clicked a carried item
		drag_prev_slot = (mouse.x - 352) / 32 + ((mouse.y - 160) / 32) * 8;
		drag_prev_src = SRC_CARRIED;
		
		item = carried[drag_prev_slot];
		carried[drag_prev_slot] = 0;
		
		return item;
	}

	return 0;
}

void MenuInventory::drop(Point mouse, int item) {
	items->playSound(item);

	int index;

	if (mouse.x >= 352 && mouse.y >= 80 && mouse.x < 608 && mouse.y < 144) {
	
		// dropped onto equipped item
		index = (mouse.x - 352) / 64;		

		if (drag_prev_src == SRC_CARRIED) {
			
			// make sure the item is going to the correct slot
			// note: equipment slots 0-3 correspond with item types 0-3
			// TODO: also check to see if the hero meets the requirements
			if (index == items->items[item].type) {
				carried[drag_prev_slot] = equipped[index];
				equipped[index] = item;
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
	else if (mouse.x >= 352 && mouse.y >= 160 && mouse.x < 608 && mouse.y < 416) {
	
		// dropped onto carried item
		index = (mouse.x - 352) / 32 + ((mouse.y - 160) / 32) * 8;
		
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
			// TODO: also check to see if the hero meets the requirements
			if (carried[index] == 0 || items->items[carried[index]].type == drag_prev_slot) {
				equipped[drag_prev_slot] = carried[index];
				carried[index] = item;
			}
			else {
				equipped[drag_prev_slot] = item; // cancel
			}
		}
	}
	else {
	
		// not dropped in a slot.  Just return it to the previous slot
		if (drag_prev_src == SRC_CARRIED)
			carried[drag_prev_slot] = item; // cancel
		else if (drag_prev_src == SRC_EQUIPPED)
			equipped[drag_prev_slot] = item; // cancel
	}
 
}

string MenuInventory::checkTooltip(Point mouse) {
	int index;
	if (mouse.x >= 352 && mouse.y >= 80 && mouse.x < 608 && mouse.y < 144) {
		
		// equipped item
		index = (mouse.x - 352) / 64;
		return items->getTooltip(equipped[index]);
		
	}
	else if (mouse.x >= 352 && mouse.y >= 160 && mouse.x < 608 && mouse.y < 416) {
	
		// carried item
		index = (mouse.x - 352) / 32 + ((mouse.y - 160) / 32) * 8;
		return items->getTooltip(carried[index]);
		
	}
	
	return "";
}

MenuInventory::~MenuInventory() {
	SDL_FreeSurface(background);
}
