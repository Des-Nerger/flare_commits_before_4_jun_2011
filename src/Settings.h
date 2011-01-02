/**
 * Settings
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef SETTINGS_H
#define SETTINGS_H

const int UNITS_PER_TILE = 64;
const int TILE_SHIFT = 6; // for fast bitshift divides
const int UNITS_PER_PIXEL_X = 2;
const int UNITS_PER_PIXEL_Y = 4;
const int TILE_W = 64;
const int TILE_H = 32;
const int TILE_W_HALF = TILE_W/2;
const int TILE_H_HALF = TILE_H/2;

const int FRAMES_PER_SEC = 30;
const int VIEW_W = 640;
const int VIEW_H = 480;
const int VIEW_W_HALF = VIEW_W/2;
const int VIEW_H_HALF = VIEW_H/2;

const int MUSIC_VOLUME = 32;

#endif
