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
#include "SDL_mixer.h"

#include "Utils.h"
#include "ItemDatabase.h"
#include "MenuTooltip.h"

struct LootDef {
	int item;
	int frame;
	Point pos;
};

class LootManager {
private:

	ItemDatabase *items;
	MenuTooltip *tip;
	
	SDL_Surface *flying_loot[6];
	Mix_Chunk *loot_flip;
	
	Point frame_size;
	int frame_count; // the last frame is the "at-rest" floor loot graphic
	
	// loot refers to ItemDatabase indices
	LootDef loot[256]; // TODO: change to dynamic list without limits

public:
	LootManager(ItemDatabase *_items, MenuTooltip *_tip);
	~LootManager();
	void loadGraphics();
	
	void logic();
	void renderTooltips(Point cam);
	
	Renderable getRender(int index);
	
	int loot_count;
	
};

#endif
