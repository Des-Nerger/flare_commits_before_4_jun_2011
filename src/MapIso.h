/**
 * class MapIso
 *
 * Isometric map data structure and rendering
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef MAP_ISO_H
#define MAP_ISO_H

#include <fstream>
#include <string>
#include <queue>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Utils.h"
#include "TileSet.h"
#include "MapCollision.h"
#include "Settings.h"

using namespace std;

struct Map_Enemy {
	string type;
	Point pos;
	int direction;
};

struct Event_Component {
	string type;
	string s;
	int x;
	int y;
	int z;
};

struct Map_Event {
	string type;
	SDL_Rect location;
	Event_Component components[8];
	int comp_num;
};



class MapIso {
private:
	SDL_Surface *screen;
	Mix_Music *music;
	Mix_Chunk *sfx;
	
	string parse_section_title(string tok);
	void parse_key_pair(string tok, string &key, string &val);
	int eatFirstInt(string &s, char separator);
	unsigned short eatFirstHex(string &s, char separator);
	string eatFirstString(string &s, char separator);
	void executeEvent(int eid);
	void removeEvent(int eid);
	void clearEvents();
	void playSFX(string filename);
		
	// map events
	Map_Event events[256];
	int event_count;
	
public:
	// functions
	MapIso(SDL_Surface *_screen);
	~MapIso();
	void clear_enemy(Map_Enemy e);
	int load(string filename);
	void loadMusic();
	void render(Renderable r[], int rnum);
	void checkEvents(Point loc);

	// vars
	string title;
	int w;
	int h;
	Point cam;
	Point hero_tile;
	Point spawn;
	int spawn_dir;
	string tileset;
	string music_filename;
	bool new_music;
	TileSet tset;
	
	unsigned short background[256][256];
	unsigned short object[256][256];
	unsigned short collision[256][256];
	MapCollision collider;

	// enemy load handling
	queue<Map_Enemy> enemies;
	Map_Enemy new_enemy;
	bool enemy_awaiting_queue;

	// teleport handling
	bool teleportation;
	Point teleport_desination;
	string teleport_mapname;
	
};


#endif
