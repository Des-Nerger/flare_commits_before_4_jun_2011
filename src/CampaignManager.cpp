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

void CampaignManager::setStatus(std::string s) {

	// hit upper limit for status
	// TODO: add a warning
	if (status_count >= MAX_STATUS) return;
	
	// if it's already set, don't add it again
	if (checkStatus(s)) return;
	
	status[status_count++] = s;
}

void CampaignManager::unsetStatus(std::string s) {
	for (int i=status_count-1; i>=0; i--) {
		if (status[i] == s) {
		
			// bubble existing statuses down
			for (int j=i; j<status_count-1; j++) {
				status[j] = status[j+1];
			}
			status_count--;
			return;
		}
	}
}


CampaignManager::~CampaignManager() {

}
