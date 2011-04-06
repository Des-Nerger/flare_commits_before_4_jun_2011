/**
 * class CampaignManager
 *
 * Contains data for story mode
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef CAMPAIGN_MANAGER_H
#define CAMPAIGN_MANAGER_H

#include <string>
#include <sstream>
#include "UtilsParsing.h"
#include "NPC.h"

const int MAX_STATUS = 1024;

class CampaignManager {
private:

public:
	CampaignManager();
	~CampaignManager();
	
	void clearAll();
	void setAll(std::string s);
	std::string getAll();
	bool checkStatus(std::string s);
	void addStatus(std::string s);
	void clearRewards();
	
	// helper functions to process NPC dialog
	int chooseDialogNode(NPC *n);
	bool processDialog(NPC *n, int dialog_node, int &event_cursor);
	
	string status[MAX_STATUS];
	int status_count;
	
	// if an event has generated a reward, set notification here
	int xp_amount;
	int currency_amount;
	int item_amount;
	int item_id;
	string log_msg;
};


#endif
