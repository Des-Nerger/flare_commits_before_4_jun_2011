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

const int MENU_CHARACTER = 0;
const int MENU_INVENTORY = 1;
const int MENU_POWERS = 2;
const int MENU_LOG = 3;

class MenuActionBar {
private:
	SDL_Surface *screen;
	SDL_Surface *background;
	SDL_Surface *emptyslot;
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
	int checkAction(Point mouse);
	int checkDrag(Point mouse);
	void checkMenu(Point mouse, bool &menu_c, bool &menu_i, bool &menu_p, bool &menu_l);
	void drop(Point mouse, int power_index, bool rearranging);
	void remove(Point mouse);
	void set(int power_id[12]);

	TooltipData checkTooltip(Point mouse);
	
	int hotkeys[12]; // refer to power_index in PowerManager
	SDL_Rect slots[12]; // the location of hotkey slots
	SDL_Rect menus[4]; // the location of the menu buttons
	
	// these store the area occupied by these hotslot sections.
	// useful for detecting mouse interactions on those locations
	SDL_Rect numberArea;
	SDL_Rect mouseArea;
	SDL_Rect menuArea;
	int drag_prev_slot;
	
};

#endif
