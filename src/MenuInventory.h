/**
 * class MenuInventory
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef MENU_INVENTORY_H
#define MENU_INVENTORY_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "FontEngine.h"
#include <string>
#include <sstream>

using namespace std;

class MenuInventory {
private:
	SDL_Surface *screen;
	FontEngine *font;

	void loadGraphics();
	SDL_Surface *background;
	SDL_Surface *equipped; // TODO: temp
public:
	MenuInventory(SDL_Surface *screen, FontEngine *font);
	~MenuInventory();
	void logic();
	void render();
	
	bool visible;

};

#endif

