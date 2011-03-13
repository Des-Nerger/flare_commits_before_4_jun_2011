/**
 * class NPCManager
 *
 * NPCs which are not combatative enemies are handled by this Manager.
 * Most commonly this involves vendor and conversation townspeople.
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef NPC_MANAGER_H
#define NPC_MANAGER_H

#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "NPC.h"
#include "MapIso.h"

using namespace std;

// max number of NPCs for a single map
const int MAX_NPC_COUNT = 32;

class NPCManager {
private:
	MapIso *map;
public:
	NPCManager(MapIso *_map);
	~NPCManager();
	NPC *npcs[MAX_NPC_COUNT];
	void handleNewMap();
	void logic();
	int checkNPCClick(Point mouse, Point cam);

	int npc_count;
};

#endif
