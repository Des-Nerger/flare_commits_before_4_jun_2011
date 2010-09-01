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

const int MAX_LOG_MESSAGES = 100;

class MenuLog {
private:
	SDL_Surface *screen;
	FontEngine *font;

	SDL_Surface *background;
	void loadGraphics();
	
	int calcDuration(string s);
	
	string log_msg[MAX_LOG_MESSAGES];
	int msg_age[MAX_LOG_MESSAGES];
	int log_count;
	
public:
	MenuLog(SDL_Surface *screen, FontEngine *font);
	~MenuLog();

	void logic();
	void render();
	void renderHUDMessages();
	void add(string s);
	
	bool visible;

};

#endif
