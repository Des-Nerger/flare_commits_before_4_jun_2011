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

	// clear campaign data
	for (int i=0; i<MAX_STATUS; i++) {
		status[i] = false;
	}
}

/**
 * Campaign data is stored as hex characters
 * Take one hex character and convert to binary
 */
void CampaignManager::setHex(char c, int start) {
	
	// do not attempt if outside our status array
	if (start < 0 || start+3 >= MAX_STATUS) return;
	
	short n = xtoi(c);
	status[start] = n & 1;
	status[start+1] = n & 2;
	status[start+2] = n & 4;
	status[start+3] = n & 8;
}

/**
 * Take the savefile camaign= (256 hex chars) and convert to status array (1024 bools)
 */
void CampaignManager::setAll(std::string s) {
	for (int i=0; i<s.length(); i++) {
		setHex(s.at(i), i*4); // 4 = bits per hex
	}
}

/**
 * Convert status array (1024 bools) to savefile campaign= (256 hex chars)
 */
std::string CampaignManager::getAll() {
	stringstream ss;
	ss.str("");
	for (int i=0; i<MAX_STATUS; i+=4) {
		ss << btox(status[i], status[i+1], status[i+2], status[i+3]);
	}
	return ss.str();
}

CampaignManager::~CampaignManager() {

}
