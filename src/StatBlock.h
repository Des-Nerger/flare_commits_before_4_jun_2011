/**
 * class StatBlock
 *
 * Character stats and calculations
 *
 * @author Clint Bellanger
 * @license GPL
 */

#ifndef STAT_BLOCK_H
#define STAT_BLOCK_H

#include <string>
using namespace std;

class StatBlock {
private:

public:
	StatBlock();
	~StatBlock();
	
	void takeDamage(int dmg);
	void recalc();
	void logic();

	bool alive;
	
	string name;
	int level;
	
	// base stats
	int offense;
	int defense;
	int physical;
	int magical;
	
	// derived stats
	int physoff;
	int physdef;
	int magoff;
	int magdef;
	
	// physical stats
	int hp;
	int maxhp;
	int frames_per_hp;
	int hp_ticker;

	// magical stats
	int mp;
	int maxmp;
	int frames_per_mp;
	int mp_ticker;
	
	// offense stats
	int accuracy;
	
	// defense stats
	int avoidance;

	// overall stats
	int crit;

	// equipment stats	
	int dmg_melee_min;
	int dmg_melee_max;
	int dmg_magic_min;
	int dmg_magic_max;
	int dmg_ranged_min;
	int dmg_ranged_max;
	int minabsorb;
	int maxabsorb;	
	int resist_fire;
	int resist_ice;
	
	// buff and debuff stats
	bool slowed;
	int slowed_duration;
	bool bleeding;
	int bleeding_duration;
	bool stunned;
	int stunned_duration;
	
	// power stats
	int temphp;
	bool offensive_stance;
	bool defensive_stance;
	bool reflect;
};

#endif
