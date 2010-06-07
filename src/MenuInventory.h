/**
 * class MenuInventory
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef MENU_INVENTORY_H
#define MENU_INVENTORY_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "FontEngine.h"
#include "ItemDatabase.h"
#include <string>
#include <sstream>

using namespace std;

// note: if you change these, also change them in ItemDatabase::applyEquipment
const int SLOT_MAIN = 0;
const int SLOT_BODY = 1;
const int SLOT_OFF = 2;
const int SLOT_ARTIFACT = 3;

const int SRC_EQUIPPED = 0;
const int SRC_CARRIED = 1;

class MenuInventory {
private:
	SDL_Surface *screen;
	ItemDatabase *items;
	FontEngine *font;

	void loadGraphics();
	SDL_Surface *background;
	
public:
	MenuInventory(SDL_Surface *screen, FontEngine *font, ItemDatabase *items);
	~MenuInventory();
	void logic();
	void render();
	int click(Point mouse);
	void drop(Point mouse, int src);
	string checkTooltip(Point mouse);
	
	bool visible;

	int carried[64];
	int equipped[4];
	int drag_prev_slot;
	int drag_prev_src;

	bool changed_equipment;
	bool changed_artifact;

};

#endif

