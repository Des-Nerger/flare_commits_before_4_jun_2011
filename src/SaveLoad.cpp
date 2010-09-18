/**
 * Save and Load functions for the GameEngine.
 *
 * I put these in a separate cpp file just to keep GameEngine.cpp devoted to its core.
 *
 * class GameEngine
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "GameEngine.h"
#include "UtilsParsing.h"
#include <fstream>
#include <iostream>

using namespace std;

/**
 * Before exiting the game, save to file
 */
void GameEngine::saveGame() {
	ofstream outfile;

	outfile.open("saves/save1.txt", ios::out);

	if (outfile.is_open()) {
	
		// hero name
		outfile << "name=" << pc->stats.name << "\n";
		
		// current experience
		outfile << "xp=" << pc->stats.xp << "\n";
		
		// stat spec
		outfile << "build=" << pc->stats.physical << "," << pc->stats.magical << "," << pc->stats.offense << "," << pc->stats.defense << "\n";
		
		// current gold
		outfile << "gold=" << menu->inv->gold << "\n";
		
		// equipped gear
		outfile << "equipped=";
		for (int i=0; i<4; i++) {
			outfile << menu->inv->equipped[i];
			if (i<3) outfile << ",";
			else outfile << "\n";
		}
	
		// carried items
		outfile << "carried=";
		for (int i=0; i<64; i++) {
			outfile << menu->inv->carried[i];
			if (i<63) outfile << ",";
			else outfile << "\n";
		}
		
		// spawn point
		outfile << "spawn=" << map->respawn_map << "," << map->respawn_point.x << "," << map->respawn_point.y << "\n";
		
		// action bar
		outfile << "actionbar=";
		for (int i=0; i<12; i++) {
			outfile << menu->act->hotkeys[i];
			if (i<11) outfile << ",";
		}
		outfile << "\n";
		outfile << endl;
		
		outfile.close();
	}
}

/**
 * When loading the game, load from file if possible
 */
void GameEngine::loadGame() {

	ifstream infile;
	string line;
	string key;
	string val;
	string starts_with;
	int hotkeys[12];
	
	for (int i=0; i<12; i++) {
		hotkeys[i] = -1;
	}

	infile.open("saves/save1.txt", ios::in);

	if (infile.is_open()) {
		while (!infile.eof()) {
			getline(infile, line);

			if (line.length() > 0) {
				starts_with = line.at(0);
				
				if (starts_with == "#") {
					// skip comments
				}
				else { // this is data.  treatment depends on section type
					parse_key_pair(line, key, val);          
					key = trim(key, ' ');
					val = trim(val, ' ');
				
					if (key == "name") {
						pc->stats.name = val;
					}
					else if (key == "xp") {
						pc->stats.xp = atoi(val.c_str());
					}
					else if (key == "build") {
						val = val + ",";
						pc->stats.physical = eatFirstInt(val, ',');
						pc->stats.magical = eatFirstInt(val, ',');
						pc->stats.offense = eatFirstInt(val, ',');
						pc->stats.defense = eatFirstInt(val, ',');
					}
					else if (key == "gold") {
						menu->inv->gold = atoi(val.c_str());
					}
					else if (key == "equipped") {
						val = val + ",";
						for (int i=0; i<4; i++) {
							menu->inv->equipped[i] = eatFirstInt(val, ',');
						}
					}
					else if (key == "carried") {
						val = val + ",";
						for (int i=0; i<64; i++) {
							menu->inv->carried[i] = eatFirstInt(val, ',');					
						}
					}
					else if (key == "spawn") {
						val = val + ",";
						map->teleport_mapname = eatFirstString(val, ',');
						map->teleport_destination.x = eatFirstInt(val, ',');
						map->teleport_destination.y = eatFirstInt(val, ',');
						map->teleportation = true;
						
						// prevent spawn.txt from putting us on the starting map
						map->clearEvents();
					}
					else if (key == "actionbar") {
						val = val + ",";
						for (int i=0; i<12; i++)
							hotkeys[i] = eatFirstInt(val, ',');
						menu->act->set(hotkeys);
					}
				}
			}
		}
			
		infile.close();
		
		// initialize vars
		pc->stats.recalc();
		menu->items->applyEquipment(&pc->stats, menu->inv->equipped);
		pc->stats.hp = pc->stats.maxhp;
		pc->stats.mp = pc->stats.maxmp;
				
		// just for aesthetics, turn the hero to face the camera
		pc->stats.direction = 6;
	}
}
