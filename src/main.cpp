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


#include "InputState.h"
#include "GameEngine.h"

SDL_Surface *screen;
InputState *inp;
GameEngine *eng;

static void init(bool fullscreen) {
	/* SDL Inits */
	if ( SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ) {		
        fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	Uint32 flags = 0;
    
	flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
    if (fullscreen) flags = flags | SDL_FULLSCREEN;
	
	
	// Create window
	screen = SDL_SetVideoMode (640, 480, 0, flags);
	if (screen == NULL) {
		
        fprintf (stderr, "Couldn't set 640x480 OpenGL video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	
	if (Mix_OpenAudio(22050, AUDIO_S16, 2, 1024)) {
		fprintf (stderr, "Unable to OpenAudio: %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
	
	/* Shared game units setup */
	inp = new InputState();
	eng = new GameEngine(screen, inp);
}

static void mainLoop () {

	bool done = false;
	int fps = 30;
	int delay = 1000/fps;
	int prevTicks = SDL_GetTicks();
	int nowTicks;
	
	while ( !done ) {
		
		// black out
		SDL_FillRect(screen, NULL, 0);

		SDL_PumpEvents();
		inp->handle();
		eng->logic();
		eng->render();
		
		// Engine done means the user escapes the main game menu.
		// Input done means the user closes the window.
		done = eng->done || inp->done;
		
		nowTicks = SDL_GetTicks();
		if (nowTicks - prevTicks < delay) SDL_Delay(delay - (nowTicks - prevTicks));
		prevTicks = nowTicks;
		
		SDL_Flip(screen);
		
		done = eng->done;
	}
}

int main(int argc, char *argv[])
{
	bool fullscreen = false;

	for (int i=0; i<argc; i++) {
		if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fullscreen") == 0) {
			fullscreen = true;
		}
	}

	init(fullscreen);
	mainLoop ();
	
	// cleanup
	// TODO: halt all sounds here before freeing music/chunks
	delete(eng);
	delete(inp);
	SDL_FreeSurface(screen);
	Mix_CloseAudio();
	SDL_Quit();
	
	return 0;
}
