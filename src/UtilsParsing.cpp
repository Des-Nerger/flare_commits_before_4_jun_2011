#include "UtilsParsing.h"
using namespace std;

unsigned short xtoi(string hex) {

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
string trim(string s, char c) {
	if (s.length() == 0) return "";
	
	unsigned int first = 0;
	unsigned int last = s.length()-1;

	while (s.at(first) == c && first < s.length()-1) {
		first++;
	}
	while (s.at(last) == c && last >= first) {
		last--;
	}
	if (first <= last) return s.substr(first,last-first+1);
	return "";
}

string parse_section_title(string s) {
	unsigned int bracket = s.find_first_of(']');
	if (bracket == string::npos) return ""; // not found
	return s.substr(1, bracket-1);
}

void parse_key_pair(string s, string &key, string &val) {
	unsigned int separator = s.find_first_of('=');
	if (separator == string::npos) {
		key = "";
		val = "";
		return; // not found
	}
	key = s.substr(0, separator);
	val = s.substr(separator+1, s.length());
}

/**
 * Given a string that starts with a number then a comma
 * Return that int, and modify the string to remove the num and comma
 *
 * This is basically a really lazy "split" replacement
 */
int eatFirstInt(string &s, char separator) {
	unsigned int seppos = s.find_first_of(separator);
	if (seppos == string::npos) {
		s = "";
		return 0; // not found
	}
	int num = atoi(s.substr(0, seppos).c_str());
	s = s.substr(seppos+1, s.length());
	return num;
}

unsigned short eatFirstHex(string &s, char separator) {
	unsigned int seppos = s.find_first_of(separator);
	if (seppos == string::npos) {
		s = "";
		return 0; // not found
	}
	unsigned short num = xtoi(s.substr(0, seppos));
	s = s.substr(seppos+1, s.length());
	return num;
}

string eatFirstString(string &s, char separator) {
	unsigned int seppos = s.find_first_of(separator);
	if (seppos == string::npos) return ""; // not found
	string outs = s.substr(0, seppos);
	s = s.substr(seppos+1, s.length());
	return outs;
}

// strip carriage return if exists
string stripCarriageReturn(string line) {
	if (line.length() > 0) {
		if ('\r' == line.at(line.length()-1)) {
			return line.substr(0, line.length()-1);
		}
	}
	return line;
}

string getLine(ifstream &infile) {
	string line;
	getline(infile, line);
	line = stripCarriageReturn(line);
	return line; 
}

