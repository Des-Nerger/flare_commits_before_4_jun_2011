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
const int NPC_MAX_VOX = 8;
const int NPC_VOX_INTRO = 0;

class NPC {
private:

	// animation info
	SDL_Surface *sprites;
	int anim_frames;
	int anim_duration;
	int current_frame;
	
public:
	NPC();
	~NPC();
	void load(string npc_id);
	void loadGraphics(string filename);
	void loadSound(string filename, int type);
	void logic();
	bool playSound(int type);
	Renderable getRender();
	
	// general info
	string name;
	Point pos; // map position
	int level; // used in determining item quality
	
	// public animation info
	Point render_size;
	Point render_offset;

	// vendor info
	bool vendor;
	int stock[NPC_VENDOR_MAX_STOCK];
	int stock_count;
	int random_stock;
	
	// vocals
	Mix_Chunk *vox_intro[NPC_MAX_VOX];
	int vox_intro_count;
	
};

#endif
