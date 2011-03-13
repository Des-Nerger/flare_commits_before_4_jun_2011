/**
 * class NPC
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "NPC.h"

NPC::NPC() {

	// init general vars
	name = "";
	pos.x = pos.y = 0;
	
	// init animation info
	sprites = NULL;
	render_size.x = render_size.y = 0;
	render_offset.x = render_offset.y = 0;
	anim_frames = 0;
	anim_duration = 0;
	current_frame = 0;
	
	// init vendor info
	vendor = false;
	for (int i=0; i<NPC_VENDOR_MAX_STOCK; i++) {
		stock[i] = -1;
	}
}

/**
 * NPCs are stored in simple config files
 *
 * @param npc_id Config file loaded at npcs/[npc_id].txt
 */
void NPC::load(string npc_id) {

	ifstream infile;
	string line;
	string key;
	string val;
	string starts_with;
	
	infile.open(("npcs/" + npc_id + ".txt").c_str(), ios::in);

	if (infile.is_open()) {
		while (!infile.eof()) {
			line = getLine(infile);

			if (line.length() > 0) {
				starts_with = line.at(0);
				
				if (starts_with == "#") {
					// skip comments
				}
				else { // this is data.  treatment depends on key
					parse_key_pair(line, key, val);          
					key = trim(key, ' ');
					val = trim(val, ' ');
				
					if (key == "name") {
						name = val;
					}
					else if (key == "gfx") {
						loadGraphics(val);
					}
					else if (key == "render_size") {
						val = val + ",";
						render_size.x = eatFirstInt(val, ',');
						render_size.y = eatFirstInt(val, ',');
					}
					else if (key == "render_offset") {
						val = val + ",";
						render_offset.x = eatFirstInt(val, ',');
						render_offset.y = eatFirstInt(val, ',');
					}
					else if (key == "anim_frames") {
						anim_frames = atoi(val.c_str());
					}
					else if (key == "anim_duration") {
						anim_duration = atoi(val.c_str());
					}
					
				}
			}
		}
	}
	infile.close();
	
}

void NPC::loadGraphics(string filename) {

	sprites = IMG_Load(("images/npcs/" + filename + ".png").c_str());
	if(!sprites) {
		fprintf(stderr, "Couldn't load NPC sprites: %s\n", IMG_GetError());
	}
		
	// optimize
	SDL_Surface *cleanup = sprites;
	sprites = SDL_DisplayFormatAlpha(sprites);
	SDL_FreeSurface(cleanup);
	
}

void NPC::logic() {

	// animate
	current_frame++;
	if (current_frame == anim_frames * anim_duration) {
		current_frame = 0;
	}

}

Renderable NPC::getRender() {
	Renderable r;
	r.sprite = sprites;
	r.map_pos.x = pos.x;
	r.map_pos.y = pos.y;
	r.src.x = render_size.x * (current_frame / anim_duration);
	r.src.y = 0;
	r.src.w = render_size.x;
	r.src.h = render_size.y;
	r.offset.x = render_offset.x;
	r.offset.y = render_offset.y;
	r.object_layer = true;
		
	return r;	
}

NPC::~NPC() {
	if (sprites != NULL) SDL_FreeSurface(sprites);
}
