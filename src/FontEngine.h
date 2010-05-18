/*
 * class FontEngine
 * Handles rendering a bitmap font
 *
 * @author Clint Bellanger
 * @license GPL
 *
 */

#ifndef FONT_ENGINE_H
#define FONT_ENGINE_H

#include <fstream>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "Utils.h"

using namespace std;

const int JUSTIFY_LEFT = 0;
const int JUSTIFY_RIGHT = 1;
const int JUSTIFY_CENTER = 2;

class FontEngine {
private:
	SDL_Surface *sprites;
	int font_width;
	int font_height;
	int line_height;
	int kerning;
	int width[256]; // width of each ASCII character
	SDL_Rect src;
	SDL_Rect dest;

	int calc_length(string text);
public:
	FontEngine();
	~FontEngine();
	void load(string filename);
	void render(string text, int x, int y, int justify, SDL_Surface *target);	
};

#endif
