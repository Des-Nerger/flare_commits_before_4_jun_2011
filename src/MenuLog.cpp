/**
 * class MenuLog
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuLog.h"

MenuLog::MenuLog(SDL_Surface *_screen, FontEngine *_font) {
	screen = _screen;
	font = _font;
	
	visible = false;
	loadGraphics();
}

void MenuLog::loadGraphics() {

	background = IMG_Load("images/menus/log.png");
	if(!background) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
}

void MenuLog::render() {
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
	font->render("Log", 160, offset_y+8, JUSTIFY_CENTER, screen, FONT_WHITE);

}

MenuLog::~MenuLog() {
	SDL_FreeSurface(background);
}
