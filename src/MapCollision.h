/*
 * MapCollision.h
 * RPGEngine
 *
 * Handle collisions between objects and the map
 *
 * @author Clint Bellanger
 * @license GPL
 *
 */

#ifndef MAP_COLLISION_H
#define MAP_COLLISION_H

#include <algorithm>
#include <stdlib.h>
#include "Utils.h"
#include "Settings.h"

// collision tile types
const int BLOCKS_ALL = 1;
const int BLOCKS_MOVEMENT = 2;

// collision check types
const int CHECK_MOVEMENT = 1;
const int CHECK_SIGHT = 2;

class MapCollision {
private:
	unsigned short colmap[256][256];
	bool line_check(int x1, int y1, int x2, int y2, int checktype);
	
public:
	MapCollision();
	~MapCollision();
	void setmap(unsigned short _colmap[256][256]);
	bool move(int &x, int &y, int step_x, int step_y, int dist);
	bool is_empty(int x, int y);
	bool line_of_sight(int x1, int y1, int x2, int y2);
	bool line_of_movement(int x1, int y1, int x2, int y2);
	
	int result_x;
	int result_y;
};

#endif
