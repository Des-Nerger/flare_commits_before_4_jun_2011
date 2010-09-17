/**
 * class MenuActionBar
 *
 * Handles the config, display, and usage of the 0-9 hotkeys, mouse buttons, and menu calls
 *
 * @author Clint Bellanger
 * @license GPL
 */
 
#include "MenuActionBar.h"

MenuActionBar::MenuActionBar(PowerManager *_powers, SDL_Surface *_screen, InputState *_inp, SDL_Surface *_icons) {
	powers = _powers;
	screen = _screen;
	inp = _inp;
	icons = _icons;
	
	src.x = 0;
	src.y = 0;
	src.w = 32;
	src.h = 32;
	label_src.x = 0;
	label_src.y = 0;
	label_src.w = 640;
	label_src.h = 10;
	
	// clear action bar
	for (int i=0; i<12; i++) {
		hotkeys[i] = -1;
	}
	
	// TEMP: set action bar positions
	// TODO: define in a config file so that the menu is customizable
	int offset_x = (VIEW_W - 640)/2;
	for (int i=0; i<12; i++) {
		slots[i].w = slots[i].h = 32;
		slots[i].y = VIEW_H-32;
		slots[i].x = offset_x + i*32 + 32;
	}
	slots[10].x += 32;
	slots[11].x += 32;
	// menu button positions
	for (int i=0; i<4; i++) {
		menus[i].w = menus[i].h = 32;
		menus[i].y = VIEW_H-32;
		menus[i].x = offset_x + 480 + i*32;
	}
	
	// TEMP: test hotkeys
	hotkeys[0] = hotkeys[10] = POWER_SWING;
	hotkeys[1] = POWER_SHOOT;
	hotkeys[2] = hotkeys[11] = POWER_SHOCK;
	hotkeys[3] = POWER_BURN;
	hotkeys[4] = POWER_QUAKE;
	hotkeys[5] = POWER_HEAL;
	hotkeys[6] = POWER_TIMESTOP;
		
	loadGraphics();
}

void MenuActionBar::loadGraphics() {

	background = IMG_Load("images/menus/slot_empty.png");
	trim = IMG_Load("images/menus/actionbar_trim.png");
	labels = IMG_Load("images/menus/actionbar_labels.png");
	attack = IMG_Load("images/menus/attack.png");
	if(!background || !trim || !labels || !attack) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
}

void MenuActionBar::renderIcon(int icon_id, int x, int y) {
	SDL_Rect src;
	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;
	src.w = src.h = dest.w = dest.h = 32;
	src.x = (icon_id % 16) * 32;
	src.y = (icon_id / 16) * 32;
	SDL_BlitSurface(icons, &src, screen, &dest);		
}

void MenuActionBar::logic() {
}



void MenuActionBar::render() {

	SDL_Rect dest;
	// just a hardcoded demo for now
	SDL_Rect trimsrc;
	
	int offset_x = (VIEW_W - 640)/2;
	
	dest.x = offset_x;
	dest.y = VIEW_H-35;
	dest.w = 640;
	dest.h = 35;
	trimsrc.x = 0;
	trimsrc.y = 0;
	trimsrc.w = 640;
	trimsrc.h = 35;
	
	SDL_BlitSurface(trim, &trimsrc, screen, &dest);	
	
	// draw hotkeyed icons
	src.x = src.y = 0;
	src.w = src.h = dest.w = dest.h = 32;
	dest.y = VIEW_H-32;	
	for (int i=0; i<12; i++) {

		if (i<=9)
			dest.x = offset_x + (i * 32) + 32;
		else
			dest.x = offset_x + (i * 32) + 64;

		if (hotkeys[i] != -1)
			renderIcon(powers->powers[hotkeys[i]].icon, dest.x, dest.y);
		else
			SDL_BlitSurface(background, &src, screen, &dest);
	}
		
	// draw hotkey labels
	// TODO: keybindings
	dest.x = offset_x;
	dest.y = VIEW_H-10;
	dest.w = 640;
	dest.h = 10;
	SDL_BlitSurface(labels, &label_src, screen, &dest);
	
}

/**
 * On mouseover, show tooltip for buttons
 */
TooltipData MenuActionBar::checkTooltip(Point mouse) {
	TooltipData tip;
	
	int offset_x = (VIEW_W - 640)/2;
	if (isWithin(menus[0], mouse)) {
		tip.lines[tip.num_lines++] = "Character Menu (C)";
		return tip;
	}
	if (isWithin(menus[1], mouse)) {
		tip.lines[tip.num_lines++] = "Inventory Menu (I)";
		return tip;
	}
	if (isWithin(menus[2], mouse)) {
		tip.lines[tip.num_lines++] = "Powers Menu (P)";
		return tip;
	}
	if (isWithin(menus[3], mouse)) {
		tip.lines[tip.num_lines++] = "Log Menu (L)";
		return tip;
	}
	for (int i=0; i<12; i++) {
		if (hotkeys[i] != -1) {
			if (isWithin(slots[i], mouse)) {
				tip.lines[tip.num_lines++] = powers->powers[hotkeys[i]].name;
			}
		}
	}

	return tip;
}

/**
 * After dragging a power or item onto the action bar, set as new hotkey
 */
void MenuActionBar::drop(Point mouse, int power_index) {


}

/**
 * If pressing an action key, return 
 */
int MenuActionBar::checkAction() {
	if (inp->pressing[BAR_1]) return hotkeys[0];
	if (inp->pressing[BAR_2]) return hotkeys[1];
	if (inp->pressing[BAR_3]) return hotkeys[2];
	if (inp->pressing[BAR_4]) return hotkeys[3];
	if (inp->pressing[BAR_5]) return hotkeys[4];
	if (inp->pressing[BAR_6]) return hotkeys[5];
	if (inp->pressing[BAR_7]) return hotkeys[6];
	if (inp->pressing[BAR_8]) return hotkeys[7];
	if (inp->pressing[BAR_9]) return hotkeys[8];
	if (inp->pressing[BAR_0]) return hotkeys[9];
	if (inp->pressing[MAIN1] && !inp->mouse_lock) return hotkeys[10];
	if (inp->pressing[MAIN2] && !inp->mouse_lock) return hotkeys[11];
	
	// also check clicking on stuff
	
	return -1;
}


MenuActionBar::~MenuActionBar() {
	SDL_FreeSurface(background);
	SDL_FreeSurface(trim);
	SDL_FreeSurface(labels);
	SDL_FreeSurface(attack);
}
