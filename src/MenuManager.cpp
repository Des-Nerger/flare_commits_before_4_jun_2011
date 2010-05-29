/**
 * class MenuManager
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuManager.h"

MenuManager::MenuManager(SDL_Surface *_screen, InputState *_inp, FontEngine *_font, StatBlock *_stats) {
	screen = _screen;
	inp = _inp;
	font = _font;
	stats = _stats;
	
	inv = new MenuInventory(screen, font);
	pow = new MenuPowers(screen, font, stats);
	chr = new MenuCharacter(screen, font, stats);
	log = new MenuLog(screen, font);
	act = new MenuActionBar(screen, inp);
	hpmp = new MenuHealthMana(screen, font);
	tip = new MenuTooltip(font, screen);
	
	pause = false;
	loadSounds();
}

void MenuManager::loadSounds() {
	sfx_open = Mix_LoadWAV("soundfx/inventory/inventory_page.ogg");
	sfx_close = Mix_LoadWAV("soundfx/inventory/inventory_close.ogg");
	
	if (!sfx_open) {
		printf("Mix_LoadWAV: %s\n", Mix_GetError());
		SDL_Quit();
	}	
}


void MenuManager::logic() {

	if (!inp->pressing[INVENTORY] && !inp->pressing[POWERS] && !inp->pressing[CHARACTER] && !inp->pressing[LOG])
		key_lock = false;

	// inventory menu toggle
	if (inp->pressing[INVENTORY] && !key_lock) {
		key_lock = true;
		inv->visible = !inv->visible;
		if (inv->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			pow->visible = false;
		}
		else
			Mix_PlayChannel(-1, sfx_close, 0);
		
	}

	// powers menu toggle
	if (inp->pressing[POWERS] && !key_lock) {
		key_lock = true;
		pow->visible = !pow->visible;
		if (pow->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			inv->visible = false;
		}
		else
			Mix_PlayChannel(-1, sfx_close, 0);
	}

	// character menu toggle
	if (inp->pressing[CHARACTER] && !key_lock) {
		key_lock = true;
		chr->visible = !chr->visible;
		if (chr->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			log->visible = false;
		}
		else
			Mix_PlayChannel(-1, sfx_close, 0);
	}
	
	// log menu toggle
	if (inp->pressing[LOG] && !key_lock) {
		key_lock = true;
		log->visible = !log->visible;
		if (log->visible) {
			Mix_PlayChannel(-1, sfx_open, 0);
			chr->visible = false;
		}
		else
			Mix_PlayChannel(-1, sfx_close, 0);
	}
	
	pause = (inv->visible || pow->visible || chr->visible || log->visible);
}

void MenuManager::render() {
	hpmp->render(stats, inp->mouse);
	act->render();
	inv->render();
	pow->render();
	chr->render();
	log->render();
	
	string tooltip = "";
	
	if (inp->mouse.x < 320 && inp->mouse.y >= 32 && inp->mouse.y <= 448) {
		if (chr->visible) {
			tooltip = chr->checkTooltip(inp->mouse);
		}
	}
	else if (inp->mouse.x >= 320 && inp->mouse.y >= 32 && inp->mouse.y <= 448) {
		if (pow->visible) {
			tooltip = pow->checkTooltip(inp->mouse);
		}
	}
	else if (inp->mouse.y >= 448) {
		tooltip = act->checkTooltip(inp->mouse);
	}
	
	if (tooltip != "") {
		tip->render(tooltip, inp->mouse);
	}
}

void MenuManager::closeAll() {
	inv->visible = false;
	pow->visible = false;
	chr->visible = false;
	log->visible = false;
		
	Mix_PlayChannel(-1, sfx_close, 0);
}

MenuManager::~MenuManager() {
	delete(inv);
	delete(pow);
	delete(chr);
	delete(log);
	delete(act);
	delete(tip);
	Mix_FreeChunk(sfx_open);
	Mix_FreeChunk(sfx_close);
	
}
