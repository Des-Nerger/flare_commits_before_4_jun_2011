/**
 * class MenuInventory
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuInventory.h"

MenuInventory::MenuInventory(SDL_Surface *_screen, FontEngine *_font, ItemDatabase *_items, StatBlock *_stats) {
	screen = _screen;
	font = _font;
	items = _items;
	stats = _stats;
	
	visible = false;
	loadGraphics();
	
	for (int i=0; i<4; i++)
		equipped[i] = 0;
		
	for (int i=0; i<64; i++)
		carried[i] = 0;
		
	drag_prev_slot = -1;
	changed_equipment = true;
	changed_artifact = true;
	
	// TODO: carried items should come from a save file
	equipped[0] = 0;
	equipped[1] = 17;
	equipped[2] = 0;
	
	carried[0] = 1;
	carried[1] = 2;
	carried[2] = 3;
	carried[3] = 4;
	carried[4] = 5;
	carried[5] = 6;
	carried[6] = 7;
	carried[7] = 8;
	carried[8] = 9;
	carried[9] = 10;
	carried[10] = 11;
	carried[11] = 12;
	carried[12] = 13;
	carried[13] = 14;
	carried[14] = 15;
	carried[15] = 16;
	carried[16] = 17;
	carried[17] = 18;
	carried[18] = 19;
	carried[24] = 33;
	carried[25] = 34;
	carried[26] = 35;
	carried[27] = 36;
	carried[28] = 37;
	carried[29] = 38;
	carried[30] = 39;
	carried[31] = 40;
	carried[32] = 41;
	carried[33] = 42;
	
	carried[40] = 100;
	carried[41] = 101;
	carried[42] = 102;
	carried[43] = 103;
	carried[44] = 104;
	carried[45] = 105;
	carried[46] = 106;
	carried[47] = 107;
	carried[48] = 108;
	carried[49] = 109;
	carried[50] = 110;
	carried[51] = 111;
	carried[52] = 112;
	carried[53] = 113;
	carried[54] = 114;
	carried[55] = 115;
	
	carried[62] = 1022;
	carried[63] = 1023;
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

int MenuInventory::click(Point mouse) {

	int item;
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	
	if (mouse.x >= offset_x+32 && mouse.y >= offset_y+48 && mouse.x < offset_x+576 && mouse.y < offset_y+112) {
		// clicked an equipped item
		drag_prev_slot = (mouse.x - (offset_x+32)) / 64;
		drag_prev_src = SRC_EQUIPPED;
		
		item = equipped[drag_prev_slot];
		equipped[drag_prev_slot] = 0;

		return item;
	}
	else if (mouse.x >= offset_x+32 && mouse.y >= offset_y+128 && mouse.x < offset_x+576 && mouse.y < offset_y+384) {
		// clicked a carried item
		drag_prev_slot = (mouse.x - (offset_x+32)) / 32 + ((mouse.y - (offset_y+128)) / 32) * 8;
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
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;

	if (mouse.x >= offset_x+32 && mouse.y >= offset_y+48 && mouse.x < offset_x+576 && mouse.y < offset_y+112) {
	
		// dropped onto equipped item
		index = (mouse.x - (offset_x+32)) / 64;	

		if (drag_prev_src == SRC_CARRIED) {
			
			// make sure the item is going to the correct slot
			// note: equipment slots 0-3 correspond with item types 0-3
			// TODO: also check to see if the hero meets the requirements
			if (index == items->items[item].type) {
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
	else if (mouse.x >= offset_x+32 && mouse.y >= offset_y+128 && mouse.x < offset_x+576 && mouse.y < offset_y+384) {
	
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
			// TODO: also check to see if the hero meets the requirements
			if (carried[index] == 0 || items->items[carried[index]].type == drag_prev_slot) {
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
		if (drag_prev_src == SRC_CARRIED)
			carried[drag_prev_slot] = item; // cancel
		else if (drag_prev_src == SRC_EQUIPPED)
			equipped[drag_prev_slot] = item; // cancel
	}
 
}

TooltipData MenuInventory::checkTooltip(Point mouse) {
	int index;
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	TooltipData tip;
	
	if (mouse.x >= offset_x+32 && mouse.y >= offset_y+48 && mouse.x < offset_x+288 && mouse.y < offset_y+112) {
		
		// equipped item
		index = (mouse.x - (offset_x+32)) / 64;	
		return items->getTooltip(equipped[index], stats);
		
	}
	else if (mouse.x >= offset_x+32 && mouse.y >= offset_y+128 && mouse.x < offset_x+288 && mouse.y < offset_y+384) {
	
		// carried item
		index = (mouse.x - (offset_x+32)) / 32 + ((mouse.y - (offset_y+128)) / 32) * 8;
		return items->getTooltip(carried[index], stats);
		
	}
	
	return tip;
}

bool MenuInventory::full() {
	bool result = true;
	for (int i=0; i<64; i++) {
		if (carried[i] == 0) result = false;
	}
	return result;
}

MenuInventory::~MenuInventory() {
	SDL_FreeSurface(background);
}
