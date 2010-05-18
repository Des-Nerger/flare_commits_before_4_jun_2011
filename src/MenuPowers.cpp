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

MenuPowers::~MenuPowers() {
	SDL_FreeSurface(background);
	SDL_FreeSurface(powers_step);
	SDL_FreeSurface(powers_unlock);
}
