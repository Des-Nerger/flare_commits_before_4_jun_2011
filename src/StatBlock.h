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
#include <fstream>
#include "Settings.h"
#include "Utils.h"
#include "UtilsParsing.h"
using namespace std;

class StatBlock {
private:

public:
	StatBlock();
	~StatBlock();
	
	void load(string filename);
	void takeDamage(int dmg);
	void recalc();
	void logic();

	bool alive;
	
	string name;
	string file_prefix;
	
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
	int hp_per_minute;
	int hp_ticker;

	// magical stats
	int mp;
	int maxmp;
	int mp_per_minute;
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
	int absorb_min;
	int absorb_max;	
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
	
	int speed;
	int dspeed;
	Point pos;
	int direction;
		
	// enemy behavioral stats
	int chance_melee_phys;
	int chance_melee_mag;
	int chance_ranged_phys;
	int chance_ranged_mag;
	int melee_range;
	int threat_range;
	Point hero_pos;
	Point last_seen; 
	int dir_favor;
	int dir_ticks;
	int patrol_ticks;
	bool in_combat;
	int cooldown;
	
	// rendering info
	int cur_state;
	int cur_frame;
	int disp_frame;
	
	Point render_size;
	Point render_offset;

	int anim_stance_position;
	int anim_stance_frames;
	int anim_stance_duration;
	int anim_run_position;
	int anim_run_frames;
	int anim_run_duration;
	int anim_melee_position;
	int anim_melee_frames;
	int anim_melee_duration;
	int anim_magic_position;
	int anim_magic_frames;
	int anim_magic_duration;
	int anim_ranged_position;
	int anim_ranged_frames;
	int anim_ranged_duration;
	int anim_block_position;
	int anim_block_frames;
	int anim_block_duration;
	int anim_hit_position;
	int anim_hit_frames;
	int anim_hit_duration;
	int anim_die_position;
	int anim_die_frames;
	int anim_die_duration;
	int anim_critdie_position;
	int anim_critdie_frames;
	int anim_critdie_duration;

};

#endif
