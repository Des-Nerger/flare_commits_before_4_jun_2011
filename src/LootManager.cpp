/**
 * class LootManager
 *
 * Handles floor loot
 *
 * @author Clint Bellanger
 * @license GPL
 */
 
#include "LootManager.h"
 
LootManager::LootManager(ItemDatabase *_items, MenuTooltip *_tip, EnemyManager *_enemies, MapIso *_map) {
	items = _items;
	tip = _tip;
	enemies = _enemies; // we need to be able to read loot state when creatures die
	map = _map; // we need to be able to read loot that drops from map containers
	
	tooltip_margin = 32; // pixels between loot drop center and label
	
	loot_count = 0;
	animation_count = 0;
	
	for (int i=0; i<64; i++) {
		flying_loot[i] = NULL;
		animation_id[i] = "";
	}
	
	loot_flip = NULL;
	
	// reset current map loot
	for (int i=0; i<256; i++) {
		loot[i].pos.x = 0;
		loot[i].pos.y = 0;
		loot[i].frame = 0;
		loot[i].item = 0;
	}

	// reset loot table
	for (int lvl=0; lvl<15; lvl++) {
		loot_table_count[lvl] = 0;
		for (int num=0; num<256; num++) {
			loot_table[lvl][num] = 0;
		}
	}
	
	loadGraphics();
	calcTables();
	loot_flip = Mix_LoadWAV("soundfx/flying_loot.ogg");
	
}

/**
 * The "loot" variable on each item refers to the "flying loot" animation for that item.
 * Here we load all the animations used by the item database.
 */
void LootManager::loadGraphics() {

	string anim_id;
	bool new_anim;

	// check all items in the item database
	for (int i=0; i<1024; i++) {
		anim_id = items->items[i].loot;
		
		new_anim = true;
		
		if (anim_id != "") {
			
			// do we have this animation loaded already?
			for (int j=0; j<animation_count; j++) {
				if (anim_id == animation_id[j]) new_anim = false;
			}
			
			if (new_anim) {
				animation_id[animation_count] = anim_id;
				flying_loot[animation_count] = IMG_Load(("images/loot/" + anim_id + ".png").c_str());
				animation_count++;
			}
		}
	}
	
	// set magic pink transparency
	for (int i=0; i<animation_count; i++) {
		SDL_SetColorKey( flying_loot[i], SDL_SRCCOLORKEY, SDL_MapRGB(flying_loot[i]->format, 255, 0, 255) ); 
	}
}

/**
 * Each item has a level, roughly associated with what level monsters drop that item.
 * Each item also has a quality which affects how often it drops.
 * Here we calculate loot probability by level so that when loot drops we
 * can quickly choose what loot should drop.
 */
void LootManager::calcTables() {

	int level;
	
	for (int i=0; i<1024; i++) {
		level = items->items[i].level;
		if (level > 0) {
			if (items->items[i].quality == ITEM_QUALITY_LOW) {
				for (int j=0; j<RARITY_LOW; j++) {
					loot_table[level][loot_table_count[level]] = i;
					loot_table_count[level]++;
				}
			}
			if (items->items[i].quality == ITEM_QUALITY_NORMAL) {
				for (int j=0; j<RARITY_NORMAL; j++) {
					loot_table[level][loot_table_count[level]] = i;
					loot_table_count[level]++;
				}
			}
			if (items->items[i].quality == ITEM_QUALITY_HIGH) {
				for (int j=0; j<RARITY_HIGH; j++) {
					loot_table[level][loot_table_count[level]] = i;
					loot_table_count[level]++;
				}
			}
			if (items->items[i].quality == ITEM_QUALITY_EPIC) {
				for (int j=0; j<RARITY_EPIC; j++) {
					loot_table[level][loot_table_count[level]] = i;
					loot_table_count[level]++;
				}
			}
		}
	}
}

void LootManager::handleNewMap() {
	loot_count = 0;
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
	
	checkEnemiesForLoot();
	checkMapForLoot();
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
		if (loot[i].frame >= 23) {
			dest.x = VIEW_W_HALF + (loot[i].pos.x/UNITS_PER_PIXEL_X - xcam.x) - (loot[i].pos.y/UNITS_PER_PIXEL_X - xcam.y);
			dest.y = VIEW_H_HALF + (loot[i].pos.x/UNITS_PER_PIXEL_Y - ycam.x) + (loot[i].pos.y/UNITS_PER_PIXEL_Y - ycam.y) + (TILE_H/2);
		
			// adjust dest.y so that the tooltip floats above the item
			dest.y -= tooltip_margin;
			td = items->getShortTooltip(loot[i].item);
			tip->render(td, dest, STYLE_TOPLABEL);
		}
	}
	

}

/**
 * Check the enemies for loot
 */
void LootManager::checkEnemiesForLoot() {
	for (int i=0; i<enemies->enemy_count; i++) {
		if (enemies->enemies[i]->loot_drop) {
			determineLoot(enemies->enemies[i]->stats.level, enemies->enemies[i]->stats.pos);
			enemies->enemies[i]->loot_drop = false;
		}
	}
}

/**
 * As map events occur, some might have a component named "loot"
 * Loot is created at component x,y
 */
void LootManager::checkMapForLoot() {
	Point p;
	Event_Component *ec;
	
	while (!map->loot.empty()) {
		ec = &map->loot.front();
		p.x = ec->x;
		p.y = ec->y;
		
		if (ec->s == "random") {
			determineLoot(ec->z, p);
		}
		else if (ec->s == "id") {
			addLoot(ec->z, p);
		}
		map->loot.pop();
	}
}

/**
 * Monsters don't just drop loot equal to their level
 * The loot level spread is a bell curve
 */
int LootManager::lootLevel(int base_level) {

	int x = rand() % 100;
	int actual;
	
	// this loot bell curve is +/- 3 levels
	if (x <= 4) actual = base_level-3;
	else if (x <= 14) actual = base_level-2;
	else if (x <= 34) actual = base_level-1;
	else if (x <= 64) actual = base_level;
	else if (x <= 84) actual = base_level+1;
	else if (x <= 94) actual = base_level+2;
	else actual = base_level+3;
	
	if (actual < 1 || actual > 14) actual = 0;
	
	return actual;
}

/**
 * This function is called when there definitely is a piece of loot dropping
 * base_level represents the average quality of this loot
 * calls addLoot()
 */
void LootManager::determineLoot(int base_level, Point pos) {

	int level = lootLevel(base_level);
	if (level > 0 && loot_table_count[level] > 0) {
		int roll = rand() % loot_table_count[level];
		addLoot(loot_table[level][roll], pos);
	}
}

void LootManager::addLoot(int item_id, Point pos) {
	// TODO: z-sort insert?
	loot[loot_count].item = item_id;
	loot[loot_count].pos.x = pos.x;
	loot[loot_count].pos.y = pos.y;
	loot[loot_count].frame = 0;
	loot_count++;
}


Renderable LootManager::getRender(int index) {
	
	Renderable r;
	r.map_pos.x = loot[index].pos.x;
	r.map_pos.y = loot[index].pos.y;
	
	// Right now the animation settings (number of frames, speed, frame size)
	// are hard coded.  At least move these to consts in the header.

	r.src = new SDL_Rect();
	r.src->x = (loot[index].frame / 4) * 64;
	r.src->y = 0;
	r.src->w = 64;
	r.src->h = 128;
	r.offset.x = 32;
	r.offset.y = 112;
	r.object_layer = true;

	for (int i=0; i<animation_count; i++) {
		if (items->items[loot[index].item].loot == animation_id[i])
			r.sprite = flying_loot[i];
	}

	return r;	
}

LootManager::~LootManager() {
	for (int i=0; i<64; i++)
		if (flying_loot[i])
			SDL_FreeSurface(flying_loot[i]);
	if (loot_flip) Mix_FreeChunk(loot_flip);
}
