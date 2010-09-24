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

unsigned short xtoi(string hex);
string trim(string s, char c);
string parse_section_title(string s);
void parse_key_pair(string s, string &key, string &val);
int eatFirstInt(string &s, char separator);
unsigned eatFirstHex(string &s, char separator);
string eatFirstString(string &s, char separator);

#endif
