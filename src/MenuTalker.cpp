/**
 * class MenuTalker
 *
 * @author morris989
 * @license GPL
 */

#include "MenuTalker.h"

MenuTalker::MenuTalker(SDL_Surface *_screen, FontEngine *_font) {
	screen = _screen;
	font = _font;
	
	visible = false;
	loadGraphics();
	loadTalker("");
	
}

void MenuTalker::loadGraphics() {

	background = IMG_Load("images/menus/talker.png");
	if(!background) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
	// optimize
	SDL_Surface *cleanup = background;
	background = SDL_DisplayFormatAlpha(background);
	SDL_FreeSurface(cleanup);	
	
}

void MenuTalker::loadTalker(string filename) {

}


void MenuTalker::logic() {

}

void MenuTalker::render() {
	if (!visible) return;
	SDL_Rect src;
	SDL_Rect dest;
	
	int offset_y = (VIEW_H - 416)/2;
	
	// background
	src.x = 0;
	src.y = 0;
	dest.x = 0;
	dest.y = offset_y;
	src.w = dest.w = 320;
	src.h = dest.h = 416;
	SDL_BlitSurface(background, &src, screen, &dest);
		
	// text overlay
	// TODO: translate()
	font->render(npc->name, 160, offset_y+8, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render(npc->txt, 40, offset_y+40, JUSTIFY_LEFT, screen, 224, FONT_WHITE);
}


MenuTalker::~MenuTalker() {
	SDL_FreeSurface(background);
}
