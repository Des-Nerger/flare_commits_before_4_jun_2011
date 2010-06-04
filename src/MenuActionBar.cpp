/**
 * class MenuActionBar
 *
 * Handles the config, display, and usage of the 0-9 hotkeys, mouse buttons, and menu calls
 *
 * @author Clint Bellanger
 * @license GPL
 */
 
#include "MenuActionBar.h"

MenuActionBar::MenuActionBar(SDL_Surface *_screen, InputState *_inp) {
	screen = _screen;
	inp = _inp;
	
	src.x = 0;
	src.y = 0;
	src.w = 32;
	src.h = 32;
	label_src.x = 0;
	label_src.y = 0;
	label_src.w = 640;
	label_src.h = 10;
	
	loadGraphics();
}

void MenuActionBar::loadGraphics() {

	background = IMG_Load("images/menus/slot_empty.png");
	trim = IMG_Load("images/menus/actionbar_trim.png");
	labels = IMG_Load("images/menus/actionbar_labels.png");
	attack = IMG_Load("images/menus/attack.png");
	if(!background || !trim || !labels || !attack) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
}

void MenuActionBar::logic() {

}

void MenuActionBar::render() {

	SDL_Rect dest;
	// just a hardcoded demo for now
	SDL_Rect trimsrc;
	
	int offset_x = (VIEW_W - 640)/2;
	
	dest.x = offset_x;
	dest.y = VIEW_H-35;
	dest.w = 640;
	dest.h = 35;
	trimsrc.x = 0;
	trimsrc.y = 0;
	trimsrc.w = 640;
	trimsrc.h = 35;
	
	SDL_BlitSurface(trim, &trimsrc, screen, &dest);	
	
	dest.y = VIEW_H-32;
	dest.w = 32;
	dest.h = 32;	
	
	
	// attack power
	dest.x = offset_x + 32;
	SDL_BlitSurface(attack, &src, screen, &dest);
	
	for (int i=1; i<=9; i++) {
		dest.x = offset_x + (i * 32) + 32;
		SDL_BlitSurface(background, &src, screen, &dest);
	}
	
	dest.x = offset_x + 384;
	SDL_BlitSurface(attack, &src, screen, &dest);
	dest.x = offset_x + 416;
	SDL_BlitSurface(background, &src, screen, &dest);
	
	dest.x = offset_x;
	dest.y = VIEW_H-10;
	dest.w = 640;
	dest.h = 10;
	SDL_BlitSurface(labels, &label_src, screen, &dest);
	
}

string MenuActionBar::checkTooltip(Point mouse) {
	int offset_x = (VIEW_W - 640)/2;
	if (mouse.x >= offset_x+448 && mouse.x <= offset_x+512 && mouse.y >= VIEW_H-32 && mouse.y <= VIEW_H)
		return "Character Menu (C)";
	if (mouse.x >= offset_x+512 && mouse.x <= offset_x+544 && mouse.y >= VIEW_H-32 && mouse.y <= VIEW_H)
		return "Inventory Menu (I)";
	if (mouse.x >= offset_x+544 && mouse.x <= offset_x+576 && mouse.y >= VIEW_H-32 && mouse.y <= VIEW_H)
		return "Powers Menu (P)";
	if (mouse.x >= offset_x+576 && mouse.x <= offset_x+608 && mouse.y >= VIEW_H-32 && mouse.y <= VIEW_H)
		return "Log Menu (L)";

	return "";
}

MenuActionBar::~MenuActionBar() {
	SDL_FreeSurface(background);
	SDL_FreeSurface(trim);
	SDL_FreeSurface(labels);
	SDL_FreeSurface(attack);
}
