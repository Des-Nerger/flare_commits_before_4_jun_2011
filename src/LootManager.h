/**
 * class LootManager
 *
 * Handles floor loot
 *
 * @author Clint Bellanger
 * @license GPL
 */
 
#ifndef LOOT_MANAGER_H
#define LOOT_MANAGER_H

#include "SDL.h"
#include "SDL_image.h"
#include "Utils.h"
#include "ItemDatabase.h"


class LootManager {
private:

	ItemDatabase *items;
	
	SDL_Surface *flying_loot;
	Point frame_size;
	int frame_count; // the last frame is the "at-rest" floor loot graphic
	
	// loot refers to ItemDatabase indices
	int loot[256]; // TODO: change to dynamic list without limits

public:
	LootManager(ItemDatabase *_items);
	~LootManager();
	Renderable getRender(int index);
	
	int loot_count;
	
};

#endif
