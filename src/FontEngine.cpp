/*
 * class FontEngine
 * Handles rendering a bitmap font
 *
 * @author Clint Bellanger
 * @license GPL
 *
 */

#include "FontEngine.h"


FontEngine::FontEngine() {

	for (int i=0; i<256; i++) {
		width[i] = 0;
	}
	load("fonts/font.txt");
}

FontEngine::~FontEngine() {
	SDL_FreeSurface(sprites);
}

void FontEngine::load(string filename) {

	string imgfile;
	string line;
	ifstream infile;
	char str[8];
	
	// load the definition file
	infile.open(filename.c_str(), ios::in);

	if (infile.is_open()) {
	
		getline(infile, line);
		imgfile = line;
		
		getline(infile, line);
		font_width = atoi(line.c_str());
		
		getline(infile, line);
		font_height = atoi(line.c_str());
		src.h = font_height;
		dest.h = font_height;
		
		getline(infile, line);
		line_height = atoi(line.c_str());
		
		getline(infile, line);
		kerning = atoi(line.c_str());
		
		// the rest of the file is character pixel widths
		while (!infile.eof()) {
			getline(infile, line);
			if (line.length() > 0) {
				strcpy(str, line.c_str());
				width[str[0]] = line.c_str()[2] - 48;
				
			}
		}
	}
	infile.close();
	
	// load the font image
	sprites = IMG_Load(("fonts/" + imgfile).c_str());
	if(!sprites) {
		// TODO: if there's an \r in the font.txt file after the filename, this will fail
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
		exit(1);
	}
}

int FontEngine::calc_length(string text) {
	int size=0;
	char c;
	for (int i=0; i<text.length(); i++) {
		c = text.c_str()[i];
		size = size + width[c] + kerning;
	}
	size = size - kerning;
	return size;
}

/**
 * Render the given text at (x,y) on the target image.
 * Justify is left, right, or center
 */
void FontEngine::render(string text, int x, int y, int justify, SDL_Surface *target) {

	char c;
	char str[256];
	
	strcpy(str, text.c_str());

	// calculate actual starting x,y based on justify
	if (justify == JUSTIFY_LEFT) {
		dest.x = x;
		dest.y = y;
	}
	else if (justify == JUSTIFY_RIGHT) {
		dest.x = x - calc_length(text);
		dest.y = y;
	}
	else if (justify == JUSTIFY_CENTER) {
		dest.x = x - calc_length(text)/2;
		dest.y = y;
	}

	for (int i=0; i<text.length(); i++) {
		// set the bounding rect of the char to render
		c = str[i];
		src.x = ((c-32) % 16) * font_width;
		src.y = ((c-32) / 16) * font_height;
		src.w = width[c];
		
		// draw the font
		SDL_BlitSurface(sprites, &src, target, &dest);
		
		// move dest
		dest.x = dest.x + width[c] + kerning;
	}
}


