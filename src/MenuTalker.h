/**
 * class MenuTalker
 *
 * @author morris989
 * @license GPL
 */

#ifndef MENU_TALKER_H
#define MENU_TALKER_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "FontEngine.h"
#include "NPC.h"
#include <string>
#include <sstream>


class MenuTalker {
private:
	SDL_Surface *screen;
	FontEngine *font;

	void loadGraphics();
	SDL_Surface *background;

public:
	MenuTalker(SDL_Surface *screen, FontEngine *font);
	~MenuTalker();

	NPC *npc;
	
	void loadTalker(string filename);
	void logic();
	void render();

	
	bool visible;

	
};

#endif
