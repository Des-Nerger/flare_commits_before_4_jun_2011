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

struct Renderable {
	Point map_pos;
	SDL_Surface *sprite;
	SDL_Rect *src;
	Point offset;
};

static Point screen_to_map(int x, int y, int camx, int camy) {
	Point r;
	int scrx = x - 320;
	int scry = y - 240;
	r.x = scrx/2 + scry + camx;
	r.y = scry - scrx/2 + camy;
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

static unsigned short xtoi(string hex) {
	char c0 = hex.at(0);
	char c1 = hex.at(1);
	unsigned short val;
	
	if (c0 >= 97) val = c0 - 87;
	else if (c0 >= 65) val = c0 - 55;
	else val = c0 - 48;
	val = val * 16;
	
	if (c1 >= 97) val = val + c1 - 87;
	else if (c1 >= 65) val = val + c1 - 55;
	else val = val + c1 - 48;
	
	return val;
}

static void zsort(Renderable r[], int rnum) {

	int zpos[rnum];
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
