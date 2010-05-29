/**
 * class MenuPowers
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef MENU_POWERS_H
#define MENU_POWERS_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "FontEngine.h"
#include "StatBlock.h"
#include <string>
#include <sstream>

using namespace std;

class MenuPowers {
private:
	SDL_Surface *screen;
	FontEngine *font;
	StatBlock *stats;
	
	SDL_Surface *background;
	SDL_Surface *powers_step;
	SDL_Surface *powers_unlock;
	void loadGraphics();
	void displayBuild(int value, int x);

public:
	MenuPowers(SDL_Surface *screen, FontEngine *font, StatBlock *stats);
	~MenuPowers();
	void logic();
	void render();
	string checkTooltip(Point mouse);
	
	bool visible;


};

#endif
