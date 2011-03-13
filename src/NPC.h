/**
 * class NPC
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef NPC_H
#define NPC_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <string>
#include <fstream>
#include "Utils.h"
#include "UtilsParsing.h"

using namespace std;

const int NPC_VENDOR_MAX_STOCK = 80;

class NPC {
private:

	// animation info
	SDL_Surface *sprites;
	Point render_size;
	Point render_offset;
	int anim_frames;
	int anim_duration;
	int current_frame;
	
public:
	NPC();
	~NPC();
	void load(string npc_id);
	void loadGraphics(string filename);
	void logic();
	Renderable getRender();
	
	// general info
	string name;
	Point pos; // map position

	// vendor info
	bool vendor;
	int stock[NPC_VENDOR_MAX_STOCK];
};

#endif
