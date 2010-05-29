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
	
	// background
	src.x = 0;
	src.y = 0;
	dest.x = 320;
	dest.y = 32;
	src.w = dest.w = 320;
	src.h = dest.h = 416;
	SDL_BlitSurface(background, &src, screen, &dest);
	
	// text overlay
	// TODO: translate()
	font->render("Powers", 480, 40, JUSTIFY_CENTER, screen);
	font->render("Physical", 384, 82, JUSTIFY_CENTER, screen);
	font->render("Physical", 448, 82, JUSTIFY_CENTER, screen);
	font->render("Magical", 512, 82, JUSTIFY_CENTER, screen);
	font->render("Magical", 576, 82, JUSTIFY_CENTER, screen);
	font->render("Offense", 384, 98, JUSTIFY_CENTER, screen);
	font->render("Defense", 448, 98, JUSTIFY_CENTER, screen);
	font->render("Offense", 512, 98, JUSTIFY_CENTER, screen);
	font->render("Defense", 576, 98, JUSTIFY_CENTER, screen);
	
	// stats
	stringstream ss;
	ss.str("");
	ss << stats->physoff;
	font->render(ss.str(), 384, 66, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->physdef;
	font->render(ss.str(), 448, 66, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->magoff;
	font->render(ss.str(), 512, 66, JUSTIFY_CENTER, screen);
	ss.str("");
	ss << stats->magdef;
	font->render(ss.str(), 576, 66, JUSTIFY_CENTER, screen);
	
	
	
	// highlighting
	displayBuild(stats->physoff, 368);
	displayBuild(stats->physdef, 432);
	displayBuild(stats->magoff, 496);
	displayBuild(stats->magdef, 560);	
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
	
	for (int i=3; i<= value; i++) {
		if (i%2 == 0) { // even stat
			dest.y = i * 32 + 80;
			SDL_BlitSurface(powers_step, &src_step, screen, &dest);
		}
		else { // odd stat
			dest.y = i * 32 + 67;
			SDL_BlitSurface(powers_unlock, &src_unlock, screen, &dest);
		
		}
	}
}

string MenuPowers::checkTooltip(Point mouse) {
	if (mouse.x >= 368 && mouse.x <= 400 && mouse.y >= 64 && mouse.y <= 112)
		return "Physical + Offense grants melee and ranged attacks";
	if (mouse.x >= 432 && mouse.x <= 464 && mouse.y >= 64 && mouse.y <= 112)
		return "Physical + Defense grants melee protection";
	if (mouse.x >= 496 && mouse.x <= 528 && mouse.y >= 64 && mouse.y <= 112)
		return "Magical + Offense grants elemental spell attacks";
	if (mouse.x >= 560 && mouse.x <= 592 && mouse.y >= 64 && mouse.y <= 112)
		return "Magical + Defense grants healing and magic protection";
			
	// tier 1 powers
	if (mouse.x >= 368 && mouse.x <= 400 && mouse.y >= 112 && mouse.y <= 144)
		return "Shoot\nBasic Ranged Attack";
	if (mouse.x >= 432 && mouse.x <= 464 && mouse.y >= 112 && mouse.y <= 144)
		return "Swing\nBasic Melee Attack";
	if (mouse.x >= 496 && mouse.x <= 528 && mouse.y >= 112 && mouse.y <= 144)
		return "Lore\nIdentify magic items";
	if (mouse.x >= 560 && mouse.x <= 592 && mouse.y >= 112 && mouse.y <= 144)
		return "Return\nWarp to your chosen Sanctuary";	
	
	// tier 3 powers
	if (mouse.x >= 368 && mouse.x <= 400 && mouse.y >= 176 && mouse.y <= 208)
		return "Blood Strike\nCause target to Bleed for a few seconds\nRequires Physical Offense 3";
	if (mouse.x >= 432 && mouse.x <= 464 && mouse.y >= 176 && mouse.y <= 208)
		return "Block\nIncrease avoidance and absorption\nRequires Physical Defense 3";
	if (mouse.x >= 496 && mouse.x <= 528 && mouse.y >= 176 && mouse.y <= 208)
		return "Bolt\nConjure electricity that arcs between targets\nRequires Magical Offense 3";
	if (mouse.x >= 560 && mouse.x <= 592 && mouse.y >= 176 && mouse.y <= 208)
		return "Heal\nRestore health\nRequires Magical Defense 3";	
	
	// tier 5 powers
	if (mouse.x >= 368 && mouse.x <= 400 && mouse.y >= 240 && mouse.y <= 272)
		return "Multishot\nFire three projectiles in one shot\nRequires Physical Offense 5";
	if (mouse.x >= 432 && mouse.x <= 464 && mouse.y >= 240 && mouse.y <= 272)
		return "Warcry\nBecome immune to debuffs for a short time and scare nearby enemies\nRequires Physical Defense 5";
	if (mouse.x >= 496 && mouse.x <= 528 && mouse.y >= 240 && mouse.y <= 272)
		return "Quake\nShatter the earth and stun nearby enemies\nRequires Magical Offense 5";
	if (mouse.x >= 560 && mouse.x <= 592 && mouse.y >= 240 && mouse.y <= 272)
		return "Shield\nAbsorbs damage\nRequires Magical Defense 5";		

	// tier 7 powers
	if (mouse.x >= 368 && mouse.x <= 400 && mouse.y >= 304 && mouse.y <= 336)
		return "Cleave\nStrike all enemies in a wide arc\nRequires Physical Offense 7";
	if (mouse.x >= 432 && mouse.x <= 464 && mouse.y >= 304 && mouse.y <= 336)
		return "Charge\nBull rush the target and knock it back\nRequires Physical Defense 7";
	if (mouse.x >= 496 && mouse.x <= 528 && mouse.y >= 304 && mouse.y <= 336)
		return "Freeze\nSlow enemies in a straight line\nRequires Magical Offense 7";
	if (mouse.x >= 560 && mouse.x <= 592 && mouse.y >= 304 && mouse.y <= 336)
		return "Teleport\nWarp instantly to the target location\nRequires Magical Defense 7";	
	
	// tier 9 powers
	if (mouse.x >= 368 && mouse.x <= 400 && mouse.y >= 368 && mouse.y <= 400)
		return "Piercing Shot\nIgnore armor and shoot straight through enemies\nRequires Physical Offense 9";
	if (mouse.x >= 432 && mouse.x <= 464 && mouse.y >= 368 && mouse.y <= 400)
		return "Vengeance\nAfter blocking, launch a heavy accurate attack\nRequires Physical Defense 9";
	if (mouse.x >= 496 && mouse.x <= 528 && mouse.y >= 368 && mouse.y <= 400)
		return "Burn\nEvoke devastating flames in a distant area\nRequires Magical Offense 9";
	if (mouse.x >= 560 && mouse.x <= 592 && mouse.y >= 368 && mouse.y <= 400)
		return "Time Stop\nPause the flow of time for a few moments\nRequires Magical Defense 9";		
	
	return "";
}

MenuPowers::~MenuPowers() {
	SDL_FreeSurface(background);
	SDL_FreeSurface(powers_step);
	SDL_FreeSurface(powers_unlock);
}
