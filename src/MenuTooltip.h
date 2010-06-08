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

struct TooltipData {
	string lines[8];
	int colors[8];
	int num_lines;
	
	TooltipData() {
		num_lines = 0;
		for (int i=0; i<8; i++) {
			lines[i] = "";
			colors[i] = FONT_WHITE;
		}
	}
	
};

class MenuTooltip {
private:
	FontEngine *font;
	SDL_Surface *screen;
	int offset;
	int width;
	int margin;
	int margin_bottom;
public:
	MenuTooltip(FontEngine *_font, SDL_Surface *_screen);
	void render(TooltipData tip, Point pos);	
};

#endif
