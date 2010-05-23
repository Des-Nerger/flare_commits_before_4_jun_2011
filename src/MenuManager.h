/**
 * class MenuManager
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "FontEngine.h"
#include "InputState.h"
#include "MenuInventory.h"
#include "MenuPowers.h"
#include "MenuCharacter.h"
#include "MenuLog.h"
#include "StatBlock.h"
#include "MenuActionBar.h"
#include "MenuHealthMana.h"

class MenuManager {
private:
	MenuInventory *inv;
	MenuPowers *pow;
	MenuCharacter *chr;
	MenuLog *log;
	MenuActionBar *act;
	MenuHealthMana *hpmp;
	Mix_Chunk *sfx_open;
	Mix_Chunk *sfx_close;
	
	
	StatBlock *stats;
	InputState *inp;
	FontEngine *font;
	SDL_Surface *screen;
	
	bool key_lock;
	void loadSounds();
	
public:
	MenuManager(SDL_Surface *screen, InputState *inp, FontEngine *font, StatBlock *stats);
	~MenuManager();
	void logic();
	void render();
	void closeAll();
	
	bool pause;
	

};

#endif
