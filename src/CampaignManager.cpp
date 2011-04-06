/**
 * class CampaignManager
 *
 * Contains data for story mode
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "CampaignManager.h"

CampaignManager::CampaignManager() {

	log_msg = "";
	clearAll();
	clearRewards();
}

void CampaignManager::clearAll() {
	// clear campaign data
	for (int i=0; i<MAX_STATUS; i++) {
		status[i] = "";
	}
	status_count = 0;
}

void CampaignManager::clearRewards() {
	xp_amount = currency_amount = item_amount = item_id = 0;
}

/**
 * Take the savefile camaign= and convert to status array
 */
void CampaignManager::setAll(std::string s) {
	string str = s + ',';
	string token;
	while (str != "" && status_count < MAX_STATUS) {
		token = eatFirstString(str, ',');
		if (token != "") status[status_count++] = token;
	}
}

/**
 * Convert status array to savefile campaign= (status csv)
 */
std::string CampaignManager::getAll() {
	stringstream ss;
	ss.str("");
	for (int i=0; i<status_count; i++) {
		ss << status[i];
		if (i < status_count-1) ss << ',';
	}
	return ss.str();
}

bool CampaignManager::checkStatus(std::string s) {
	for (int i=0; i<status_count; i++) {
		if (status[i] == s) return true;
	}
	return false;
}

void CampaignManager::addStatus(std::string s) {

	// hit upper limit for status
	// TODO: add a warning
	if (status_count >= MAX_STATUS) return;
	
	status[status_count++] = s;
}

/**
 * NPCs have a list of dialog nodes
 * The player wants to begin dialog with this NPC
 * Determine the correct dialog node by the place in the story line
 */
int CampaignManager::chooseDialogNode(NPC *n) {

	// NPC dialog nodes are listed in timeline order
	// So check from the bottom of the list up
	// First node we reach that meets requirements is the correct node
	
	for (int i=n->dialog_count-1; i>=0; i--) {
		for (int j=0; j<NPC_MAX_EVENTS; j++) {
			
			// check requirements
			// break (skip to next dialog node) if any requirement fails
			// if we reach an event that is not a requirement, succeed
			
			if (n->dialog[i][j].type == "requires_status") {
				if (!checkStatus(n->dialog[i][j].s)) break;
			}
			else if (n->dialog[i][j].type == "requires_not") {
				if (checkStatus(n->dialog[i][j].s)) break;
			}
			else {
				return i;
			}
		}
	}
	return 0;
}

/**
 * Process the current dialog
 *
 * Return false if the dialog has ended
 */
bool CampaignManager::processDialog(NPC *n, int dialog_node, int &event_cursor) {

	stringstream ss;
	ss.str("");
	
	while (event_cursor < NPC_MAX_EVENTS) {
	
		// we've already determined requirements are met, so skip these
		if (n->dialog[dialog_node][event_cursor].type == "requires_status") {
			// continue to next event component
		}
		else if (n->dialog[dialog_node][event_cursor].type == "requires_not") {
			// continue to next event component
		}
		else if (n->dialog[dialog_node][event_cursor].type == "set_status") {
			addStatus(n->dialog[dialog_node][event_cursor].s);
		}
		else if (n->dialog[dialog_node][event_cursor].type == "him") {
			return true;
		}
		else if (n->dialog[dialog_node][event_cursor].type == "her") {
			return true;
		}
		else if (n->dialog[dialog_node][event_cursor].type == "you") {
			return true;
		}
		else if (n->dialog[dialog_node][event_cursor].type == "reward_xp") {
			xp_amount = n->dialog[dialog_node][event_cursor].x;
			if (log_msg != "") log_msg += "\n";
			ss << "You receive " << xp_amount << " XP.";
			log_msg += ss.str();
			
			// continue to next event component
		}
		else if (n->dialog[dialog_node][event_cursor].type == "reward_currency") {
			currency_amount = n->dialog[dialog_node][event_cursor].x;
			if (log_msg != "") log_msg += "\n";
			ss << "You receive " << currency_amount << " gold.";
			log_msg += ss.str();
			
			// continue to next event component
		}
		else if (n->dialog[dialog_node][event_cursor].type == "reward_item") {
			item_id = n->dialog[dialog_node][event_cursor].x;
			item_amount = n->dialog[dialog_node][event_cursor].y;
			
			// allow GameEngine::checkNPCInteraction() to set log message for items
			// reason 1: If the player's inventory is full, don't log the receive message
			// reason 2: GameEngine can lookup the item name
		
			// continue to next event component
		}
		else if (n->dialog[dialog_node][event_cursor].type == "") {
			// conversation ends
			return false;
		}
		
		event_cursor++;
	}
	return false;
}

CampaignManager::~CampaignManager() {

}
