/**
 * class ActionBar
 *
 * Handles the config, display, and usage of the 0-9 hotkeys, mouse buttons, and menu calls
 *
 * @author Clint Bellanger
 * @license GPL
 */
 
#ifndef MENU_ACTION_BAR_H
#define MENU_ACTION_BAR_H

#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "InputState.h"
#include "MenuTooltip.h"
#include "PowerManager.h"

class MenuActionBar {
private:
	SDL_Surface *screen;
	SDL_Surface *trim;
	SDL_Surface *background;
	SDL_Surface *attack; // temp
	SDL_Surface *icons;
	
	PowerManager *powers;
	InputState *inp;
	SDL_Rect src;
	SDL_Rect label_src;
	
	// for now the key mappings are static.  Just use an image for the labels
	SDL_Surface *labels;
	
public:
	MenuActionBar(PowerManager *_powers, SDL_Surface *_screen, InputState *_inp, SDL_Surface *icons);
	~MenuActionBar();
	void loadGraphics();
	void renderIcon(int icon_id, int x, int y);
	void logic();
	void render();
	int checkAction();
	TooltipData checkTooltip(Point mouse);
	
	int hotkeys[12]; // refer to power_index in PowerManager
};

#endif
