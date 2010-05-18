/**
 * class TileSet
 *
 * TileSet storage and file loading
 *
 * @author Clint Bellanger
 * @license GPL
 */
 
#include "TileSet.h"

TileSet::TileSet() {
	sprites = NULL;
	for (int i=0; i<256; i++) {
		tiles[i].src.x = 0;
		tiles[i].src.y = 0;
		tiles[i].src.w = 0;
		tiles[i].src.h = 0;
		tiles[i].offset.x = 0;
		tiles[i].offset.y = 0;
	}
}

/**
 * Given a string that starts with a number then a comma
 * Return that int, and modify the string to remove the num and comma
 */
int TileSet::eatFirstInt(string &s, char separator) {
	int seppos = s.find_first_of(separator);
	int num = atoi(s.substr(0, seppos).c_str());
	s = s.substr(seppos+1, s.length());
	return num;
}

unsigned short TileSet::eatFirstHex(string &s, char separator) {
	int seppos = s.find_first_of(separator);
	unsigned short num = xtoi(s.substr(0, seppos));
	s = s.substr(seppos+1, s.length());
	return num;
}

void TileSet::loadGraphics(string filename) {
	if (sprites) SDL_FreeSurface(sprites);
	
	sprites = IMG_Load(("images/tilesets/" + filename).c_str());
	if(!sprites) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
	}
	SDL_SetColorKey( sprites, SDL_SRCCOLORKEY, SDL_MapRGB(sprites->format, 255, 0, 255) ); 
}

void TileSet::load(string filename) {
	if (current_map == filename) return;
	
	ifstream infile;
	string line;
	string starts_with;
	unsigned short index;

	infile.open(("tilesetdefs/" + filename).c_str(), ios::in);

	if (infile.is_open()) {
		string img;
		
		// first line is the tileset image filename
		getline(infile, line);
		img = line;

		while (!infile.eof()) {
			getline(infile, line);

			if (line.length() > 0) {
				line = line + ',';

				// split across comma
				// line contains:
				// index, x, y, w, h, ox, oy

				index = eatFirstHex(line, ',');
				tiles[index].src.x = eatFirstInt(line, ',');
				tiles[index].src.y = eatFirstInt(line, ',');
				tiles[index].src.w = eatFirstInt(line, ',');
				tiles[index].src.h = eatFirstInt(line, ',');
				tiles[index].offset.x = eatFirstInt(line, ',');
				tiles[index].offset.y = eatFirstInt(line, ',');
			}
		}

		infile.close();
		loadGraphics(img);
	}

	current_map = filename;
}

TileSet::~TileSet() {
	SDL_FreeSurface(sprites);
}
