/*
 *  MenuTooltip.cpp
 *  RPGEngine
 *
 *  Created by Clint Bellanger on 5/29/10.
 *  Copyright 2010 Clint Bellanger. All rights reserved.
 *
 */

#include "MenuTooltip.h"

MenuTooltip::MenuTooltip(FontEngine *_font, SDL_Surface *_screen) {
	font = _font;
	screen = _screen;
	offset=12;
	width=160;
	margin=4;
	
	// make the bottom margin smaller for visual balance
	// (adjust for line height and low hanging characters like g,j,p,q,y)
	margin_bottom=1;
}

/**
 * Tooltip position depends on the screen quadrant of the mouse
 */
void MenuTooltip::render(string text, Point pos) {
	SDL_Rect background;
	
	Point size = font->calc_size(text, width);
	background.w = size.x + margin + margin;
	background.h = size.y + margin + margin_bottom;
	
	// upper left
	if (pos.x < 320 && pos.y < 240) {
		background.x = pos.x + offset;
		background.y = pos.y + offset;
		SDL_FillRect(screen, &background, 0);
		font->render(text, pos.x + offset + margin, pos.y + offset + margin, JUSTIFY_LEFT, screen, size.x);
	}
	// upper right
	else if (pos.x >= 320 && pos.y < 240) {
		background.x = pos.x - offset - size.x - margin - margin;
		background.y = pos.y + offset;
		SDL_FillRect(screen, &background, 0);
		font->render(text, pos.x - offset - size.x - margin, pos.y + offset + margin, JUSTIFY_LEFT, screen, size.x);	
	}
	// lower left
	else if (pos.x < 320 && pos.y >= 240) {
		background.x = pos.x + offset;
		background.y = pos.y - offset - size.y - margin - margin;
		SDL_FillRect(screen, &background, 0);
		font->render(text, pos.x + offset + margin, pos.y - offset - size.y - margin, JUSTIFY_LEFT, screen, size.x);	
	}
	// lower right
	else if (pos.x >= 320 && pos.y >= 240) {
		background.x = pos.x - offset - size.x - margin - margin;		
		background.y = pos.y - offset - size.y - margin - margin;
		SDL_FillRect(screen, &background, 0);
		font->render(text, pos.x - offset - size.x - margin, pos.y - offset - size.y - margin, JUSTIFY_LEFT, screen, size.x);
	}
			
}