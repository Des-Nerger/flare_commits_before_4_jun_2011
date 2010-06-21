/**
 * class LootManager
 *
 * Handles floor loot
 *
 * @author Clint Bellanger
 * @license GPL
 */
 
#include "LootManager.h"
 
LootManager::LootManager(ItemDatabase *_items, MenuTooltip *_tip) {
	items = _items;
	tip = _tip;
	
	loot_count = 0;
	
	for (int i=0; i<6; i++)
		flying_loot[i] = NULL;
	loot_flip = NULL;
		
	for (int i=0; i<256; i++) {
		loot[i].pos.x = 0;
		loot[i].pos.y = 0;
		loot[i].frame = 0;
		loot[i].item = 0;
	}
		
	loadGraphics();
	loot_flip = Mix_LoadWAV("soundfx/flying_loot.ogg");

	// testing loot
	loot_count = 2;
	loot[0].item = 2;
	loot[0].pos.x = 512;
	loot[0].pos.y = 512;

	loot[1].item = 101;
	loot[1].pos.x = 400;
	loot[1].pos.y = 400;
	
	
}

void LootManager::loadGraphics() {

	flying_loot[0] = IMG_Load("images/loot/armor.png");
	flying_loot[1] = IMG_Load("images/loot/bag.png");
	flying_loot[2] = IMG_Load("images/loot/bow.png");
	flying_loot[3] = IMG_Load("images/loot/shield.png");
	flying_loot[4] = IMG_Load("images/loot/staff.png");
	flying_loot[5] = IMG_Load("images/loot/sword.png");
	
	for (int i=0; i<6; i++) {
		SDL_SetColorKey( flying_loot[i], SDL_SRCCOLORKEY, SDL_MapRGB(flying_loot[i]->format, 255, 0, 255) ); 
	}
}

void LootManager::logic() {
	for (int i=0; i<loot_count; i++) {
	
		// animate flying loot
		if (loot[i].frame < 23)
			loot[i].frame++;

		if (loot[i].frame == 1)
			if (loot_flip) Mix_PlayChannel(-1, loot_flip, 0);

		if (loot[i].frame == 20)
			items->playSound(loot[i].item);
	}
}


/**
 * Show all tooltips for loot on the floor
 */
void LootManager::renderTooltips(Point cam) {

	Point xcam;
	Point ycam;
	xcam.x = cam.x/UNITS_PER_PIXEL_X;
	xcam.y = cam.y/UNITS_PER_PIXEL_X;
	ycam.x = cam.x/UNITS_PER_PIXEL_Y;
	ycam.y = cam.y/UNITS_PER_PIXEL_Y;
	
	Point dest;
	TooltipData td;
	
	for (int i = 0; i < loot_count; i++) {			

		dest.x = VIEW_W_HALF + (loot[i].pos.x/UNITS_PER_PIXEL_X - xcam.x) - (loot[i].pos.y/UNITS_PER_PIXEL_X - xcam.y);
		dest.y = VIEW_H_HALF + (loot[i].pos.x/UNITS_PER_PIXEL_Y - ycam.x) + (loot[i].pos.y/UNITS_PER_PIXEL_Y - ycam.y) + (TILE_H/2);
		td = items->getShortTooltip(loot[i].item);
		tip->render(td, dest);
	}
	

}

Renderable LootManager::getRender(int index) {
	// test code
	
	Renderable r;
	r.map_pos.x = loot[index].pos.x;
	r.map_pos.y = loot[index].pos.y;

	r.src = new SDL_Rect();
	r.src->x = (loot[index].frame / 4) * 64;
	r.src->y = 0;
	r.src->w = 64;
	r.src->h = 64;
	r.offset.x = 32;
	r.offset.y = 32;
	r.object_layer = true;


	if (items->items[loot[index].item].loot == "armor")
		r.sprite = flying_loot[0];
	else if (items->items[loot[index].item].loot == "bow")
		r.sprite = flying_loot[2];
	else if (items->items[loot[index].item].loot == "shield")
		r.sprite = flying_loot[3];
	else if (items->items[loot[index].item].loot == "staff")
		r.sprite = flying_loot[4];
	else if (items->items[loot[index].item].loot == "sword")
		r.sprite = flying_loot[5];
	else
		r.sprite = flying_loot[1]; // bag
	

	return r;	
}

LootManager::~LootManager() {
	for (int i=0; i<6; i++)
		if (flying_loot[i])
			SDL_FreeSurface(flying_loot[i]);
	if (loot_flip) Mix_FreeChunk(loot_flip);
}
