/**
 * class ItemDatabase
 *
 * @author Clint Bellanger
 * @license GPL
 */


#include "ItemDatabase.h"

ItemDatabase::ItemDatabase(SDL_Surface *_screen, SDL_Surface *_icons) {
	screen = _screen;
	icons = _icons;
	
	for (int i=0; i<1024; i++) {
		items[i].name = "";
		items[i].level = 0;
		items[i].quality = ITEM_QUALITY_NORMAL;
		items[i].icon32 = 0;
		items[i].icon64 = 0;
		items[i].type = -1;
		items[i].dmg_min = 0;
		items[i].dmg_max = 0;
		items[i].abs_min = 0;
		items[i].abs_max = 0;
		items[i].req_stat = 0;
		items[i].req_val = 0;
		items[i].bonus_stat = "";
		items[i].bonus_val = 0;
		items[i].sfx = SFX_NONE;
		items[i].gfx = "";
		items[i].loot = "";
		items[i].power = -1;
		items[i].power_mod = -1;
		items[i].power_desc = "";
		items[i].price = 0;
		items[i].max_quantity = 1;
	}
	
	vendor_ratio = 4; // this means scrap/vendor pays 1/4th price to buy items from hero
	load();
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

			line = getLine(infile);

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
					else if (key == "level")
						items[id].level = atoi(val.c_str());
					else if (key == "icon") {
						val = val + ",";
						items[id].icon32 = eatFirstInt(val, ',');
						if (val.length() > 0)
							items[id].icon64 = eatFirstInt(val, ',');
					}
					else if (key == "quality") {
						if (val == "low")
							items[id].quality = ITEM_QUALITY_LOW;
						else if (val == "high")
							items[id].quality = ITEM_QUALITY_HIGH;
						else if (val == "epic")
							items[id].quality = ITEM_QUALITY_EPIC;
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
							items[id].req_stat = REQUIRES_MENT;
						else if (s == "o")
							items[id].req_stat = REQUIRES_OFF;
						else if (s == "d")
							items[id].req_stat = REQUIRES_DEF;
					}
					else if (key == "bonus") {
						val = val + ",";
						items[id].bonus_stat = eatFirstString(val, ',');
						items[id].bonus_val = eatFirstInt(val, ',');
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
					else if (key == "gfx")
						items[id].gfx = val;
					else if (key == "loot")
						items[id].loot = val;
					else if (key == "effect")
						items[id].effect = val;
					else if (key == "power")
						items[id].power = atoi(val.c_str());
					else if (key == "power_mod")
						items[id].power_mod = atoi(val.c_str());
					else if (key == "power_desc")
						items[id].power_desc = val;
					else if (key == "price")
						items[id].price = atoi(val.c_str());
					else if (key == "max_quantity")
						items[id].max_quantity = atoi(val.c_str());
				}
			}
		}
	}
	infile.close();
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
	if (items[item].sfx != SFX_NONE)
		if (sfx[items[item].sfx])
			Mix_PlayChannel(-1, sfx[items[item].sfx], 0);
}

void ItemDatabase::playCoinsSound() {
	Mix_PlayChannel(-1, sfx[SFX_COINS], 0);
}

TooltipData ItemDatabase::getShortTooltip( ItemStack stack) {
	stringstream ss;
	TooltipData tip;
	
	if (stack.item == 0) return tip;
	
	// name
	if( stack.quantity > 1) {
		ss << stack.quantity << " " << items[stack.item].name;
	} else {
		ss << items[stack.item].name;
	}
	tip.lines[tip.num_lines++] = ss.str();
	
	// color quality
	if (items[stack.item].quality == ITEM_QUALITY_LOW) {
		tip.colors[0] = FONT_GRAY;
	}
	else if (items[stack.item].quality == ITEM_QUALITY_HIGH) {
		tip.colors[0] = FONT_GREEN;
	}
	else if (items[stack.item].quality == ITEM_QUALITY_EPIC) {
		tip.colors[0] = FONT_BLUE;
	}
	
	return tip;
}

/**
 * Create detailed tooltip showing all relevant item info
 */
TooltipData ItemDatabase::getTooltip(int item, StatBlock *stats, bool vendor_view) {
	stringstream ss;
	TooltipData tip;
	
	if (item == 0) return tip;
	
	// name
	tip.lines[tip.num_lines++] = items[item].name;
	
	// color quality
	if (items[item].quality == ITEM_QUALITY_LOW) {
		tip.colors[0] = FONT_GRAY;
	}
	else if (items[item].quality == ITEM_QUALITY_HIGH) {
		tip.colors[0] = FONT_GREEN;
	}
	else if (items[item].quality == ITEM_QUALITY_EPIC) {
		tip.colors[0] = FONT_BLUE;
	}
	
	// level
	ss.str();
	if (items[item].level != 0) {
		ss << "Level " << items[item].level;
		tip.lines[tip.num_lines++] = ss.str();
	}
	
	// type
	if (items[item].type != ITEM_TYPE_OTHER) {
		if (items[item].type == ITEM_TYPE_MAIN)
			tip.lines[tip.num_lines++] = "Main Hand";
		else if (items[item].type == ITEM_TYPE_BODY)
			tip.lines[tip.num_lines++] = "Body";
		else if (items[item].type == ITEM_TYPE_OFF)
			tip.lines[tip.num_lines++] = "Off Hand";
		else if (items[item].type == ITEM_TYPE_ARTIFACT)
			tip.lines[tip.num_lines++] = "Artifact";
		else if (items[item].type == ITEM_TYPE_CONSUMABLE)
			tip.lines[tip.num_lines++] = "Consumable";
		else if (items[item].type == ITEM_TYPE_GEM)
			tip.lines[tip.num_lines++] = "Gem";
	}
	
	// damage
	ss.str("");
	if (items[item].dmg_max > 0) {
		if (items[item].req_stat == REQUIRES_PHYS) {
			ss << "Melee ";
		}
		else if (items[item].req_stat == REQUIRES_MENT) {
			ss << "Mental ";
		}
		else if (items[item].req_stat == REQUIRES_OFF) {
			ss << "Ranged ";
		}
		
		if (items[item].dmg_min < items[item].dmg_max) {
			ss << "damage: " << items[item].dmg_min << "-" << items[item].dmg_max;
		}
		else {
			ss << "damage: " << items[item].dmg_max;		
		}
		tip.lines[tip.num_lines++] = ss.str();
	}

	ss.str("");

	// absorb
	if (items[item].abs_max > 0) {
		if (items[item].abs_min < items[item].abs_max) {
			ss << "Absorb: " << items[item].abs_min << "-" << items[item].abs_max;
		}
		else {
			ss << "Absorb: " << items[item].abs_max;		
		}
		tip.lines[tip.num_lines++] = ss.str();
	}
	
	ss.str("");
	
	// bonus
	if (items[item].bonus_stat != "") {
		if (items[item].bonus_val > 0)
			ss << "Increases " << items[item].bonus_stat << " by " << items[item].bonus_val;
		else
			ss << "Decreases " << items[item].bonus_stat << " by " << (-1 * items[item].bonus_val);
		tip.lines[tip.num_lines++] = ss.str();
	}
	
	// power
	if (items[item].power_desc != "") {
		ss.str("");
		ss << items[item].power_desc;
		tip.colors[tip.num_lines] = FONT_GREEN;
		tip.lines[tip.num_lines++] = ss.str();
	}

	ss.str("");
	
	// requirement
	if (items[item].req_val > 0) {
		if (items[item].req_stat == REQUIRES_PHYS) {
			ss << "Requires Physical " << items[item].req_val;
			if (stats->physical < items[item].req_val) tip.colors[tip.num_lines] = FONT_RED;
		}
		else if (items[item].req_stat == REQUIRES_MENT) {
			ss << "Requires Mental " << items[item].req_val;
			if (stats->mental < items[item].req_val) tip.colors[tip.num_lines] = FONT_RED;
		}
		else if (items[item].req_stat == REQUIRES_OFF) {
			ss << "Requires Offense " << items[item].req_val;
			if (stats->offense < items[item].req_val) tip.colors[tip.num_lines] = FONT_RED;
		}
		else if (items[item].req_stat == REQUIRES_DEF) {
			ss << "Requires Defense " << items[item].req_val;
			if (stats->defense < items[item].req_val) tip.colors[tip.num_lines] = FONT_RED;
		}
		tip.lines[tip.num_lines++] = ss.str();
	}
	
	// buy or sell price
	ss.str("");
	if (items[item].price > 0) {
		if (vendor_view) {
			int buy_price = items[item].price;
			ss << "Buy price:: " << buy_price << " gold";
		}
		else {
			int sell_price = items[item].price/vendor_ratio;
			if (sell_price == 0) sell_price=1;
			ss << "Sell price: " << sell_price << " gold";
		}

		if (vendor_view && stats->gold < items[item].price)
			tip.colors[tip.num_lines] = FONT_RED;

		tip.lines[tip.num_lines++] = ss.str();
		
	}
	
	return tip;
}

/**
 * Given the equipped items, calculate the hero's stats
 */
void ItemDatabase::applyEquipment(StatBlock *stats, ItemStack * equipped) {

	// note: these are also defined in MenuInventory.h
	int SLOT_MAIN = 0;
	int SLOT_BODY = 1;
	int SLOT_OFF = 2;
	//int SLOT_ARTIFACT = 3;

	int prev_hp = stats->hp;
	int prev_mp = stats->mp;

	// defaults
	stats->recalc();
	stats->dmg_melee_min = stats->dmg_ment_min = 1;
	stats->dmg_melee_max = stats->dmg_ment_max = 4;
	stats->dmg_ranged_min = stats->dmg_ranged_max = 0;
	stats->absorb_min = stats->absorb_max = 0;
	stats->speed = 14;
	stats->dspeed = 10;
	stats->attunement_fire = 100;
	stats->attunement_ice = 100;


	// main hand weapon
	int item_id = equipped[SLOT_MAIN].item;
	if (item_id > 0) {
		if (items[item_id].req_stat == REQUIRES_PHYS) {
			stats->dmg_melee_min = items[item_id].dmg_min;
			stats->dmg_melee_max = items[item_id].dmg_max;
			stats->melee_weapon_power = items[item_id].power_mod;
			stats->wielding_physical = true;
		}
		else if (items[item_id].req_stat == REQUIRES_MENT) {
			stats->dmg_ment_min = items[item_id].dmg_min;
			stats->dmg_ment_max = items[item_id].dmg_max;						
			stats->mental_weapon_power = items[item_id].power_mod;	
			stats->wielding_mental = true;
		}
	}
	// off hand item
	item_id = equipped[SLOT_OFF].item;
	if (item_id > 0) {
		if (items[item_id].req_stat == REQUIRES_OFF) {
			stats->dmg_ranged_min = items[item_id].dmg_min;
			stats->dmg_ranged_max = items[item_id].dmg_max;
			stats->ranged_weapon_power = items[item_id].power_mod;
			stats->wielding_offense = true;
		}
		else if (items[item_id].req_stat == REQUIRES_DEF) {
			stats->absorb_min += items[item_id].abs_min;
			stats->absorb_max += items[item_id].abs_max;						
		}
	}		
	// body item
	item_id = equipped[SLOT_BODY].item;
	if (item_id > 0) {
		stats->absorb_min += items[item_id].abs_min;
		stats->absorb_max += items[item_id].abs_max;						
	}

	// apply bonuses from all items
	for (int i=0; i<4; i++) {
		item_id = equipped[i].item;
		
		if (items[item_id].bonus_stat == "HP")
			stats->maxhp += items[item_id].bonus_val;
		else if (items[item_id].bonus_stat == "HP regen")
			stats->hp_per_minute += items[item_id].bonus_val;
		else if (items[item_id].bonus_stat == "MP")
			stats->maxmp += items[item_id].bonus_val;
		else if (items[item_id].bonus_stat == "MP regen")
			stats->mp_per_minute += items[item_id].bonus_val;
		else if (items[item_id].bonus_stat == "accuracy")
			stats->accuracy += items[item_id].bonus_val;
		else if (items[item_id].bonus_stat == "avoidance")
			stats->avoidance += items[item_id].bonus_val;
		else if (items[item_id].bonus_stat == "crit")
			stats->crit += items[item_id].bonus_val;
		else if (items[item_id].bonus_stat == "speed") {
			stats->speed += items[item_id].bonus_val;
			// speed bonuses are in multiples of 3
			// 3 ordinal, 2 diagonal is rounding pythagorus
			stats->dspeed += ((items[item_id].bonus_val) * 2) /3;
		}
		else if (items[item_id].bonus_stat == "fire resist")
			stats->attunement_fire -= items[item_id].bonus_val;
		else if (items[item_id].bonus_stat == "ice resist")
			stats->attunement_ice -= items[item_id].bonus_val;
	}
	
	// apply previous hp/mp
	if (prev_hp < stats->maxhp)
		stats->hp = prev_hp;
	else
		stats->hp = stats->maxhp;
		
	if (prev_mp < stats->maxmp)
		stats->mp = prev_mp;
	else
		stats->mp = stats->maxmp;
		
}

ItemDatabase::~ItemDatabase() {
	SDL_FreeSurface(icons);
	for (int i=0; i<12; i++) {
		if (sfx[i])
			Mix_FreeChunk(sfx[i]);
	}
}
