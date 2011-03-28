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

	background = IMG_Load("images/menus/dialog_box.png");
	if(!background) {
		fprintf(stderr, "Couldn't load image dialog_box.png: %s\n", IMG_GetError());
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
	string line;
	
	int offset_x = (VIEW_W - 640)/2;
	int offset_y = (VIEW_H - 416)/2;
	
	// dialog box
	src.x = 0;
	src.y = 0;
	dest.x = offset_x;
	dest.y = offset_y + 320;
	src.w = dest.w = 640;
	src.h = dest.h = 96;
	SDL_BlitSurface(background, &src, screen, &dest);
	
	// show active portrait
	
	// text overlay
	// TODO: translate()
	line = npc->name + ": " + npc->txt;
	font->render(line, offset_x+48, offset_y+336, JUSTIFY_LEFT, screen, 544, FONT_WHITE);
}


MenuTalker::~MenuTalker() {
	SDL_FreeSurface(background);
}
