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
	if (stats->minabsorb == stats->maxabsorb)
		ss << stats->minabsorb;
	else
		ss << stats->minabsorb << "-" << stats->maxabsorb;
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

MenuCharacter::~MenuCharacter() {
	SDL_FreeSurface(background);
	SDL_FreeSurface(proficiency);
}
