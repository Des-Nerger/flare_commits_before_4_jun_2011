/**
 * class LootManager
 *
 * Handles floor loot
 *
 * @author Clint Bellanger
 * @license GPL
 */
 
#include "LootManager.h"
 
LootManager::LootManager(ItemDatabase *_items) {
	items = _items;
	loot_count = 0;
}
 
Renderable LootManager::getRender(int index) {

}

LootManager::~LootManager() {

}
