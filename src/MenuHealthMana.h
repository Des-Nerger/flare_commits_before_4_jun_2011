/**
 * MenuHealthMana
 *
 * Handles the display of the Health and Mana bars at the top/left of the screen
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef MENU_HEALTH_MANA_H
#define MENU_HEALTH_MANA_H

#include "SDL.h"
#include "SDL_image.h"
#include "StatBlock.h"
#include "Utils.h"
#include "FontEngine.h"
#include <string>
#include <sstream>

using namespace std;

class MenuHealthMana {
private:
	SDL_Surface *screen;
	FontEngine *font;
	SDL_Surface *background;
	SDL_Surface *bar_red;
	SDL_Surface *bar_green;
	SDL_Surface *bar_blue;
public:
	MenuHealthMana(SDL_Surface *_screen, FontEngine *_font);
	~MenuHealthMana();
	void loadGraphics();
	void render(StatBlock *stats, Point mouse);
};

#endif
