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

const int MAX_STATUS = 1024;


class CampaignManager {
private:

	void setHex(char c, int start);

public:
	CampaignManager();
	~CampaignManager();
	void setAll(std::string s);
	std::string getAll();
	
	bool status[MAX_STATUS];
};


#endif
