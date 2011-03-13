/**
 * class NPCManager
 *
 * NPCs which are not combatative enemies are handled by this Manager.
 * Most commonly this involves vendor and conversation townspeople.
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "NPCManager.h"

NPCManager::NPCManager(MapIso *_map) {

	map = _map;

	npc_count = 0;
	for (int i=0; i<MAX_NPC_COUNT; i++) {
		npcs[i] = NULL;
	}
}

void NPCManager::handleNewMap() {
	
	Map_NPC mn;
		
	// remove existing NPCs
	for (int i=0; i<npc_count; i++) {
		delete(npcs[i]);
		npcs[i] = NULL;
	}
	
	npc_count = 0;
	
	// read the queued NPCs in the map file
	while (!map->npcs.empty()) {
		mn = map->npcs.front();
		map->npcs.pop();
		
		npcs[npc_count] = new NPC();
		npcs[npc_count]->load(mn.id);
		npcs[npc_count]->pos.x = mn.pos.x;
		npcs[npc_count]->pos.y = mn.pos.y;
		
		npc_count++;
	}

}

void NPCManager::logic() {
	for (int i=0; i<npc_count; i++) {
		npcs[i]->logic();
	}
}

int NPCManager::checkNPCClick(Point mouse, Point cam) {
	Point p;
	SDL_Rect r;
	for(int i=0; i<npc_count; i++) {

		p = map_to_screen(npcs[i]->pos.x, npcs[i]->pos.y, cam.x, cam.y);
	
		r.w = npcs[i]->render_size.x;
		r.h = npcs[i]->render_size.y;
		r.x = p.x - npcs[i]->render_offset.x;
		r.y = p.y - npcs[i]->render_offset.y;
		
		if (isWithin(r, mouse)) {
			return i;
		}
	}
	return -1;
}

NPCManager::~NPCManager() {
	for (int i=0; i<npc_count; i++) {
		delete(npcs[i]);
	}

}
