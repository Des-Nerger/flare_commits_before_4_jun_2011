/**
 * class MenuPowers
 *
 * @author Clint Bellanger
 * @license GPL
 */

#include "MenuPowers.h"

MenuPowers::MenuPowers(SDL_Surface *_screen, FontEngine *_font, StatBlock *_stats) {
	screen = _screen;
	font = _font;
	stats = _stats;
	
	visible = false;
	loadGraphics();
}

void MenuPowers::loadGraphics() {

	background = IMG_Load("images/menus/powers.png");
	powers_step = IMG_Load("images/menus/powers_step.png");
	powers_unlock = IMG_Load("images/menus/powers_unlock.png"); 
	if(!background || !powers_step || !powers_unlock) {
		fprintf(stderr, "Couldn't load image: %s\n", IMG_GetError());
		SDL_Quit();
	}
	
}

void MenuPowers::render() {
	if (!visible) return;
	
	SDL_Rect src;
	SDL_Rect dest;
	
	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	
	// background
	src.x = 0;
	src.y = 0;
	dest.x = offset_x;
	dest.y = offset_y;
	src.w = dest.w = 320;
	src.h = dest.h = 416;
	SDL_BlitSurface(background, &src, screen, &dest);
	
	// text overlay
	// TODO: translate()
	font->render("Powers", offset_x+160, offset_y+8, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Physical", offset_x+64, offset_y+50, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Physical", offset_x+128, offset_y+50, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Magical", offset_x+192, offset_y+50, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Magical", offset_x+256, offset_y+50, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Offense", offset_x+64, offset_y+66, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Defense", offset_x+128, offset_y+66, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Offense", offset_x+192, offset_y+66, JUSTIFY_CENTER, screen, FONT_WHITE);
	font->render("Defense", offset_x+256, offset_y+66, JUSTIFY_CENTER, screen, FONT_WHITE);
	
	// stats
	stringstream ss;
	ss.str("");
	ss << stats->physoff;
	font->render(ss.str(), offset_x+64, offset_y+34, JUSTIFY_CENTER, screen, FONT_WHITE);
	ss.str("");
	ss << stats->physdef;
	font->render(ss.str(), offset_x+128, offset_y+34, JUSTIFY_CENTER, screen, FONT_WHITE);
	ss.str("");
	ss << stats->magoff;
	font->render(ss.str(), offset_x+192, offset_y+34, JUSTIFY_CENTER, screen, FONT_WHITE);
	ss.str("");
	ss << stats->magdef;
	font->render(ss.str(), offset_x+256, offset_y+34, JUSTIFY_CENTER, screen, FONT_WHITE);
	
	
	
	// highlighting
	displayBuild(stats->physoff, offset_x+48);
	displayBuild(stats->physdef, offset_x+112);
	displayBuild(stats->magoff, offset_x+176);
	displayBuild(stats->magdef, offset_x+240);	
}

void MenuPowers::displayBuild(int value, int x) {
	SDL_Rect src_step;
	SDL_Rect src_unlock;
	SDL_Rect dest;
	
	src_step.x = src_unlock.x = 0;
	src_step.y = src_unlock.y = 0;
	src_step.w = 32;
	src_step.h = 19;
	src_unlock.w = 32;
	src_unlock.h = 45;

	dest.x = x;
	int offset_y = (VIEW_H - 416)/2;
	
	for (int i=3; i<= value; i++) {
		if (i%2 == 0) { // even stat
			dest.y = i * 32 + offset_y + 48;
			SDL_BlitSurface(powers_step, &src_step, screen, &dest);
		}
		else { // odd stat
			dest.y = i * 32 + offset_y + 35;
			SDL_BlitSurface(powers_unlock, &src_unlock, screen, &dest);
		
		}
	}
}

TooltipData MenuPowers::checkTooltip(Point mouse) {

	TooltipData tip;

	int offset_x = (VIEW_W - 320);
	int offset_y = (VIEW_H - 416)/2;
	
	if (mouse.y >= offset_y+32 && mouse.y <= offset_y+80) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80) {
			tip.lines[tip.num_lines++] = "Physical + Offense grants melee and ranged attacks";
			return tip;
		}
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144) {
			tip.lines[tip.num_lines++] = "Physical + Defense grants melee protection";
			return tip;
		}
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208) {
			tip.lines[tip.num_lines++] = "Magical + Offense grants elemental spell attacks";
			return tip;
		}
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272) {
			tip.lines[tip.num_lines++] = "Magical + Defense grants healing and magic protection";
			return tip;
		}
	}
	else if (mouse.y >= offset_y+80 && mouse.y <= offset_y+112) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80) {
			tip.lines[tip.num_lines++] = "Shoot";
			tip.lines[tip.num_lines++] = "Basic Ranged Attack";
			return tip;
		}
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144) {
			tip.lines[tip.num_lines++] = "Swing";
			tip.lines[tip.num_lines++] = "Basic Melee Attack";
			return tip;
		}
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208) {
			tip.lines[tip.num_lines++] = "Lore";
			tip.lines[tip.num_lines++] = "Identify magic items";
			return tip;
		}
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272) {
			tip.lines[tip.num_lines++] = "Return";
			tip.lines[tip.num_lines++] = "Warp to your chosen Sanctuary";	
			return tip;
		}
	}
	else if (mouse.y >= offset_y+144 && mouse.y <= offset_y+176) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80) {
			tip.lines[tip.num_lines++] = "Blood Strike";
			tip.lines[tip.num_lines++] = "Cause target to Bleed for a few seconds";
			if (stats->physoff < 3) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Physical Offense 3";
			return tip;
		}
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144) {
			tip.lines[tip.num_lines++] = "Block";
			tip.lines[tip.num_lines++] = "Increase avoidance and absorption";
			if (stats->physdef < 3) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Physical Defense 3";
			return tip;
		}
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208) {
			tip.lines[tip.num_lines++] = "Bolt";
			tip.lines[tip.num_lines++] = "Conjure electricity that arcs between targets";
			if (stats->magoff < 3) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Magical Offense 3";
			return tip;
		}
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272) {
			tip.lines[tip.num_lines++] = "Heal";
			tip.lines[tip.num_lines++] = "Restore health";
			if (stats->magdef < 3) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Magical Defense 3";	
			return tip;
		}
	}
	else if (mouse.y >= offset_y+208 && mouse.y <= offset_y+240) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80) {
			tip.lines[tip.num_lines++] = "Multishot";
			tip.lines[tip.num_lines++] = "Fire three projectiles in one shot";
			if (stats->physoff < 5) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Physical Offense 5";
			return tip;
		}
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144) {
			tip.lines[tip.num_lines++] = "Warcry";
			tip.lines[tip.num_lines++] = "Become immune to debuffs for a short time and scare nearby enemies";
			if (stats->physdef < 5) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Physical Defense 5";
			return tip;
		}
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208) {
			tip.lines[tip.num_lines++] = "Quake";
			tip.lines[tip.num_lines++] = "Shatter the earth and stun nearby enemies";
			if (stats->magoff < 5) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Magical Offense 5";
			return tip;
		}
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272) {
			tip.lines[tip.num_lines++] = "Shield";
			tip.lines[tip.num_lines++] = "Absorbs damage";
			if (stats->magdef < 5) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Magical Defense 5";		
			return tip;
		}
	}
	else if (mouse.y >= offset_y+272 && mouse.y <= offset_y+304) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80) {
			tip.lines[tip.num_lines++] = "Cleave";
			tip.lines[tip.num_lines++] = "Strike all enemies in a wide arc";
			if (stats->physoff < 7) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Physical Offense 7";
			return tip;
		}
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144) {
			tip.lines[tip.num_lines++] = "Charge";
			tip.lines[tip.num_lines++] = "Bull rush the target and knock it back";
			if (stats->physdef < 7) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Physical Defense 7";
			return tip;
		}
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208) {
			tip.lines[tip.num_lines++] = "Freeze";
			tip.lines[tip.num_lines++] = "Slow enemies in a straight line";
			if (stats->magoff < 7) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Magical Offense 7";
			return tip;
		}
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272) {
			tip.lines[tip.num_lines++] = "Teleport";
			tip.lines[tip.num_lines++] = "Warp instantly to the target location";
			if (stats->magdef < 7) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Magical Defense 7";	
			return tip;
		}
	}
	else if (mouse.y >= offset_y+336 && mouse.y <= offset_y+368) {
		if (mouse.x >= offset_x+48 && mouse.x <= offset_x+80) {
			tip.lines[tip.num_lines++] = "Piercing Shot";
			tip.lines[tip.num_lines++] = "Ignore armor and shoot straight through enemies";
			if (stats->physoff < 9) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Physical Offense 9";
			return tip;
		}
		if (mouse.x >= offset_x+112 && mouse.x <= offset_x+144) {
			tip.lines[tip.num_lines++] = "Vengeance";
			tip.lines[tip.num_lines++] = "After blocking, launch a heavy accurate attack";
			if (stats->physdef < 9) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Physical Defense 9";
			return tip;
		}
		if (mouse.x >= offset_x+176 && mouse.x <= offset_x+208) {
			tip.lines[tip.num_lines++] = "Burn";
			tip.lines[tip.num_lines++] = "Evoke devastating flames in a distant area";
			if (stats->magoff < 9) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Magical Offense 9";
			return tip;
		}
		if (mouse.x >= offset_x+240 && mouse.x <= offset_x+272) {
			tip.lines[tip.num_lines++] = "Time Stop";
			tip.lines[tip.num_lines++] = "Pause the flow of time for a few moments";
			if (stats->magdef < 9) tip.colors[tip.num_lines] = FONT_RED;
			tip.lines[tip.num_lines++] = "Requires Magical Defense 9";		
			return tip;
		}
	}
	
	return tip;
}

MenuPowers::~MenuPowers() {
	SDL_FreeSurface(background);
	SDL_FreeSurface(powers_step);
	SDL_FreeSurface(powers_unlock);
}
