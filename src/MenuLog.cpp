/**
 * class MenuLog
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuLog.h"

MenuLog::MenuLog(SDL_Surface *_screen, FontEngine *_font) {
	screen = _screen;
	font = _font;
	log_count = 0;
	visible = false;
	loadGraphics();
}

void MenuLog::loadGraphics() {

	background = IMG_Load("images/menus/log.png");
	if(!background) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
}

/**
 * Perform one frame of logic
 * Age messages
 */
void MenuLog::logic() {
	for (int i=0; i<log_count; i++)
		if (msg_age[i] > 0) msg_age[i]--;
}

/**
 * Render graphics for this frame when the menu is open
 */
void MenuLog::render() {
	if (!visible) return;
	
	SDL_Rect src;
	SDL_Rect dest;
	int offset_y = (VIEW_H - 416)/2;
	
	// background
	src.x = 0;
	src.y = 0;
	dest.x = 0;
	dest.y = offset_y;
	src.w = dest.w = 320;
	src.h = dest.h = 416;
	SDL_BlitSurface(background, &src, screen, &dest);
	
	// text overlay
	// TODO: translate()
	font->render("Log", 160, offset_y+8, JUSTIFY_CENTER, screen, FONT_WHITE);
	
	
	// display latest log messages
	
	Point size;
	int display_number = 0;
	int total_size = 0;

	// first calculate how many entire messages can fit in the log view
	for (int i=log_count-1; i>=0; i--) {
		size = font->calc_size(log_msg[i], 224);
		total_size += size.y;
		if (total_size < 344) display_number++;
		else break;
	}
	
	// now display these messages
	int cursor_y = offset_y + 40;
	for (int i=log_count-display_number; i<log_count; i++) {
		
		size = font->calc_size(log_msg[i], 224);	
		font->render(log_msg[i], 40, cursor_y, JUSTIFY_LEFT, screen, 224, FONT_WHITE);
		cursor_y += size.y;
	}
	
}

/**
 * New messages appear on the screen for a brief time
 */
void MenuLog::renderHUDMessages() {
	Point size;
	int cursor_y;
	
	cursor_y = VIEW_H - 40;
	
	// go through new messages
	for (int i=log_count-1; i>=0; i--) {
		if (msg_age[i] > 0 && cursor_y > 32) {
		
			size = font->calc_size(log_msg[i], 224);
			cursor_y -= size.y;
	
			font->render(log_msg[i], 32, cursor_y, JUSTIFY_LEFT, screen, 224, FONT_WHITE);
			
		}
		else return; // no more new messages
	}
}

/**
 * Calculate how long a given message should remain on the HUD
 * Formula: minimum time plus x frames per character
 */
int MenuLog::calcDuration(string s) {
	// 5 seconds plus an extra second per 10 letters
	return FRAMES_PER_SEC * 5 + s.length() * (FRAMES_PER_SEC/10);
}

/**
 * Add a new message to the log
 */
void MenuLog::add(string s) {

	if (log_count == MAX_LOG_MESSAGES) {

		// remove oldest message
		for (int i=0; i<MAX_LOG_MESSAGES-2; i++) {
			log_msg[i] = log_msg[i+1];
			msg_age[i] = msg_age[i+1];
		}

		log_count--;
	}
	
	// add new message
	log_msg[log_count] = s;
	msg_age[log_count] = calcDuration(s);
	
	// force HUD messages to vanish in order
	if (log_count > 0) {
		if (msg_age[log_count] < msg_age[log_count-1])
			msg_age[log_count] = msg_age[log_count-1];
	}
	
	log_count++;
}

MenuLog::~MenuLog() {
	SDL_FreeSurface(background);
}
