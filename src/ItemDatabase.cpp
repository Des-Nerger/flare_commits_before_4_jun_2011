/**
 * class ItemDatabase
 *
 * @author Clint Bellanger
 * @license GPL
 */


#include "ItemDatabase.h"

ItemDatabase::ItemDatabase(SDL_Surface *_screen) {
	screen = _screen;
	
	for (int i=0; i<1024; i++) {
		items[i].name = "";
		items[i].icon32 = 0;
		items[i].icon64 = 0;
		items[i].type = -1;
		items[i].dmg_min = 0;
		items[i].dmg_max = 0;
		items[i].abs_min = 0;
		items[i].abs_max = 0;
		items[i].req_stat = 0;
		items[i].req_val = 0;
		items[i].sfx = SFX_NONE;
	}
	load();
	loadIcons();
	loadSounds();
}

void ItemDatabase::load() {
	ifstream infile;
	string line;
	string key;
	string val;
	string starts_with;
	int id = 0;
	string s;
	
	infile.open("items/items.txt", ios::in);

	if (infile.is_open()) {
		while (!infile.eof()) {

			getline(infile, line);

			if (line.length() > 0) {
				starts_with = line.at(0);
				
				if (starts_with == "#") {
					// skip comments
				}
				else if (starts_with == "[") {
					// skip headers
					// we expect the first entry after the header
					//   to be the new id
				}
				else { // this is data.  treatment depends on section type
					parse_key_pair(line, key, val);          
					key = trim(key, ' ');
					val = trim(val, ' ');
					//num = atoi(val.c_str());
					
					if (key == "id")
						id = atoi(val.c_str());
					else if (key == "name")
						items[id].name = val;
					else if (key == "icon") {
						val = val + ",";
						items[id].icon32 = eatFirstInt(val, ',');
						if (val.length() > 0)
							items[id].icon64 = eatFirstInt(val, ',');
					}
					else if (key == "type") {
						if (val == "main")
							items[id].type = ITEM_TYPE_MAIN;
						else if (val == "body")
							items[id].type = ITEM_TYPE_BODY;
						else if (val == "off")
							items[id].type = ITEM_TYPE_OFF;
						else if (val == "artifact")
							items[id].type = ITEM_TYPE_ARTIFACT;
						else if (val == "consumable")
							items[id].type = ITEM_TYPE_CONSUMABLE;
						else if (val == "gem")
							items[id].type = ITEM_TYPE_GEM;
					}
					else if (key == "dmg") {
						val = val + ",";
						items[id].dmg_min = eatFirstInt(val, ',');
						if (val.length() > 0)
							items[id].dmg_max = eatFirstInt(val, ',');
						else
							items[id].dmg_max = items[id].dmg_min;
					}
					else if (key == "abs") {
						val = val + ",";
						items[id].abs_min = eatFirstInt(val, ',');
						if (val.length() > 0)
							items[id].abs_max = eatFirstInt(val, ',');
						else
							items[id].abs_max = items[id].abs_min;
					}
					else if (key == "req") {
						val = val + ",";
						s = eatFirstString(val, ',');
						items[id].req_val = eatFirstInt(val, ',');
						if (s == "p")
							items[id].req_stat = REQUIRES_PHYS;
						else if (s == "m")
							items[id].req_stat = REQUIRES_MAG;
						else if (s == "o")
							items[id].req_stat = REQUIRES_OFF;
						else if (s == "d")
							items[id].req_stat = REQUIRES_DEF;
					}
					else if (key == "sfx") {
						if (val == "book")
							items[id].sfx = SFX_BOOK;
						else if (val == "cloth")
							items[id].sfx = SFX_CLOTH;
						else if (val == "coins")
							items[id].sfx = SFX_COINS;
						else if (val == "gem")
							items[id].sfx = SFX_GEM;
						else if (val == "leather")
							items[id].sfx = SFX_LEATHER;
						else if (val == "metal")
							items[id].sfx = SFX_METAL;
						else if (val == "page")
							items[id].sfx = SFX_PAGE;
						else if (val == "maille")
							items[id].sfx = SFX_MAILLE;
						else if (val == "object")
							items[id].sfx = SFX_OBJECT;
						else if (val == "heavy")
							items[id].sfx = SFX_HEAVY;
						else if (val == "wood")
							items[id].sfx = SFX_WOOD;
						else if (val == "potion")
							items[id].sfx = SFX_POTION;
					}
				}
			}
		}
	}
	infile.close();
}


void ItemDatabase::loadIcons() {
	
	icons = IMG_Load("images/icons/icons.png");
	if(!icons) {
		fprintf(stderr, "Couldn't load icons: %s\n", IMG_GetError());
	}
}

void ItemDatabase::loadSounds() {

	sfx[SFX_BOOK] = Mix_LoadWAV("soundfx/inventory/inventory_book.ogg");
	sfx[SFX_CLOTH] = Mix_LoadWAV("soundfx/inventory/inventory_cloth.ogg");
	sfx[SFX_COINS] = Mix_LoadWAV("soundfx/inventory/inventory_coins.ogg");
	sfx[SFX_GEM] = Mix_LoadWAV("soundfx/inventory/inventory_gem.ogg");
	sfx[SFX_LEATHER] = Mix_LoadWAV("soundfx/inventory/inventory_leather.ogg");
	sfx[SFX_METAL] = Mix_LoadWAV("soundfx/inventory/inventory_metal.ogg");
	sfx[SFX_PAGE] = Mix_LoadWAV("soundfx/inventory/inventory_page.ogg");
	sfx[SFX_MAILLE] = Mix_LoadWAV("soundfx/inventory/inventory_maille.ogg");
	sfx[SFX_OBJECT] = Mix_LoadWAV("soundfx/inventory/inventory_object.ogg");
	sfx[SFX_HEAVY] = Mix_LoadWAV("soundfx/inventory/inventory_heavy.ogg");
	sfx[SFX_WOOD] = Mix_LoadWAV("soundfx/inventory/inventory_wood.ogg");
	sfx[SFX_POTION] = Mix_LoadWAV("soundfx/inventory/inventory_potion.ogg");
	
}

void ItemDatabase::renderIcon(int item, int x, int y, int size) {
	dest.x = x;
	dest.y = y;
	if (size == ICON_SIZE_32) {
		src.w = src.h = dest.w = dest.h = 32;
		src.x = (items[item].icon32 % 16) * 32;
		src.y = (items[item].icon32 / 16) * 32;
		SDL_BlitSurface(icons, &src, screen, &dest);		
	}
	else if (size == ICON_SIZE_64) {
		src.w = src.h = dest.w = dest.h = 64;
		src.x = (items[item].icon64 % 8) * 64;
		src.y = (items[item].icon64 / 8) * 64 + 256;
		SDL_BlitSurface(icons, &src, screen, &dest);
	}
}

void ItemDatabase::playSound(int item) {
	if (sfx[items[item].sfx])
		Mix_PlayChannel(-1, sfx[items[item].sfx], 0);
}

string ItemDatabase::getTooltip(int item) {
	stringstream ss;
	
	// name
	ss << items[item].name;
	
	// type
	if (items[item].type != ITEM_TYPE_OTHER) {
		if (items[item].type == ITEM_TYPE_MAIN)
			ss << "\nMain Hand";
		else if (items[item].type == ITEM_TYPE_BODY)
			ss << "\nBody";
		else if (items[item].type == ITEM_TYPE_OFF)
			ss << "\nOff Hand";
		else if (items[item].type == ITEM_TYPE_ARTIFACT)
			ss << "\nArtifact";
		else if (items[item].type == ITEM_TYPE_CONSUMABLE)
			ss << "\nConsumable";
	}
	
	// damage
	if (items[item].dmg_max > 0) {
		if (items[item].dmg_min < items[item].dmg_max) {
			ss << "\nDamage: " << items[item].dmg_min << "-" << items[item].dmg_max;
		}
		else {
			ss << "\nDamage: " << items[item].dmg_max;		
		}
	}

	// absorb
	if (items[item].abs_max > 0) {
		if (items[item].abs_min < items[item].abs_max) {
			ss << "\nAbsorb: " << items[item].abs_min << "-" << items[item].abs_max;
		}
		else {
			ss << "\nAbsorb: " << items[item].abs_max;		
		}
	}
	
	// requirement
	if (items[item].req_val > 0) {
		if (items[item].req_stat == REQUIRES_PHYS)
			ss << "\nRequires Physical " << items[item].req_val;
		else if (items[item].req_stat == REQUIRES_MAG)
			ss << "\nRequires Magical " << items[item].req_val;
		else if (items[item].req_stat == REQUIRES_OFF)
			ss << "\nRequires Offense " << items[item].req_val;
		else if (items[item].req_stat == REQUIRES_DEF)
			ss << "\nRequires Defense " << items[item].req_val;
	}
	
	return ss.str();
}

ItemDatabase::~ItemDatabase() {
	SDL_FreeSurface(icons);
	for (int i=0; i<12; i++) {
		if (sfx[i])
			Mix_FreeChunk(sfx[i]);
	}
}
