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
#include "MenuTooltip.h"
#include "StatBlock.h"
#include "PowerManager.h"
#include <string>
#include <sstream>

using namespace std;

const int EQUIPMENT = 0;
const int CARRIED = 1;

const int MAX_EQUIPPED = 4;
const int MAX_CARRIED = 64;

// note: if you change these, also change them in ItemDatabase::applyEquipment
const int SLOT_MAIN = 0;
const int SLOT_BODY = 1;
const int SLOT_OFF = 2;
const int SLOT_ARTIFACT = 3;

class MenuInventory {
private:
	SDL_Surface *screen;
	ItemDatabase *items;
	FontEngine *font;
	StatBlock *stats;
	PowerManager *powers;

	void loadGraphics();
	SDL_Surface *background;
	
public:
	MenuInventory(SDL_Surface *screen, FontEngine *font, ItemDatabase *items, StatBlock *stats, PowerManager *powers);
	~MenuInventory();
	void logic();
	void render();
	ItemStack click(Point mouse);
	int areaClicked(Point mouse);
	int slotClicked(int area, Point mouse);
	void sell(Point mouse);
	void sell(ItemStack stack);
	void itemReturn(ItemStack stack);
	bool requirementsMet(int item);
	void drop(Point mouse, ItemStack stack);
	void activate(Point mouse);
	TooltipData checkTooltip(Point mouse);
	bool full();
	void add( ItemStack stack, int area = CARRIED, int slot = -1);
	void substract( int area, int slot, int quantity = 1);
	void addGold(int count);
	int getItemCountCarried(int item);
	bool isItemEquipped(int item);
	void remove(int item);
	bool purchase(Point mouse, int item);
	void update_equipment( int slot);
	
	bool visible;

	SDL_Rect carried_area;
	SDL_Rect equipped_area;

	ItemStack * inventory[2];
	int gold;
	int drag_prev_slot;
	int drag_prev_src;

	// the following two are separate because artifacts don't display on the hero.
	// so we only update the hero sprites when non-artifact changes occur.
	bool changed_equipment;
	bool changed_artifact;

};

#endif

