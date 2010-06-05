/**
 * class MenuPowers
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuPowers.h"

MenuPowers::MenuPowers(SDL_Surface *_screen, FontEngine *_font, StatBlock *_stats) {
	screen = _screen;
	font = _font;
	stats = _stats;
	
	visible = false;
	loadGraphics();
}

void MenuPowers::loadGraphics() {

	background = IMG_Load("images/menus/powers.png");
	powers_step = IMG_Load("images/menus/powers_step.png");
	powers_unlock = IMG_Load("images/menus/powers_unlock.png"); 
	if(!background || !powers_step || !powers_unlock) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
}

void MenuPowers::render() {
	if (!visible) return;
	
	SDL_Rect src;
	SDL_Rect dest;
	
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	
	// background
	src.x = 0;
	src.y = 0;
	dest.x = offset_x;
	dest.y = offset_y;
	src.w = dest.w = 320;
	src.h = dest.h = 416;
	SDL_BlitSurface(background, &src, screen, &dest);
	
	// text overlay
	// TODO: translate()
	font->render("Powers", offset_x+160, offset_y+8, JUSTIFY_CENTER, screen);
	font->render("Physical", offset_x+64, offset_y+50, JUSTIFY_CENTER, screen);
	font->render("Physical", offset_x+128, offset_y+50, JUSTIFY_CENTER, screen);
	font->render("Magical", offset_x+192, offset_y+50, JUSTIFY_CENTER, screen);
	font->render("Magical", offset_x+256, offset_y+50, JUSTIFY_CENTER, screen);
	font->render("Offense", offset_x+64, offset_y+66, JUSTIFY_CENTER, screen);
	font->render("Defense", offset_x+128, offset_y+66, JUSTIFY_CENTER, screen);
	font->render("Offense", offset_x+192, offset_y+66, JUSTIFY_CENTER, screen);
	font->render("Defense", offset_x+256, offset_y+66, JUSTIFY_CENTER, screen);
	
	// stats
	stringstream ss;
	ss.str("");
	ss << stats->physoff;
	font->render(ss.str(), offset_x+64, offset_y+34, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->physdef;
	font->render(ss.str(), offset_x+128, offset_y+34, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->magoff;
	font->render(ss.str(), offset_x+192, offset_y+34, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->magdef;
	font->render(ss.str(), offset_x+256, offset_y+34, JUSTIFY_CENTER, screen);
	
	
	
	// highlighting
	displayBuild(stats->physoff, offset_x+48);
	displayBuild(stats->physdef, offset_x+112);
	displayBuild(stats->magoff, offset_x+176);
	displayBuild(stats->magdef, offset_x+240);	
}

void MenuPowers::displayBuild(int value, int x) {
	SDL_Rect src_step;
	SDL_Rect src_unlock;
	SDL_Rect dest;
	
	src_step.x = src_unlock.x = 0;
	src_step.y = src_unlock.y = 0;
	src_step.w = 32;
	src_step.h = 19;
	src_unlock.w = 32;
	src_unlock.h = 45;

	dest.x = x;
	int offset_y = (VIEW_H - 416)/2;
	
	for (int i=3; i<= value; i++) {
		if (i%2 == 0) { // even stat
			dest.y = i * 32 + offset_y + 48;
			SDL_BlitSurface(powers_step, &src_step, screen, &dest);
		}
		else { // odd stat
			dest.y = i * 32 + offset_y + 35;
			SDL_BlitSurface(powers_unlock, &src_unlock, screen, &dest);
		
		}
	}
}

string MenuPowers::checkTooltip(Point mouse) {

	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	
	if (mouse.y >= offset_y+32 && mouse.y <= offset_y+80) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80)
			return "Physical + Offense grants melee and ranged attacks";
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144)
			return "Physical + Defense grants melee protection";
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208)
			return "Magical + Offense grants elemental spell attacks";
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272)
			return "Magical + Defense grants healing and magic protection";
	}
	else if (mouse.y >= offset_y+80 && mouse.y <= offset_y+112) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80)
			return "Shoot\nBasic Ranged Attack";
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144)
			return "Swing\nBasic Melee Attack";
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208)
			return "Lore\nIdentify magic items";
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272)
			return "Return\nWarp to your chosen Sanctuary";	
	}
	else if (mouse.y >= offset_y+144 && mouse.y <= offset_y+176) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80)
			return "Blood Strike\nCause target to Bleed for a few seconds\nRequires Physical Offense 3";
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144)
			return "Block\nIncrease avoidance and absorption\nRequires Physical Defense 3";
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208)
			return "Bolt\nConjure electricity that arcs between targets\nRequires Magical Offense 3";
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272)
			return "Heal\nRestore health\nRequires Magical Defense 3";	
	}
	else if (mouse.y >= offset_y+208 && mouse.y <= offset_y+240) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80)
			return "Multishot\nFire three projectiles in one shot\nRequires Physical Offense 5";
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144)
			return "Warcry\nBecome immune to debuffs for a short time and scare nearby enemies\nRequires Physical Defense 5";
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208)
			return "Quake\nShatter the earth and stun nearby enemies\nRequires Magical Offense 5";
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272)
			return "Shield\nAbsorbs damage\nRequires Magical Defense 5";		
	}
	else if (mouse.y >= offset_y+272 && mouse.y <= offset_y+304) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80)
			return "Cleave\nStrike all enemies in a wide arc\nRequires Physical Offense 7";
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144)
			return "Charge\nBull rush the target and knock it back\nRequires Physical Defense 7";
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208)
			return "Freeze\nSlow enemies in a straight line\nRequires Magical Offense 7";
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272)
			return "Teleport\nWarp instantly to the target location\nRequires Magical Defense 7";	
	}
	else if (mouse.y >= offset_y+336 && mouse.y <= offset_y+368) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80)
			return "Piercing Shot\nIgnore armor and shoot straight through enemies\nRequires Physical Offense 9";
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144)
			return "Vengeance\nAfter blocking, launch a heavy accurate attack\nRequires Physical Defense 9";
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208)
			return "Burn\nEvoke devastating flames in a distant area\nRequires Magical Offense 9";
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272)
			return "Time Stop\nPause the flow of time for a few moments\nRequires Magical Defense 9";		
	}
	
	return "";
}

MenuPowers::~MenuPowers() {
	SDL_FreeSurface(background);
	SDL_FreeSurface(powers_step);
	SDL_FreeSurface(powers_unlock);
}
