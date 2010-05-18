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
	
	pause = false;
}

void MenuManager::logic() {

	if (!inp->pressing[INVENTORY] && !inp->pressing[POWERS] && !inp->pressing[CHARACTER] && !inp->pressing[LOG])
		key_lock = false;

	// inventory menu toggle
	if (inp->pressing[INVENTORY] && !key_lock) {
		key_lock = true;
		inv->visible = !inv->visible;
		if (inv->visible) pow->visible = false;
	}

	// powers menu toggle
	if (inp->pressing[POWERS] && !key_lock) {
		key_lock = true;
		pow->visible = !pow->visible;
		if (pow->visible) inv->visible = false;
	}

	// character menu toggle
	if (inp->pressing[CHARACTER] && !key_lock) {
		key_lock = true;
		chr->visible = !chr->visible;
		if (chr->visible) log->visible = false;
	}
	
	// log menu toggle
	if (inp->pressing[LOG] && !key_lock) {
		key_lock = true;
		log->visible = !log->visible;
		if (log->visible) chr->visible = false;
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
}

void MenuManager::closeAll() {
	inv->visible = false;
	pow->visible = false;
	chr->visible = false;
	log->visible = false;
}

MenuManager::~MenuManager() {
	delete(inv);
	delete(pow);
	delete(chr);
	delete(log);
	delete(act);
}
