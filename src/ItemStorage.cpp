/**
 * class ItemStorage
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "ItemStorage.h"

void ItemStorage::init(int _slot_number, ItemDatabase *_items) {
	slot_number = _slot_number;
	items = _items;

	storage = new ItemStack[slot_number];

	for( int i=0; i<slot_number; i++) {
		storage[i].item = 0;
		storage[i].quantity = 0;
	}
}

ItemStack & ItemStorage::operator [] (int slot) {
	return storage[slot];
}

void ItemStorage::clear() {
	for( int i=0; i<slot_number; i++) {
		storage[i].item = 0;
		storage[i].quantity = 0;
	}
}

/**
 * Insert item into first available carried slot, preferably in the optionnal specified slot
 *
 * @param ItemStack Stack of items
 * @param slot Slot number where it will try to store the item
 */
void ItemStorage::add( ItemStack stack, int slot) {
	int max_quantity;
	int quantity_added;
	int i;

	if( stack.item != 0) {
		max_quantity = items->items[stack.item].max_quantity;
		if( slot > -1) {
			// a slot is specified
			if( storage[slot].item != 0 && storage[slot].item != stack.item) {
				// the proposed slot isn't available
				slot = -1;
			}
		} else {
			// first search of stack to complete if the item is stackable
			i = 0;
			while( max_quantity > 1 && slot == -1 && i < slot_number) {
				if (storage[i].item == stack.item && storage[i].quantity < max_quantity) {
					slot = i;
				}
				i++;
			}
			// then an empty slot
			i = 0;
			while( slot == -1 && i < slot_number) {
				if (storage[i].item == 0) {
					slot = i;
				}
				i++;
			}
		}
		if( slot != -1) {
			// Add
			quantity_added = min( stack.quantity, max_quantity - storage[slot].quantity);
			storage[slot].item = stack.item;
			storage[slot].quantity += quantity_added;
			stack.quantity -= quantity_added;
			// Add back the remaining
			if( stack.quantity > 0) {
				add( stack);
			}
		}
		else {
			// No available slot, drop
			// TODO: We should drop on the floor an item we can't store
		}
	}
}

/**
 * Substract an item from the specified slot, or remove it if it's the last
 *
 * @param slot Slot number
 */
void ItemStorage::substract(int slot, int quantity) {
	storage[slot].quantity -= quantity;
	if (storage[slot].quantity <= 0) {
		storage[slot].item = 0;
	}
}

/**
 * Remove one given item
 */
bool ItemStorage::remove(int item) {
	for (int i=0; i<slot_number; i++) {
		if (storage[i].item == item) {
			substract(i, 1);
			return true;
		}
	}
	return false;
}

void ItemStorage::sort() {
	bubbleSort(storage, slot_number);
}

//TODO: handle stackable items
bool ItemStorage::full() {
	for (int i=0; i<slot_number; i++) {
		if (storage[i].item == 0) {
			return false;
		}
	}
	return true;
}
 
/**
 * Get the number of the specified item carried (not equipped)
 */
int ItemStorage::count(int item) {
	int count=0;
	for (int i=0; i<slot_number; i++) {
		if (storage[i].item == item) {
			count += storage[i].quantity;
		}
	}
	return count;
}

/**
 * Check to see if the given item is equipped
 */
bool ItemStorage::contain(int item) {
	for (int i=0; i<slot_number; i++) {
		if (storage[i].item == item)
			return true;
	}
	return false;
}

ItemStorage::~ItemStorage() {
	delete[] storage;
}

