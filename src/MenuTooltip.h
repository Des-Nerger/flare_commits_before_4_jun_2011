/**
 * class MenuTooltip
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef MENU_TOOLTIP_H
#define MENU_TOOLTIP_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "FontEngine.h"
#include "Utils.h"
#include "Settings.h"

class MenuTooltip {
private:
	FontEngine *font;
	SDL_Surface *screen;
	int offset;
	int width;
	int margin;
public:
	MenuTooltip(FontEngine *_font, SDL_Surface *_screen);
	void render(string text, Point pos);	
};

#endif