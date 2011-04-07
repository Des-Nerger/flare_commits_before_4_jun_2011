/**
 * class ItemDatabase
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef ITEM_DATABASE_H
#define ITEM_DATABASE_H

#include <string>
#include <sstream>
#include <fstream>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "UtilsParsing.h"
#include "StatBlock.h"
#include "MenuTooltip.h"

using namespace std;

const int MAX_ITEM_ID = 8192;

const int ICON_SIZE_32 = 32;
const int ICON_SIZE_64 = 64;

const int ITEM_TYPE_OTHER = -1;
const int ITEM_TYPE_MAIN = 0;
const int ITEM_TYPE_BODY = 1;
const int ITEM_TYPE_OFF = 2;
const int ITEM_TYPE_ARTIFACT = 3;
const int ITEM_TYPE_CONSUMABLE = 4;
const int ITEM_TYPE_GEM = 5;

const int REQUIRES_PHYS = 0;
const int REQUIRES_MENT = 1;
const int REQUIRES_OFF = 2;
const int REQUIRES_DEF = 3;

const int SFX_NONE = -1;
const int SFX_BOOK = 0;
const int SFX_CLOTH = 1;
const int SFX_COINS = 2;
const int SFX_GEM = 3;
const int SFX_LEATHER = 4;
const int SFX_METAL = 5;
const int SFX_PAGE = 6;
const int SFX_MAILLE = 7;
const int SFX_OBJECT = 8;
const int SFX_HEAVY = 9;
const int SFX_WOOD = 10;
const int SFX_POTION = 11;

const int ITEM_QUALITY_LOW = 0;
const int ITEM_QUALITY_NORMAL = 1;
const int ITEM_QUALITY_HIGH = 2;
const int ITEM_QUALITY_EPIC = 3;

struct Item {
	string name;
	int level;
	int quality;
	int type;
	int icon32;
	int icon64;
	int dmg_min;
	int dmg_max;
	int abs_min;
	int abs_max;
	int req_stat;
	int req_val;
	string bonus_stat;
	int bonus_val;
	string effect;
	int sfx;
	string gfx;
	string loot;
	int power;
	int power_mod;
	string power_desc;
	int price;
	int max_quantity; // max count per stack
	int rand_loot; // max amount appearing in a loot stack
	int rand_vendor; // max amount appearing in a vendor stack
};

struct ItemStack {
	int item;
	int quantity;
	bool operator > (ItemStack param);
};

class ItemDatabase {
private:
	SDL_Surface *screen;
	SDL_Surface *icons32;
	SDL_Surface *icons64; // item db is the only module that currently uses the 64px icons
	FontEngine *font;
	SDL_Rect src;
	SDL_Rect dest;
	Mix_Chunk *sfx[12];

public:
	ItemDatabase(SDL_Surface *_screen, FontEngine *_font);
	~ItemDatabase();
	void load();
	void loadSounds();
	void loadIcons();
	void renderIcon(ItemStack stack, int x, int y, int size);
	void playSound(int item);
	void playCoinsSound();	
	TooltipData getTooltip(int item, StatBlock *stats, bool vendor_view);
	TooltipData getShortTooltip(ItemStack item);
	void applyEquipment(StatBlock *stats, ItemStack *equipped);

	Item items[MAX_ITEM_ID];
	int vendor_ratio;
};

#endif
