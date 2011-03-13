/**
 * class MenuVendor
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef MENU_VENDOR_H
#define MENU_VENDOR_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "FontEngine.h"
#include "ItemDatabase.h"
#include "MenuTooltip.h"
#include "StatBlock.h"
#include "NPC.h"
#include <string>
#include <sstream>

const int VENDOR_SLOTS = 80;

class MenuVendor {
private:
	SDL_Surface *screen;
	ItemDatabase *items;
	FontEngine *font;
	StatBlock *stats;

	void loadGraphics();
	SDL_Surface *background;

public:
	MenuVendor(SDL_Surface *screen, FontEngine *font, ItemDatabase *items, StatBlock *stats);
	~MenuVendor();

	NPC *npc;
	
	void loadMerchant(string filename);
	void logic();
	void render();
	int buy(Point mouse, int &gold);
	int click(Point mouse);
	TooltipData checkTooltip(Point mouse);
	void setInventory();
	
	bool visible;
	int stock[VENDOR_SLOTS]; // items the vendor currently has in stock
	SDL_Rect slots[VENDOR_SLOTS];
	SDL_Rect slots_area;
	
};


#endif
