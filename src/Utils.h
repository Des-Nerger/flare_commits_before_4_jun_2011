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

// Utility Functions
int round(float f);
Point round(FPoint fp);
Point screen_to_map(int x, int y, int camx, int camy);
Point map_to_screen(int x, int y, int camx, int camy);
FPoint calcVector(Point pos, int direction, int dist);
double calcDist(Point p1, Point p2);
bool isWithin(Point center, int radius, Point target);
bool isWithin(SDL_Rect r, Point target);
void zsort(Renderable r[], int rnum);
void drawPixel(SDL_Surface *screen, int x, int y, Uint32 color);

#endif
