/*
 *  UtilsParsing.h
 *  RPGEngine
 *
 *  Created by Clint Bellanger on 5/25/10.
 *  Copyright 2010 Clint Bellanger. All rights reserved.
 *
 */

#ifndef UTILS_PARSING_H
#define UTILS_PARSING_H

#include <string>
using namespace std;


static unsigned short xtoi(string hex) {
	char c0 = hex.at(0);
	char c1 = hex.at(1);
	unsigned short val;
	
	if (c0 >= 97) val = c0 - 87;
	else if (c0 >= 65) val = c0 - 55;
	else val = c0 - 48;
	val = val * 16;
	
	if (c1 >= 97) val = val + c1 - 87;
	else if (c1 >= 65) val = val + c1 - 55;
	else val = val + c1 - 48;
	
	return val;
}

/**
 * trim: remove leading and trailing c from s
 */
static string trim(string s, char c) {
	int first = 0;
	int last = s.length()-1;
	if (last < 0) return "";

	while (s.at(first) == c && first < s.length()-1) {
		first++;
	}
	while (s.at(last) == c && last >= first) {
		last--;
	}
	if (first <= last) return s.substr(first,last-first+1);
	return "";
}

static string parse_section_title(string s) {
	return s.substr(1, s.find_first_of(']') -1);
}

static void parse_key_pair(string s, string &key, string &val) {
	int separator = s.find_first_of('=');
	key = s.substr(0, separator);
	val = s.substr(separator+1, s.length());
}

/**
 * Given a string that starts with a number then a comma
 * Return that int, and modify the string to remove the num and comma
 *
 * This is basically a really lazy "split" replacement
 */
static int eatFirstInt(string &s, char separator) {
	int seppos = s.find_first_of(separator);
	int num = atoi(s.substr(0, seppos).c_str());
	s = s.substr(seppos+1, s.length());
	return num;
}

static unsigned eatFirstHex(string &s, char separator) {
	int seppos = s.find_first_of(separator);
	unsigned short num = xtoi(s.substr(0, seppos));
	s = s.substr(seppos+1, s.length());
	return num;
}

static string eatFirstString(string &s, char separator) {
	int seppos = s.find_first_of(separator);
	string outs = s.substr(0, seppos);
	s = s.substr(seppos+1, s.length());
	return outs;
}

#endif
