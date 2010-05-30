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
 * Using the given wrap width, calculate the width and height necessary to display this text
 */
Point FontEngine::calc_size(string text, int width) {
	char newline = 10;

	// if this contains newlines, recurse
	int check_newline = text.find_first_of(newline);
	if (check_newline > -1) {
		Point p1 = calc_size(text.substr(0, check_newline), width);
		Point p2 = calc_size(text.substr(check_newline+1, text.length()), width);
		Point p3;
		
		if (p1.x > p2.x) p3.x = p1.x;
		else p3.x = p2.x;
		
		p3.y = p1.y + p2.y;
		return p3;
	}

	int height = 0;
	int max_width = 0;

	string segment;
	string fulltext;
	string builder = "";
	string builder_prev = "";
	char space = 32;
	
	fulltext = text + " ";
	segment = eatFirstString(fulltext, space);
	
	while(segment != "") {
		builder = builder + segment;
		
		if (calc_length(builder) > width) {
			height = height + line_height;
			if (calc_length(builder_prev) > max_width) max_width = calc_length(builder_prev);
			builder_prev = "";
			builder = segment + " ";
		}
		else {
			builder = builder + " ";
			builder_prev = builder;
		}
		
		segment = eatFirstString(fulltext, space);
	}
	
	height = height + line_height;
	if (calc_length(builder) > max_width) max_width = calc_length(builder);
				
	Point size;
	size.x = max_width - this->width[32] - kerning; // remove the extra blankspace at the end
	size.y = height;
	return size;

}

/**
 * Render the given text at (x,y) on the target image.
 * Justify is left, right, or center
 */
void FontEngine::render(string text, int x, int y, int justify, SDL_Surface *target) {

	unsigned char c;
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
		if (c >= 32 && c <= 127) {
			src.x = ((c-32) % 16) * font_width;
			src.y = ((c-32) / 16) * font_height;
			src.w = width[c];
		
			// draw the font
			SDL_BlitSurface(sprites, &src, target, &dest);
		
			// move dest
			dest.x = dest.x + width[c] + kerning;
		}
	}
}

/**
 * If width is passed as a parameter, word wrap to that width
 * This version handles newlines
 */
void FontEngine::render(string text, int x, int y, int justify, SDL_Surface *target, int width) {
	// start with an empty string
	// add the next token
	// check length
	// when the length is greater than width, output that line and move to the next token
	
	cursor_y = y;
	string segment;
	string fulltext;
	string builder = "";
	string builder_prev = "";
	char space = 32;
	char newline = 10;

	// if this contains newlines, recurse
	int check_newline = text.find_first_of(newline);
	if (check_newline > -1) {
		render(text.substr(0, check_newline), x, cursor_y, justify, target, width);
		render(text.substr(check_newline+1, text.length()), x, cursor_y, justify, target, width);
		return;
	}
	
	fulltext = text + " ";
	segment = eatFirstString(fulltext, space);
	
	while(segment != "") {
		builder = builder + segment;
		
		if (calc_length(builder) > width) {
			render(builder_prev, x, cursor_y, justify, target);
			cursor_y += line_height;
			builder_prev = "";
			builder = segment + " ";
		}
		else {
			builder = builder + " ";
			builder_prev = builder;
		}
		
		segment = eatFirstString(fulltext, space);
	}

	render(builder, x, cursor_y, justify, target);
	cursor_y += line_height;

}

FontEngine::~FontEngine() {
	SDL_FreeSurface(sprites);
}

