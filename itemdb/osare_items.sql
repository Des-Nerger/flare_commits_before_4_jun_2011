create table base_items (
	id int not null primary key,
	name varchar(32),
	level int,
	type varchar(16),
	icon32 int,
	icon64 int,
	dmg_min int,
	dmg_max int,
	abs_min int,
	abs_max int,
	req_stat varchar(16),
	req_val int,
	sfx varchar(32),
	gfx varchar(32),
	loot varchar(32)
);

create table item_mods (
	id int not null primary key,
	prefix varchar(32),
	suffix varchar(32),
	quality varchar(16),
	level int,
	dmg_min int,
	dmg_max int,
	abs_min int,
	abs_max int,
	bonus_stat varchar(16),
	bonus_pval int,
	bonus_mval int,
	bonus_oval int,
	bonus_dval int,
	bonus_art int,
	effect varchar(32)
);

create table items (
	id int not null primary key,
	base_type int,
	item_mod int,
	name varchar(32)
);

// DATA

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (1,"Dagger",2,"main",0,96,2,6,NULL,NULL,"p",2,"metal","dagger","dagger");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (2,"Shortsword",4,"main",1,97,3,8,NULL,NULL,"p",3,"metal","shortsword","shortsword");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (3,"Longsword",6,"main",2,98,4,10,NULL,NULL,"p",4,"metal","longsword","longsword");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (4,"Greatsword",8,"main",3,99,5,12,NULL,NULL,"p",5,"metal","greatsword","greatsword");

--

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (5,"Wand",2,"main",4,100,2,6,NULL,NULL,"m",2,"wood","wand","wand");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (6,"Rod",4,"main",5,101,3,8,NULL,NULL,"m",3,"wood","rod","rod");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (7,"Staff",6,"main",6,102,4,10,NULL,NULL,"m",4,"wood","staff","staff");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (8,"Greatstaff",8,"main",7,103,5,12,NULL,NULL,"m",5,"wood","greatstaff","greatstaff");

--

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (9,"Slingshot",2,"off",8,104,2,6,NULL,NULL,"o",2,"wood","slingshot","slingshot");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (10,"Shortbow",4,"off",9,105,3,8,NULL,NULL,"o",3,"wood","shortbow","shortbow");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (11,"Longbow",6,"off",10,106,4,10,NULL,NULL,"o",4,"wood","longbow","longbow");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (12,"Greatbow",8,"off",11,107,5,12,NULL,NULL,"o",5,"wood","greatbow","greatbow");

--

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (13,"Clothes",1,"body",16,112,NULL,NULL,0,0,"d",1,"cloth","clothes","clothes");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (14,"Leather Armor",2,"body",12,108,NULL,NULL,1,1,"d",2,"leather","leather_armor","leather_armor");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (15,"Buckler",4,"off",13,109,NULL,NULL,0,2,"d",3,"wood","buckler","buckler");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (16,"Steel Armor",6,"body",14,110,NULL,NULL,2,2,"d",4,"maille","steel_armor","steel_armor");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (17,"Shield",8,"off",15,111,NULL,NULL,1,3,"d",5,"heavy","shield","shield");

--

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (18,"Belt",3,"artifact",19,115,NULL,NULL,NULL,NULL,NULL,NULL,"leather",NULL,"belt");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (19,"Boots",4,"artifact",17,113,NULL,NULL,NULL,NULL,NULL,NULL,"leather",NULL,"boots");

insert into base_items(id,name,level,type,icon64,icon32,dmg_min,dmg_max,abs_min,abs_max,req_stat,req_val,sfx,gfx,loot)
values (20,"Ring",7,"artifact",18,114,NULL,NULL,NULL,NULL,NULL,NULL,"gem",NULL,"ring");

--

select id,prefix,suffix,bonus_stat from item_mods;

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (1,"Rusted ",NULL,"low",-1,-1,-1,-1,0,NULL,NULL,NULL,NULL,NULL,NULL);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (2,"Cracked ",NULL,"low",-1,-1,-1,-1,0,NULL,NULL,NULL,NULL,NULL,NULL);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (3,"Cursed ",NULL,"low",-1,0,0,0,0,"accuracy",-3,-3,-4,-2,-5);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (4,"Cursed ",NULL,"low",-1,0,0,0,0,"avoidance",-3,-3,-2,-4,-5);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (5,"Dull ",NULL,"low",-1,0,0,0,0,"crit",-1,-1,-1,-1,-1);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (6,"Depleted ",NULL,"low",-1,0,0,0,0,"crit",-1,-1,-1,-1,-1);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (7,"Warped ",NULL,"low",-1,0,0,0,0,"crit",-1,-1,-1,-1,-1);

---

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (8,"Dwarven ",NULL,"high",1,0,0,0,0,"health",4,2,3,3,5);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (9,NULL," of Restoration","high",2,0,0,0,0,"health regen",1,1,1,1,2);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (10,"Elven ",NULL,"high",1,0,0,0,0,"mana",2,4,3,3,5);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (11,NULL," of Meditation","high",2,0,0,0,0,"mana regen",1,1,1,1,2);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (12,"Battle ",NULL,"high",2,1,1,0,1,NULL,NULL,NULL,NULL,NULL,NULL);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (13,NULL," of Accuracy","high",1,0,0,0,0,"accuracy",3,3,4,2,5);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (14,NULL," of Avoidance","high",1,0,0,0,0,"avoidance",3,3,2,4,5);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (15,"Summer ",NULL,"high",1,0,0,0,0,"fire resist",8,8,6,10,20);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (16,"Winter ",NULL,"high",1,0,0,0,0,"ice resist",8,8,6,10,20);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (17,NULL," of Slaying","high",2,0,0,0,0,"crit",1,1,1,1,2);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (18,NULL," of Speed","high",2,0,0,0,0,"speed",1,1,1,1,3);

---

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (19,"Warlord ",NULL,"high",3,0,1,0,1,"health",7,5,6,6,10);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (20,NULL," of Victory","high",4,0,1,0,1,"health regen",2,2,2,2,4);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (21,"Archmage ",NULL,"high",3,0,1,0,1,"mana",5,7,6,6,10);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (22,NULL," of Wizardry","high",4,0,1,0,1,"mana regen",2,2,2,2,4);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (23,"Ancient ",NULL,"high",4,2,2,1,1,NULL,NULL,NULL,NULL,NULL,NULL);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (24,NULL," of Precision","high",3,0,1,0,1,"accuracy",6,6,7,5,10);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (25,NULL," of Protection","high",3,0,1,0,1,"avoidance",6,6,5,7,10);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (26,"Fiery ",NULL,"high",3,0,1,0,1,"fire resist",11,11,9,15,25);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (27,"Frozen ",NULL,"high",3,0,1,0,1,"ice resist",11,11,9,15,25);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (28,NULL," of Destruction","high",4,0,1,0,1,"crit",2,2,2,2,4);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (29,NULL," of Travel","high",4,0,1,0,1,"speed",2,2,2,2,6);

---

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (30,"Epic Health ",NULL,"epic",5,2,2,1,1,"health",10,8,9,9,15);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (31,"Epic Health Regen ",NULL,"epic",6,2,2,1,1,"health regen",3,3,3,3,6);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (32,"Epic Mana ",NULL,"epic",5,2,2,1,1,"mana",8,10,9,9,15);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (33,"Epic Mana Regen ",NULL,"epic",6,2,2,1,1,"mana regen",3,3,3,3,6);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (34,"Epic Accuracy ",NULL,"epic",5,2,2,1,1,"accuracy",9,9,10,8,15);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (35,"Epic Avoidance ",NULL,"epic",5,2,2,1,1,"avoidance",9,9,8,10,15);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (36,"Epic Fire Resist ",NULL,"epic",5,2,2,1,1,"fire resist",14,14,12,20,30);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (37,"Epic Frost Resist ",NULL,"epic",5,2,2,1,1,"ice resist",14,14,12,20,30);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (38,"Epic Crit ",NULL,"epic",6,2,2,1,1,"crit",3,3,3,3,6);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (39,"Epic Speed ",NULL,"epic",6,2,2,1,1,"speed",3,3,3,3,9);

---

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (40,"Cracked ",NULL,"low",0,0,0,0,-1,NULL,NULL,NULL,NULL,NULL,NULL);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (41,"Tattered ",NULL,"low",0,0,0,0,0,NULL,NULL,NULL,NULL,NULL,NULL);

insert into item_mods (id, prefix, suffix, quality, level, dmg_min, dmg_max, abs_min, abs_max, bonus_stat, bonus_pval, bonus_mval, bonus_oval, bonus_dval, bonus_art)
values (42,"Tarnished ",NULL,"low",0,0,0,0,0,NULL,NULL,NULL,NULL,NULL,NULL);


