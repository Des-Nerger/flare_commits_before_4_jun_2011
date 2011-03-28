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
	txt = "";
	pos.x = pos.y = 0;
	
	// init animation info
	sprites = NULL;
	render_size.x = render_size.y = 0;
	render_offset.x = render_offset.y = 0;
	anim_frames = 0;
	anim_duration = 0;
	current_frame = 0;

	// init talker info
	portrait = NULL;
	talker = false;
	
	// init vendor info
	vendor = false;
	for (int i=0; i<NPC_VENDOR_MAX_STOCK; i++) {
		stock[i] = -1;
	}
	stock_count = 0;
	random_stock = 0;
	vox_intro_count = 0;
	
	for (int i=0; i<NPC_MAX_VOX; i++) {
		vox_intro[i] = NULL;
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

	string filename_sprites = "";
	string filename_portrait = "";

	if (infile.is_open()) {
		while (!infile.eof()) {
			line = getLine(infile);

			if (line.length() > 0) {
				starts_with = line.at(0);
				
				if (starts_with == "#") {
					// skip comments
				}
				else if (starts_with == "[") {
					// skip headers
				}
				else { // this is data.  treatment depends on key
					parse_key_pair(line, key, val);          
					key = trim(key, ' ');
					val = trim(val, ' ');
				
					if (key == "name") {
						name = val;
					}
					else if (key == "level") {
						level = atoi(val.c_str());
					}
					else if (key == "gfx") {
						filename_sprites = val;
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

					// handle talkers
					else if (key == "talker") {
						if (val == "true") talker=true;
					}
					else if (key == "portrait") {
						filename_portrait = val;
					}
					else if (key == "txt") {
						txt=val;
					}
					
					// handle vendors
					else if (key == "vendor") {
						if (val == "true") vendor=true;
					}
					else if (key == "constant_stock") {
						val = val + ",";
						while (val != "" && stock_count < NPC_VENDOR_MAX_STOCK) {
							stock[stock_count++] = eatFirstInt(val, ',');
						}

					}
					else if (key == "random_stock") {
						random_stock = atoi(val.c_str());
					}
					
					// handle vocals
					else if (key == "vox_intro") {
						loadSound(val, NPC_VOX_INTRO);
					}
				}
			}
		}
	}
	infile.close();
	loadGraphics(filename_sprites, filename_portrait);
}

void NPC::loadGraphics(string filename_sprites, string filename_portrait) {

	if (filename_sprites != "") {
		sprites = IMG_Load(("images/npcs/" + filename_sprites + ".png").c_str());
		if(!sprites) {
			fprintf(stderr, "Couldn't load NPC sprites: %s\n", IMG_GetError());
		}
	
		SDL_SetColorKey( sprites, SDL_SRCCOLORKEY, SDL_MapRGB(sprites->format, 255, 0, 255) );
	
		// optimize
		SDL_Surface *cleanup = sprites;
		sprites = SDL_DisplayFormatAlpha(sprites);
		SDL_FreeSurface(cleanup);
	}
	if (filename_portrait != "") {
		portrait = IMG_Load(("images/portraits/" + filename_portrait + ".png").c_str());
		if(!portrait) {
			fprintf(stderr, "Couldn't load NPC portrait: %s\n", IMG_GetError());
		}
	
		SDL_SetColorKey( portrait, SDL_SRCCOLORKEY, SDL_MapRGB(portrait->format, 255, 0, 255) );
	
		// optimize
		SDL_Surface *cleanup = portrait;
		portrait = SDL_DisplayFormatAlpha(portrait);
		SDL_FreeSurface(cleanup);
	}
	
}

/**
 * filename assumes the file is in soundfx/npcs/
 * type is a const int enum, see NPC.h
 */
void NPC::loadSound(string filename, int type) {

	if (type == NPC_VOX_INTRO) {
	
		// if too many already loaded, skip this one
		if (vox_intro_count == NPC_MAX_VOX) return;
		vox_intro[vox_intro_count] = Mix_LoadWAV(("soundfx/npcs/" + filename).c_str());
		
		if (vox_intro[vox_intro_count])
			vox_intro_count++;
	}

}

void NPC::logic() {

	// animate
	current_frame++;
	if (current_frame == anim_frames * anim_duration) {
		current_frame = 0;
	}

}

/**
 * type is a const int enum, see NPC.h
 */
bool NPC::playSound(int type) {
	int roll;
	if (type == NPC_VOX_INTRO) {
		if (vox_intro_count == 0) return false;
		roll = rand() % vox_intro_count;
		Mix_PlayChannel(-1, vox_intro[roll], 0);
		return true;
	}
	return false;
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
	if (portrait != NULL) SDL_FreeSurface(portrait);
	for (int i=0; i<NPC_MAX_VOX; i++) {
		Mix_FreeChunk(vox_intro[i]);
	}
}
