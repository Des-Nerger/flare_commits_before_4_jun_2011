/**
 * class MenuCharacter
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuCharacter.h"

MenuCharacter::MenuCharacter(SDL_Surface *_screen, FontEngine *_font, StatBlock *_stats) {
	screen = _screen;
	font = _font;
	stats = _stats;
	
	visible = false;

	loadGraphics();
}

void MenuCharacter::loadGraphics() {

	background = IMG_Load("images/menus/character.png");
	proficiency = IMG_Load("images/menus/character_proficiency.png");
	if(!background || !proficiency) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
}

void MenuCharacter::render() {
	if (!visible) return;
	
	SDL_Rect src;
	SDL_Rect dest;
	
	// background
	src.x = 0;
	src.y = 0;
	dest.x = 0;
	dest.y = 32;
	src.w = dest.w = 320;
	src.h = dest.h = 416;
	SDL_BlitSurface(background, &src, screen, &dest);
	
	// labels
	// TODO: translate()
	font->render("Character", 160, 40, JUSTIFY_CENTER, screen);
	font->render("Name", 72, 66, JUSTIFY_RIGHT, screen);
	font->render("Level", 248, 66, JUSTIFY_RIGHT, screen);
	font->render("Physical", 40, 106, JUSTIFY_LEFT, screen);
	font->render("Magical", 40, 170, JUSTIFY_LEFT, screen);
	font->render("Offense", 40, 234, JUSTIFY_LEFT, screen);
	font->render("Defense", 40, 298, JUSTIFY_LEFT, screen);
	font->render("Total Health", 152, 138, JUSTIFY_RIGHT, screen);
	font->render("Regen", 248, 138, JUSTIFY_RIGHT, screen);
	font->render("Total Mana", 152, 202, JUSTIFY_RIGHT, screen);
	font->render("Regen", 248, 202, JUSTIFY_RIGHT, screen);
	font->render("Accuracy vs. Def 1", 152, 266, JUSTIFY_RIGHT, screen);
	font->render("vs. Def 5", 248, 266, JUSTIFY_RIGHT, screen);
	font->render("Avoidance vs. Off 1", 152, 330, JUSTIFY_RIGHT, screen);
	font->render("vs. Off 5", 248, 330, JUSTIFY_RIGHT, screen);
	font->render("Main Weapon", 120, 370, JUSTIFY_RIGHT, screen);
	font->render("Ranged Weapon", 120, 386, JUSTIFY_RIGHT, screen);
	font->render("Crit Chance", 120, 402, JUSTIFY_RIGHT, screen);
	font->render("Absorb", 248, 370, JUSTIFY_RIGHT, screen);
	font->render("Fire Resist", 248, 386, JUSTIFY_RIGHT, screen);
	font->render("Ice Resist", 248, 402, JUSTIFY_RIGHT, screen);

	// character data
	stringstream ss;
	font->render(stats->name, 83, 66, JUSTIFY_LEFT, screen);
	ss.str("");
	ss << stats->level;
	font->render(ss.str(), 268, 66, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->physical;
	font->render(ss.str(), 24, 106, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->magical;
	font->render(ss.str(), 24, 170, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->offense;
	font->render(ss.str(), 24, 234, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->defense;
	font->render(ss.str(), 24, 298, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->maxhp;
	font->render(ss.str(), 172, 138, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->hp_per_minute;
	font->render(ss.str(), 268, 138, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->maxmp;
	font->render(ss.str(), 172, 202, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->mp_per_minute;
	font->render(ss.str(), 268, 202, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << (stats->accuracy - 5) << "%";
	font->render(ss.str(), 172, 266, JUSTIFY_CENTER, screen);	
	ss.str("");
	ss << (stats->accuracy - 25) << "%";
	font->render(ss.str(), 268, 266, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << (stats->avoidance - 5) << "%";
	font->render(ss.str(), 172, 330, JUSTIFY_CENTER, screen);	
	ss.str("");
	ss << (stats->avoidance - 25) << "%";
	font->render(ss.str(), 268, 330, JUSTIFY_CENTER, screen);	
	ss.str("");
	if (stats->dmg_melee_max >= stats->dmg_magic_max)
		ss << stats->dmg_melee_min << "-" << stats->dmg_melee_max;
	else
		ss << stats->dmg_magic_min << "-" << stats->dmg_magic_max;
	font->render(ss.str(), 144, 370, JUSTIFY_CENTER, screen);	
	ss.str("");
	if (stats->dmg_ranged_max > 0)
		ss << stats->dmg_ranged_min << "-" << stats->dmg_ranged_max;
	else
		ss << "-";
	font->render(ss.str(), 144, 386, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->crit << "%";
	font->render(ss.str(), 144, 402, JUSTIFY_CENTER, screen);	
	ss.str("");
	if (stats->absorb_min == stats->absorb_max)
		ss << stats->absorb_min;
	else
		ss << stats->absorb_min << "-" << stats->absorb_max;
	font->render(ss.str(), 272, 370, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->resist_fire << "%";
	font->render(ss.str(), 272, 386, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->resist_ice << "%";
	font->render(ss.str(), 272, 402, JUSTIFY_CENTER, screen);
	
	// highlight proficiencies
	displayProficiencies(stats->physical, 96);
	displayProficiencies(stats->magical, 160);
	displayProficiencies(stats->offense, 224);
	displayProficiencies(stats->defense, 288);	
	
}

void MenuCharacter::displayProficiencies(int value, int y) {
	SDL_Rect src;
	SDL_Rect dest;
	src.x = 0;
	src.y = 0;
	src.w = dest.w = 48;
	src.h = dest.h = 32;
	dest.y = y;
	
	for (int i=2; i<= value; i++) {
		dest.x = 112 + (i-2) * 48;
		SDL_BlitSurface(proficiency, &src, screen, &dest);
	}
}

string MenuCharacter::checkTooltip(Point mouse) {
	if (mouse.x >= 256 && mouse.x <= 280 && mouse.y >= 64 && mouse.y <= 80)
		return "Experience to next level:\n0/1000";
	if (mouse.x >= 16 && mouse.x <= 80 && mouse.y >= 104 && mouse.y <= 120)
		return "Physical (P) increases melee proficiency and total health";
	if (mouse.x >= 16 && mouse.x <= 80 && mouse.y >= 168 && mouse.y <= 184)
		return "Magical (M) increases magic proficiency and total mana";
	if (mouse.x >= 16 && mouse.x <= 80 && mouse.y >= 232 && mouse.y <= 248)
		return "Offense (O) increases ranged proficiency and accuracy";
	if (mouse.x >= 16 && mouse.x <= 80 && mouse.y >= 296 && mouse.y <= 312)
		return "Defense (D) increases armor proficiency and avoidance";

	// Physical
	if (mouse.x >= 128 && mouse.x <= 160 && mouse.y >= 96 && mouse.y <= 128)
		return "Dagger Proficiency\nRequires Physical 2";
	if (mouse.x >= 176 && mouse.x <= 208 && mouse.y >= 96 && mouse.y <= 128)
		return "Shortsword Proficiency\nRequires Physical 3";
	if (mouse.x >= 224 && mouse.x <= 256 && mouse.y >= 96 && mouse.y <= 128)
		return "Longsword Proficiency\nRequires Physical 4";
	if (mouse.x >= 272 && mouse.x <= 304 && mouse.y >= 96 && mouse.y <= 128)
		return "Greatsword Proficiency\nRequires Physical 5";

	if (mouse.x >= 64 && mouse.x <= 184 && mouse.y >= 136 && mouse.y <= 152)
		return "Each point of Physical grants +4 health";
	if (mouse.x >= 208 && mouse.x <= 280 && mouse.y >= 136 && mouse.y <= 152)
		return "Ticks of health regen per minute\nEach point of Physical grants +1 health regen";

		
	// Magical
	if (mouse.x >= 128 && mouse.x <= 160 && mouse.y >= 160 && mouse.y <= 192)
		return "Wand Proficiency\nRequires Magical 2";
	if (mouse.x >= 176 && mouse.x <= 208 && mouse.y >= 160 && mouse.y <= 192)
		return "Rod Proficiency\nRequires Magical 3";
	if (mouse.x >= 224 && mouse.x <= 256 && mouse.y >= 160 && mouse.y <= 192)
		return "Staff Proficiency\nRequires Magical 4";
	if (mouse.x >= 272 && mouse.x <= 304 && mouse.y >= 160 && mouse.y <= 192)
		return "Greatstaff Proficiency\nRequires Magical 5";
		
	if (mouse.x >= 64 && mouse.x <= 184 && mouse.y >= 200 && mouse.y <= 216)
		return "Each point of Magical grants +4 mana";
	if (mouse.x >= 208 && mouse.x <= 280 && mouse.y >= 200 && mouse.y <= 216)
		return "Ticks of mana regen per minute\nEach point of Magical grants +1 mana regen";
		
		
	// Offense
	if (mouse.x >= 128 && mouse.x <= 160 && mouse.y >= 224 && mouse.y <= 256)
		return "Slingshot Proficiency\nRequires Offense 2";
	if (mouse.x >= 176 && mouse.x <= 208 && mouse.y >= 224 && mouse.y <= 256)
		return "Shortbow Proficiency\nRequires Offense 3";
	if (mouse.x >= 224 && mouse.x <= 256 && mouse.y >= 224 && mouse.y <= 256)
		return "Longbow Proficiency\nRequires Offense 4";
	if (mouse.x >= 272 && mouse.x <= 304 && mouse.y >= 224 && mouse.y <= 256)
		return "Greatbow Proficiency\nRequires Offense 5";
		
	if (mouse.x >= 64 && mouse.x <= 280 && mouse.y >= 264 && mouse.y <= 280)
		return "Each point of Offense grants +5 accuracy";
		
		
	// Defense
	if (mouse.x >= 128 && mouse.x <= 160 && mouse.y >= 288 && mouse.y <= 320)
		return "Light Armor Proficiency\nRequires Defense 2";
	if (mouse.x >= 176 && mouse.x <= 208 && mouse.y >= 288 && mouse.y <= 320)
		return "Light Shield Proficiency\nRequires Defense 3";
	if (mouse.x >= 224 && mouse.x <= 256 && mouse.y >= 288 && mouse.y <= 320)
		return "Heavy Armor Proficiency\nRequires Defense 4";
	if (mouse.x >= 272 && mouse.x <= 304 && mouse.y >= 288 && mouse.y <= 320)
		return "Heavy Shield Proficiency\nRequires Defense 5";
		
	if (mouse.x >= 64 && mouse.x <= 280 && mouse.y >= 328 && mouse.y <= 344)
		return "Each point of Defense grants +5 avoidance";
		
	return "";
}

MenuCharacter::~MenuCharacter() {
	SDL_FreeSurface(background);
	SDL_FreeSurface(proficiency);
}
