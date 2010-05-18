/**
 * class MenuInventory
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuInventory.h"

MenuInventory::MenuInventory(SDL_Surface *_screen, FontEngine *_font) {
	screen = _screen;
	font = _font;
	
	visible = false;
	loadGraphics();
}

void MenuInventory::loadGraphics() {

	background = IMG_Load("images/menus/inventory.png");
	equipped = IMG_Load("images/menus/equipped_temp.png");
	if(!background || !equipped) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
}

void MenuInventory::render() {
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
	font->render("Inventory", 480, 40, JUSTIFY_CENTER, screen);
	font->render("Main Hand", 384, 66, JUSTIFY_CENTER, screen);
	font->render("Body", 448, 66, JUSTIFY_CENTER, screen);
	font->render("Off Hand", 512, 66, JUSTIFY_CENTER, screen);
	font->render("Artifact", 576, 66, JUSTIFY_CENTER, screen);
	
	// data
	// TODO: temp equipped
	src.w = dest.w = 192;
	src.h = dest.h = 64;
	dest.x = 352;
	dest.y = 80;
	SDL_BlitSurface(equipped, &src, screen, &dest);
}

MenuInventory::~MenuInventory() {
	SDL_FreeSurface(background);
	SDL_FreeSurface(equipped);
}
