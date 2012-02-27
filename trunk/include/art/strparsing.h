//Link error 2005 when definitions are in same file...
#ifndef STRPARSING_H
#define STRPARSING_H

#include <vector>
#include <sstream>
#include <string>

using std::string;
using std::stringstream;
using std::vector;

vector<string> strsplit(const string &s, const char delim) ;
string strcrop(const string &s) ;

#endif
