/**
 * Utils
 *
 * Various utility structures, enums, function
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "math.h"
#include "Settings.h"

using namespace std;

struct Point {
	int x,y;
};

struct FPoint {
	float x,y;
};

static int round(float f) {
	return (int)(f + 0.5);
}

static Point round(FPoint fp) {
	Point result;
	result.x = round(fp.x);
	result.y = round(fp.y);
	return result;
}

// message passing struct for various sprites rendered map inline
struct Renderable {
	Point map_pos;
	SDL_Surface *sprite;
	SDL_Rect *src;
	Point offset;
	bool object_layer;
};

struct Event_Component {
	string type;
	string s;
	int x;
	int y;
	int z;
};

// TODO: rewrite using UNITS_PER_PIXEL_X,Y?
static Point screen_to_map(int x, int y, int camx, int camy) {
	Point r;
	int scrx = x - VIEW_W_HALF;
	int scry = y - VIEW_H_HALF;
	r.x = scrx + scry*2 + camx;
	r.y = scry*2 - scrx + camy;
	return r;
}

static Point map_to_screen(int x, int y, int camx, int camy) {
	Point r;
	r.x = VIEW_W_HALF + (x/UNITS_PER_PIXEL_X - camx/UNITS_PER_PIXEL_X) - (y/UNITS_PER_PIXEL_X - camy/UNITS_PER_PIXEL_X);
	r.y = VIEW_H_HALF + (x/UNITS_PER_PIXEL_Y - camx/UNITS_PER_PIXEL_Y) + (y/UNITS_PER_PIXEL_Y - camy/UNITS_PER_PIXEL_Y);
	return r;
}

/**
 * Apply parameter distance to position and direction
 */
static FPoint calcVector(Point pos, int direction, int dist) {
	FPoint p;
	p.x = (float)(pos.x);
	p.y = (float)(pos.y);

	float dist_straight = (float)dist;
	float dist_diag = ((float)dist) * 0.7071; //  1/sqrt(2)
	
	switch (direction) {
		case 0:
			p.x -= dist_diag;
			p.y += dist_diag;
			break;
		case 1:
			p.x -= dist_straight;
			break;
		case 2:
			p.x -= dist_diag;
			p.y -= dist_diag;
			break;
		case 3:
			p.y -= dist_straight;
			break;
		case 4:
			p.x += dist_diag;
			p.y -= dist_diag;
			break;
		case 5:
			p.x += dist_straight;
			break;
		case 6:
			p.x += dist_diag;
			p.y += dist_diag;
			break;
		case 7:
			p.y += dist_straight;
			break;
	}
	return p;
}

static double calcDist(Point p1, Point p2) {
	int x = p2.x - p1.x;
	int y = p2.y - p1.y;
	double step1 = x*x + y*y;
	return sqrt(step1);
}

/**
 * is target within the area defined by center and radius?
 */
static bool isWithin(Point center, int radius, Point target) {
	return (calcDist(center, target) < radius);
}


static void zsort(Renderable r[], int rnum) {

	int zpos[1024];
	int ztemp;
	Renderable rtemp;
	
	// calculate zpos
	for (int i=0; i<rnum; i++) {
		zpos[i] = r[i].map_pos.x/2 + r[i].map_pos.y/2;
	}
	
	// sort
	// TODO: better sort algo
	for (int i=0; i<rnum; i++) {
		for (int j=0; j<rnum-1; j++) {
			if (zpos[j] > zpos[j+1]) {
				ztemp = zpos[j];
				zpos[j] = zpos[j+1];
				zpos[j+1] = ztemp;
				rtemp = r[j];
				r[j] = r[j+1];
				r[j+1] = rtemp;			
			}
		}	
	}
	
}

#endif
