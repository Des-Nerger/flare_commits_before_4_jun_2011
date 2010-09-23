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
#include "MenuTooltip.h"
#include "ItemDatabase.h"
#include "PowerManager.h"
#include "MenuMiniMap.h"
#include "MenuExperience.h"

const int DRAG_SRC_POWERS = 1;
const int DRAG_SRC_INVENTORY = 2;

class MenuManager {
private:
	
	Mix_Chunk *sfx_open;
	Mix_Chunk *sfx_close;
	SDL_Surface *icons;

	PowerManager *powers;
	StatBlock *stats;
	InputState *inp;
	FontEngine *font;
	SDL_Surface *screen;
	

		
	bool key_lock;
	bool rightclick_lock;
	void loadSounds();
	void loadIcons();
	
	bool dragging;
	int drag_item;
	int drag_power;
	int drag_src;

	
public:
	MenuManager(PowerManager *powers, SDL_Surface *screen, InputState *inp, FontEngine *font, StatBlock *stats);
	~MenuManager();
	void logic();
	void render();
	void renderIcon(int icon_id, int x, int y);
	void closeAll();

	MenuInventory *inv;
	MenuPowers *pow;
	MenuCharacter *chr;
	MenuLog *log;
	MenuActionBar *act;
	MenuHealthMana *hpmp;
	MenuTooltip *tip;
	MenuMiniMap *mini;
	MenuExperience *xp;
	ItemDatabase *items;
	
	
	bool pause;
	int drop_item;	

};

#endif
