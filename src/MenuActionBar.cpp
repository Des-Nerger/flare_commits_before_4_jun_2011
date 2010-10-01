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
	drag_prev_slot = -1;
	
	// clear action bar
	for (int i=0; i<12; i++) {
		hotkeys[i] = -1;
	}
	
	// TEMP: set action bar positions
	// TODO: define in a config file so that the menu is customizable
	int offset_x = (VIEW_W - 640)/2;
	int offset_y = VIEW_H-32;
	
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
	
	// default: LMB set to basic melee attack
	hotkeys[10] = POWER_SWING;
		
	// screen areas occupied by the three main sections	
	numberArea.h = mouseArea.h = menuArea.h = 32;
	numberArea.y = mouseArea.y = menuArea.y = offset_y;
	numberArea.x = offset_x+32;
	numberArea.w = 320;
	mouseArea.x = offset_x+384;
	mouseArea.w = 64;
	menuArea.x = offset_x+480;
	menuArea.w = 128;
	
	loadGraphics();
}

void MenuActionBar::loadGraphics() {

	emptyslot = IMG_Load("images/menus/slot_empty.png");
	background = IMG_Load("images/menus/actionbar_trim.png");
	labels = IMG_Load("images/menus/actionbar_labels.png");
	attack = IMG_Load("images/menus/attack.png");
	if(!emptyslot || !background || !labels || !attack) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
}

/**
 * generic render 32-pixel icon
 */
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
	
	SDL_BlitSurface(background, &trimsrc, screen, &dest);	
	
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
			SDL_BlitSurface(emptyslot, &src, screen, &dest);
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
	
	//int offset_x = (VIEW_W - 640)/2;
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
void MenuActionBar::drop(Point mouse, int power_index, bool rearranging) {
	for (int i=0; i<12; i++) {
		if (isWithin(slots[i], mouse)) {
			if (rearranging) {
				hotkeys[drag_prev_slot] = hotkeys[i];
			}
			hotkeys[i] = power_index;
			return;
		}
	}
}

/**
 * CTRL-click a hotkey to clear it
 */
void MenuActionBar::remove(Point mouse) {
	for (int i=0; i<12; i++) {
		if (isWithin(slots[i], mouse)) {
			hotkeys[i] = -1;
			return;
		}
	}
}

/**
 * If pressing an action key, return 
 */
int MenuActionBar::checkAction(Point mouse) {

	// TODO: check clicking on stuff
	
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
	
	return -1;
}

/**
 * If clicking while a menu is open, assume the player wants to rearrange the action bar
 */
 int MenuActionBar::checkDrag(Point mouse) {
	int power_index;
	
	for (int i=0; i<12; i++) {
		if (isWithin(slots[i], mouse)) {
			drag_prev_slot = i;
			power_index = hotkeys[i];
			hotkeys[i] = -1;
			return power_index;
		}
	}
	
	return -1;
 }

/**
 * Set all hotkeys at once e.g. when loading a game
 */
void MenuActionBar::set(int power_id[12]) {
	for (int i=0; i<12; i++)
		hotkeys[i] = power_id[i];
}

MenuActionBar::~MenuActionBar() {
	SDL_FreeSurface(emptyslot);
	SDL_FreeSurface(background);
	SDL_FreeSurface(labels);
	SDL_FreeSurface(attack);
}
