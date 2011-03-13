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

NPCManager::NPCManager(MapIso *_map, MenuTooltip *_tip, LootManager *_loot) {

	map = _map;
	tip = _tip;
	loot = _loot;

	npc_count = 0;
	for (int i=0; i<MAX_NPC_COUNT; i++) {
		npcs[i] = NULL;
	}
	
	tooltip_margin = 64;
}

void NPCManager::handleNewMap() {
	
	Map_NPC mn;
	int item_roll;
	
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
		
		// if this NPC needs randomized items
		while (npcs[npc_count]->random_stock > 0 && npcs[npc_count]->stock_count < NPC_VENDOR_MAX_STOCK) {
			item_roll = loot->randomItem(npcs[npc_count]->level);
			if (item_roll != 0) {
				npcs[npc_count]->stock[npcs[npc_count]->stock_count++] = item_roll;
			}
			npcs[npc_count]->random_stock--;
		}
		bubbleSort(npcs[npc_count]->stock, npcs[npc_count]->stock_count);
		removeDupes(npcs[npc_count]->stock, npcs[npc_count]->stock_count);
		for (int i=npcs[npc_count]->stock_count; i<NPC_VENDOR_MAX_STOCK; i++) {
			npcs[npc_count]->stock[i] = -1;
		}
		
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

/**
 * On mouseover, display NPC's name
 */
void NPCManager::renderTooltips(Point cam, Point mouse) {
	Point p;
	SDL_Rect r;
	TooltipData td;
	
	for(int i=0; i<npc_count; i++) {

		p = map_to_screen(npcs[i]->pos.x, npcs[i]->pos.y, cam.x, cam.y);
	
		r.w = npcs[i]->render_size.x;
		r.h = npcs[i]->render_size.y;
		r.x = p.x - npcs[i]->render_offset.x;
		r.y = p.y - npcs[i]->render_offset.y;
		
		if (isWithin(r, mouse)) {
		
			// adjust dest.y so that the tooltip floats above the item
			p.y -= tooltip_margin;
			
			td.num_lines = 1;
			td.colors[0] = FONT_WHITE;
			td.lines[0] = npcs[i]->name;
			
			tip->render(td, p, STYLE_TOPLABEL);
		}
	}
}

NPCManager::~NPCManager() {
	for (int i=0; i<npc_count; i++) {
		delete(npcs[i]);
	}

}
