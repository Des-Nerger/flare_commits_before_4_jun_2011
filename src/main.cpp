/**
 * Generic main.cpp for game engines
 *
 */
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include "Settings.h"

#include "InputState.h"
#include "GameEngine.h"

SDL_Surface *screen;
InputState *inps;
GameEngine *eng;

bool FULLSCREEN;

static void init() {
	/* SDL Inits */
	if ( SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ) {		
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	Uint32 flags = 0;
    
	flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
    if (FULLSCREEN) flags = flags | SDL_FULLSCREEN;
	
	
	// Create window
	screen = SDL_SetVideoMode (VIEW_W, VIEW_H, 0, flags);
	if (screen == NULL) {
		
        fprintf (stderr, "Couldn't set OpenGL video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	
	if (Mix_OpenAudio(22050, AUDIO_S16, 2, 1024)) {
		fprintf (stderr, "Unable to OpenAudio: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
	
	SDL_WM_SetCaption("Flare", "Flare");
	
	/* Shared game units setup */
	inps = new InputState();
	eng = new GameEngine(screen, inps);
}

static void mainLoop () {

	bool done = false;
	int fps = FRAMES_PER_SEC;
	int delay = 1000/fps;
	int prevTicks = SDL_GetTicks();
	int nowTicks;
	
	while ( !done ) {
		
		// black out
		SDL_FillRect(screen, NULL, 0);

		SDL_PumpEvents();
		inps->handle();
		eng->logic();
		eng->render();
		
		// Engine done means the user escapes the main game menu.
		// Input done means the user closes the window.
		done = eng->done || inps->done;
		
		nowTicks = SDL_GetTicks();
		if (nowTicks - prevTicks < delay) SDL_Delay(delay - (nowTicks - prevTicks));
		
		prevTicks = nowTicks;
				
		SDL_Flip(screen);
		
		done = eng->done;
	}
}

int main(int argc, char *argv[])
{

	for (int i=0; i<argc; i++) {
		if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fullscreen") == 0) {
			FULLSCREEN = true;
		}
	}

	init();
	mainLoop ();
	
	// cleanup
	// TODO: halt all sounds here before freeing music/chunks
	delete(eng);
	delete(inps);
	SDL_FreeSurface(screen);
	Mix_CloseAudio();
	SDL_Quit();
	
	return 0;
}
