/**
 * class InputState
 *
 * Handles keyboard and mouse states
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "InputState.h"


InputState::InputState(void) {
	binding[CANCEL] = SDLK_ESCAPE;
	binding[ACCEPT] = SDLK_RETURN;
	binding[UP] = SDLK_w;
	binding[DOWN] = SDLK_s;
	binding[LEFT] = SDLK_a;
	binding[RIGHT] = SDLK_d;
	
	binding2[CANCEL] = SDLK_BACKSPACE;
	binding2[ACCEPT] = SDLK_SPACE;
	binding2[UP] = SDLK_UP;
	binding2[DOWN] = SDLK_DOWN;
	binding2[LEFT] = SDLK_LEFT;
	binding2[RIGHT] = SDLK_RIGHT;
	
	binding[BAR_1] = binding2[BAR_1] = SDLK_1;
	binding[BAR_2] = binding2[BAR_2] = SDLK_2;
	binding[BAR_3] = binding2[BAR_3] = SDLK_3;
	binding[BAR_4] = binding2[BAR_4] = SDLK_4;
	binding[BAR_5] = binding2[BAR_5] = SDLK_5;
	binding[BAR_6] = binding2[BAR_6] = SDLK_6;
	binding[BAR_7] = binding2[BAR_7] = SDLK_7;
	binding[BAR_8] = binding2[BAR_8] = SDLK_8;
	binding[BAR_9] = binding2[BAR_9] = SDLK_9;
	binding[BAR_0] = binding2[BAR_0] = SDLK_0;
	
	binding[CHARACTER] = binding2[CHARACTER] = SDLK_c;
	binding[INVENTORY] = binding2[INVENTORY] = SDLK_i;
	binding[POWERS] = binding2[POWERS] = SDLK_p;
	binding[LOG] = binding2[LOG] = SDLK_l;
	
	binding[MAIN1] = binding2[MAIN1] = SDL_BUTTON_LEFT;
	binding[MAIN2] = binding2[MAIN2] = SDL_BUTTON_RIGHT;
	
	binding[CTRL] = SDLK_LCTRL;
	binding2[CTRL] = SDLK_RCTRL;
	binding[SHIFT] = SDLK_LSHIFT;
	binding2[SHIFT] = SDLK_RSHIFT;
	
	for (int key=0; key<key_count; key++) {
		pressing[key] = false;	
	}
	done = false;
	mouse_lock = false;
	mouse2_lock = false;
}
	
void InputState::handle() {
	SDL_Event event;
	
	SDL_GetMouseState(&mouse.x, &mouse.y);
	
	/* Check for events */
	while (SDL_PollEvent (&event)) {
		switch (event.type) {
				
			case SDL_MOUSEBUTTONDOWN:
				for (int key=0; key<key_count; key++) {
					if (event.button.button == binding[key] || event.button.button == binding2[key]) {
						pressing[key] = true;
					}
				}
				break;
			case SDL_MOUSEBUTTONUP:
				for (int key=0; key<key_count; key++) {
					if (event.button.button == binding[key] || event.button.button == binding2[key]) {
						pressing[key] = false;
					}
				}
				break;
			case SDL_KEYDOWN:
				for (int key=0; key<key_count; key++) {
					if (event.key.keysym.sym == binding[key] || event.key.keysym.sym == binding2[key]) {
						pressing[key] = true;
					}
				}
				break;
			case SDL_KEYUP:
				for (int key=0; key<key_count; key++) {
					if (event.key.keysym.sym == binding[key] || event.key.keysym.sym == binding2[key]) {
						pressing[key] = false;
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
