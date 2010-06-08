/**
 * MenuHealthMana
 *
 * Handles the display of the Health and Mana bars at the top/left of the screen
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuHealthMana.h"

MenuHealthMana::MenuHealthMana(SDL_Surface *_screen, FontEngine *_font) {
	screen = _screen;
	font = _font;
	
	loadGraphics();
}

void MenuHealthMana::loadGraphics() {

	background = IMG_Load("images/menus/bar_hp_mp.png");
	bar_red = IMG_Load("images/menus/bar_red.png");
	bar_green = IMG_Load("images/menus/bar_green.png");
	bar_blue = IMG_Load("images/menus/bar_blue.png");
	if(!background || !bar_red || !bar_green || !bar_blue) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
}

void MenuHealthMana::render(StatBlock *stats, Point mouse) {
	SDL_Rect src;
	SDL_Rect dest;
	int hp_bar_length;
	int mp_bar_length;
	
	// draw trim/background
	src.x = dest.x = 0;
	src.y = dest.y = 0;
	src.w = dest.w = 106;
	src.h = dest.h = 33;
	
	SDL_BlitSurface(background, &src, screen, &dest);
	
	if (stats->maxhp == 0)
		hp_bar_length = 0;
	else
		hp_bar_length = (stats->hp * 100) / stats->maxhp;
		
	if (stats->maxmp == 0)
		mp_bar_length = 0;
	else
		mp_bar_length = (stats->mp * 100) / stats->maxmp;
	
	// draw hp bar
	src.x = 0;
	src.y = 0;
	src.w = 1;
	src.h = 12;
	dest.y = 3;
	dest.w = 1;
	dest.h = 1;
	for (int i=0; i<hp_bar_length; i++) {
		dest.x = i + 3;
		if (hp_bar_length < 50) {
			SDL_BlitSurface(bar_red, &src, screen, &dest);
		}
		else {
			SDL_BlitSurface(bar_green, &src, screen, &dest);		
		}
	}
	dest.y = 18;
	for (int i=0; i<mp_bar_length; i++) {
		dest.x = i + 3;
		SDL_BlitSurface(bar_blue, &src, screen, &dest);		
	}
	
	// if mouseover, draw text
	if (mouse.x <= 106 && mouse.y <= 33) {

		stringstream ss;
		ss << stats->hp << "/" << stats->maxhp;
		font->render(ss.str(), 5,4,JUSTIFY_LEFT, screen, FONT_WHITE);
		ss.str("");
		ss << stats->mp << "/" << stats->maxmp;
		font->render(ss.str(), 5,19,JUSTIFY_LEFT, screen, FONT_WHITE);
	 
	}
}

MenuHealthMana::~MenuHealthMana() {
	SDL_FreeSurface(background);
	SDL_FreeSurface(bar_red);
	SDL_FreeSurface(bar_green);
	SDL_FreeSurface(bar_blue);
}

