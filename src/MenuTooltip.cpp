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
void MenuTooltip::render(TooltipData tip, Point pos) {
	SDL_Rect background;
	
	string fulltext;
	
	fulltext = tip.lines[0];
	for (int i=1; i<tip.num_lines; i++) {
		fulltext = fulltext + "\n" + tip.lines[i];
	}
	
	Point size = font->calc_size(fulltext, width);
	background.w = size.x + margin + margin;
	background.h = size.y + margin + margin_bottom;
	
	int cursor_x;
	int cursor_y;
	
	// upper left
	if (pos.x < VIEW_W_HALF && pos.y < VIEW_H_HALF) {
		background.x = pos.x + offset;
		background.y = pos.y + offset;
		SDL_FillRect(screen, &background, 0);
		cursor_x = pos.x + offset + margin;
		cursor_y = pos.y + offset + margin;
	}
	// upper right
	else if (pos.x >= VIEW_W_HALF && pos.y < VIEW_H_HALF) {
		background.x = pos.x - offset - size.x - margin - margin;
		background.y = pos.y + offset;
		SDL_FillRect(screen, &background, 0);
		cursor_x = pos.x - offset - size.x - margin;
		cursor_y = pos.y + offset + margin;
	}
	// lower left
	else if (pos.x < VIEW_W_HALF && pos.y >= VIEW_H_HALF) {
		background.x = pos.x + offset;
		background.y = pos.y - offset - size.y - margin - margin;
		SDL_FillRect(screen, &background, 0);
		cursor_x = pos.x + offset + margin;
		cursor_y = pos.y - offset - size.y - margin;
	}
	// lower right
	else if (pos.x >= VIEW_W_HALF && pos.y >= VIEW_H_HALF) {
		background.x = pos.x - offset - size.x - margin - margin;		
		background.y = pos.y - offset - size.y - margin - margin;
		SDL_FillRect(screen, &background, 0);
		cursor_x = pos.x - offset - size.x - margin;
		cursor_y = pos.y - offset - size.y - margin;
	}
	
	for (int i=0; i<tip.num_lines; i++) {
		font->render(tip.lines[i], cursor_x, cursor_y, JUSTIFY_LEFT, screen, size.x, tip.colors[i]);
		cursor_y = font->cursor_y;
	}
			
}
