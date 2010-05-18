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

#include "SDL.h"
#include "SDL_image.h"
#include "InputState.h"

class MenuActionBar {
private:
	SDL_Surface *screen;
	SDL_Surface *trim;
	SDL_Surface *background;
	SDL_Surface *attack; // temp
	
	InputState *inp;
	SDL_Rect src;
	SDL_Rect label_src;
	
	// for now the key mappings are static.  Just use an image for the labels
	SDL_Surface *labels;
	
public:
	MenuActionBar(SDL_Surface *_screen, InputState *_inp);
	~MenuActionBar();
	void loadGraphics();
	void logic();
	void render();
};

#endif
