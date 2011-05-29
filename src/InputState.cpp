/**
 * class InputState
 *
 * Handles keyboard and mouse states
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "InputState.h"
#include <fstream>
#include <string>
#include "UtilsParsing.h"

using namespace std;

InputState::InputState(void) {
	binding[CANCEL] = SDLK_ESCAPE;
	binding[ACCEPT] = SDLK_RETURN;
	binding[UP] = SDLK_w;
	binding[DOWN] = SDLK_s;
	binding[LEFT] = SDLK_a;
	binding[RIGHT] = SDLK_d;
	
	binding_alt[CANCEL] = SDLK_BACKSPACE;
	binding_alt[ACCEPT] = SDLK_SPACE;
	binding_alt[UP] = SDLK_UP;
	binding_alt[DOWN] = SDLK_DOWN;
	binding_alt[LEFT] = SDLK_LEFT;
	binding_alt[RIGHT] = SDLK_RIGHT;
	
	binding[BAR_1] = binding_alt[BAR_1] = SDLK_1;
	binding[BAR_2] = binding_alt[BAR_2] = SDLK_2;
	binding[BAR_3] = binding_alt[BAR_3] = SDLK_3;
	binding[BAR_4] = binding_alt[BAR_4] = SDLK_4;
	binding[BAR_5] = binding_alt[BAR_5] = SDLK_5;
	binding[BAR_6] = binding_alt[BAR_6] = SDLK_6;
	binding[BAR_7] = binding_alt[BAR_7] = SDLK_7;
	binding[BAR_8] = binding_alt[BAR_8] = SDLK_8;
	binding[BAR_9] = binding_alt[BAR_9] = SDLK_9;
	binding[BAR_0] = binding_alt[BAR_0] = SDLK_0;
	
	binding[CHARACTER] = binding_alt[CHARACTER] = SDLK_c;
	binding[INVENTORY] = binding_alt[INVENTORY] = SDLK_i;
	binding[POWERS] = binding_alt[POWERS] = SDLK_p;
	binding[LOG] = binding_alt[LOG] = SDLK_l;
	
	binding[MAIN1] = binding_alt[MAIN1] = SDL_BUTTON_LEFT;
	binding[MAIN2] = binding_alt[MAIN2] = SDL_BUTTON_RIGHT;
	
	binding[CTRL] = SDLK_LCTRL;
	binding_alt[CTRL] = SDLK_RCTRL;
	binding[SHIFT] = SDLK_LSHIFT;
	binding_alt[SHIFT] = SDLK_RSHIFT;
	
	for (int key=0; key<key_count; key++) {
		pressing[key] = false;
		lock[key] = false;
	}
	done = false;
	
	loadKeyBindings();
}

/**
 * Key bindings are found in config/keybindings.txt
 */
void InputState::loadKeyBindings() {
	ifstream infile;
	string line;
	string key;
	string val;
	int key1;
	int key2;
	string starts_with;
	int cursor;
	
	infile.open("config/keybindings.txt", ios::in);

	if (infile.is_open()) {
		while (!infile.eof()) {
			line = getLine(infile);

			if (line.length() > 0) {
				starts_with = line.at(0);
				
				if (starts_with == "#") {
					// skip comments
				}
				else if (starts_with == "[") {
					// skip headers
				}
				else { // this is data.  treatment depends on key
					parse_key_pair(line, key, val);          
					key = trim(key, ' ');
					val = trim(val, ' ');
					key1 = eatFirstInt(val, ',');
					key2 = atoi(val.c_str());
				
					cursor = -1;
				
					if (key == "cancel") cursor = CANCEL;
					else if (key == "accept") cursor = ACCEPT;
					else if (key == "up") cursor = UP;
					else if (key == "down") cursor = DOWN;
					else if (key == "left") cursor = LEFT;
					else if (key == "right") cursor = RIGHT;
					else if (key == "bar1") cursor = BAR_1;
					else if (key == "bar2") cursor = BAR_2;
					else if (key == "bar3") cursor = BAR_3;
					else if (key == "bar4") cursor = BAR_4;
					else if (key == "bar5") cursor = BAR_5;
					else if (key == "bar6") cursor = BAR_6;
					else if (key == "bar7") cursor = BAR_7;
					else if (key == "bar8") cursor = BAR_8;
					else if (key == "bar9") cursor = BAR_9;
					else if (key == "bar0") cursor = BAR_0;
					else if (key == "main1") cursor = MAIN1;
					else if (key == "main2") cursor = MAIN2;
					else if (key == "character") cursor = CHARACTER;
					else if (key == "inventory") cursor = INVENTORY;
					else if (key == "powers") cursor = POWERS;
					else if (key == "log") cursor = LOG;
					else if (key == "ctrl") cursor = CTRL;
					else if (key == "shift") cursor = SHIFT;
					
					if (cursor != -1) {
						binding[cursor] = key1;
						binding_alt[cursor] = key2;
					}
	
				}
			}
		}
	}
	infile.close();
}
	
void InputState::handle() {
	SDL_Event event;
	
	SDL_GetMouseState(&mouse.x, &mouse.y);
	
	static bool joyHasMovedX;
	static bool joyHasMovedY;
	static int fakeKeyX;
	static int fakeKeyY;

	/* Check for events */
	while (SDL_PollEvent (&event)) {
		switch (event.type) {
				
			case SDL_MOUSEBUTTONDOWN:
				for (int key=0; key<key_count; key++) {
					if (event.button.button == binding[key] || event.button.button == binding_alt[key]) {
						pressing[key] = true;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				for (int key=0; key<key_count; key++) {
					if (event.button.button == binding[key] || event.button.button == binding_alt[key]) {
						pressing[key] = false;
						lock[key] = false;
					}
				}
				break;
			case SDL_KEYDOWN:
				for (int key=0; key<key_count; key++) {
					if (event.key.keysym.sym == binding[key] || event.key.keysym.sym == binding_alt[key]) {
						pressing[key] = true;
					}
				}
				break;
			case SDL_KEYUP:
				for (int key=0; key<key_count; key++) {
					if (event.key.keysym.sym == binding[key] || event.key.keysym.sym == binding_alt[key]) {
						pressing[key] = false;
						lock[key] = false;
					}
				}
				break;
			case SDL_JOYAXISMOTION:
				switch(event.jaxis.axis) {
					/* first analog */
					case 0:
						/* left */
						if ((joyHasMovedX == false) && (event.jaxis.value < -JOY_DEADZONE) && (event.jaxis.value > JOY_MIN)) {
							fakeKeyX = SDLK_LEFT;
							joyHasMovedX = true;
						}
						/* right */
						if ((joyHasMovedX == false) && (event.jaxis.value > JOY_DEADZONE) && (event.jaxis.value < JOY_MAX)) {
							fakeKeyX = SDLK_RIGHT;
							joyHasMovedX = true;
						}
						/* centered */
						if ((event.jaxis.value > -JOY_DEADZONE) && (event.jaxis.value < JOY_DEADZONE)) {
							joyHasMovedX = false;
						}
						break;
					case 1:
						/* up */
						if ((joyHasMovedY == false) && (event.jaxis.value < -JOY_DEADZONE) && (event.jaxis.value > JOY_MIN)) {
							fakeKeyY = SDLK_UP;
							joyHasMovedY = true;
						}
						/* down */
						if ((joyHasMovedY == false) && (event.jaxis.value > JOY_DEADZONE) && (event.jaxis.value < JOY_MAX)) {
							fakeKeyY = SDLK_DOWN;
							joyHasMovedY = true;
						}
						/* centered */
						if ((event.jaxis.value > -JOY_DEADZONE) && (event.jaxis.value < JOY_DEADZONE)) {
							joyHasMovedY = false;
						}
						break;
					/* second analog */
					case 2:
						break;
					case 4:
						break;
				}
				for (int key=0; key<key_count; key++) {
					if (fakeKeyX == binding[key] || fakeKeyX == binding_alt[key]) {
						if (joyHasMovedX) {
							pressing[key] = true;
						}
						else {
							pressing[key] = false;
							lock[key] = false;
						}
					}
					if (fakeKeyY == binding[key] || fakeKeyY == binding_alt[key]) {
						if (joyHasMovedY) {
							pressing[key] = true;
						}
						else {
							pressing[key] = false;
							lock[key] = false;
						}
					}
				}
				break;

			case SDL_JOYHATMOTION:
				switch (event.jhat.value) {
					case SDL_HAT_CENTERED:
						for (int key = 0; key<key_count; key++) {
							if (fakeKeyX == binding[key] || fakeKeyX == binding_alt[key]) {
								pressing[key] = false;
								lock[key] = false;
							}
							if (fakeKeyY == binding[key] || fakeKeyY == binding_alt[key]) {
								pressing[key] = false;
								lock[key] = false;
							}
						}
						break;
					case SDL_HAT_UP:
						fakeKeyY = SDLK_UP;
						for (int key = 0; key<key_count; key++) {
							if (fakeKeyX == binding[key] || fakeKeyX == binding_alt[key]) {
								pressing[key] = false;
								lock[key] = false;
							}
							if (fakeKeyY == binding[key] || fakeKeyY == binding_alt[key]) {
								pressing[key] = true;
							}
						}
						break;
					case SDL_HAT_DOWN:
						fakeKeyY = SDLK_DOWN;
						for (int key = 0; key<key_count; key++) {
							if (fakeKeyX == binding[key] || fakeKeyX == binding_alt[key]) {
								pressing[key] = false;
								lock[key] = false;
							}
							if (fakeKeyY == binding[key] || fakeKeyY == binding_alt[key]) {
								pressing[key] = true;
							}
						}
						break;
					case SDL_HAT_LEFT:
						fakeKeyX = SDLK_LEFT;
						for (int key = 0; key<key_count; key++) {
							if (fakeKeyX == binding[key] || fakeKeyX == binding_alt[key]) {
								pressing[key] = true;
							}
							if (fakeKeyY == binding[key] || fakeKeyY == binding_alt[key]) {
								pressing[key] = false;
								lock[key] = false;
							}
						}
						break;
					case SDL_HAT_RIGHT:
						fakeKeyX = SDLK_RIGHT;
						for (int key = 0; key<key_count; key++) {
							if (fakeKeyX == binding[key] || fakeKeyX == binding_alt[key]) {
								pressing[key] = true;
							}
							if (fakeKeyY == binding[key] || fakeKeyY == binding_alt[key]) {
								pressing[key] = false;
								lock[key] = false;
							}
						}
						break;
					case SDL_HAT_LEFTUP:
						fakeKeyX = SDLK_LEFT;
						fakeKeyY = SDLK_UP;
						for (int key = 0; key<key_count; key++) {
							if (fakeKeyX == binding[key] || fakeKeyX == binding_alt[key]) {
								pressing[key] = true;
							}
							if (fakeKeyY == binding[key] || fakeKeyY == binding_alt[key]) {
								pressing[key] = true;
							}
						}
						break;
					case SDL_HAT_LEFTDOWN:
						fakeKeyX = SDLK_LEFT;
						fakeKeyY = SDLK_DOWN;
						for (int key = 0; key<key_count; key++) {
							if (fakeKeyX == binding[key] || fakeKeyX == binding_alt[key]) {
								pressing[key] = true;
							}
							if (fakeKeyY == binding[key] || fakeKeyY == binding_alt[key]) {
								pressing[key] = true;
							}
						}
						break;
					case SDL_HAT_RIGHTUP:
						fakeKeyX = SDLK_RIGHT;
						fakeKeyY = SDLK_UP;
						for (int key = 0; key<key_count; key++) {
							if (fakeKeyX == binding[key] || fakeKeyX == binding_alt[key]) {
								pressing[key] = true;
							}
							if (fakeKeyY == binding[key] || fakeKeyY == binding_alt[key]) {
								pressing[key] = true;
							}
						}
						break;
					case SDL_HAT_RIGHTDOWN:
						fakeKeyX = SDLK_RIGHT;
						fakeKeyY = SDLK_DOWN;
						for (int key = 0; key<key_count; key++) {
							if (fakeKeyX == binding[key] || fakeKeyX == binding_alt[key]) {
								pressing[key] = true;
							}
							if (fakeKeyY == binding[key] || fakeKeyY == binding_alt[key]) {
								pressing[key] = true;
							}
						}
						break;
				}
				break;
			case SDL_JOYBUTTONDOWN:
				for (int key=0; key<key_count; key++) {
					if (event.jbutton.button == binding[key] || event.jbutton.button == binding_alt[key]) {
						pressing[key] = true;
					}
				}
				break;
			case SDL_JOYBUTTONUP:
				for (int key=0; key<key_count; key++) {
					if (event.jbutton.button == binding[key] || event.jbutton.button == binding_alt[key]) {
						pressing[key] = false;
						lock[key] = false;
					}
				}
				break;
			case SDL_QUIT:
				done = 1;
				break;
			default:
				break;
		}
	}

		
}

InputState::~InputState() {
}
