/**
 * class MenuLog
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef MENU_LOG_H
#define MENU_LOG_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "FontEngine.h"

class MenuLog {
private:
	SDL_Surface *screen;
	FontEngine *font;

	SDL_Surface *background;
	void loadGraphics();
public:
	MenuLog(SDL_Surface *screen, FontEngine *font);
	~MenuLog();

	void logic();
	void render();
	
	bool visible;

};

#endif
