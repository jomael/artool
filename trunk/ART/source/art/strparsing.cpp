
#ifndef STRPARSING_CPP
#define STRPARSING_CPP

#include "strparsing.h"
#include <vector>
#include <sstream>
#include <string>
#include <iostream>

using std::string;
using std::stringstream;
using std::vector;

vector<string> strsplit(const string &s, const char delim) ;
string strcrop(const string &s) ;

int TEST;
vector<int> VECTOR_TEST;
string STRING_TEST;
stringstream STRINGSTREAM_TEST;

vector<string> strsplit(const string &s, const char delim) 
{
	vector<string> elems;
	stringstream ss(s);
	string item;
	while(std::getline(ss, item, delim)) 
	{
		elems.push_back(item);
	}
	return elems;
}

string strcrop(const string &s) 
{
	size_t a = s.find_first_not_of(" \t\n");
	//if no non-whitespace character is found, return empty string
	if (a == string::npos) return "";
	size_t l = s.find_last_not_of(" \t\n");
	return s.substr(a, 1 + l - a);
}

#endif

