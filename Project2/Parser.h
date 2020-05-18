#pragma once
#include <iostream>
#include <string>
#include <regex>
#include <iterator>
#include <cmath>
#include "Folder.h"
//#include "File.h"

using namespace std;

using namespace explorer;

bool parse_string(string* input);

bool test_string(string* input,const char* test);

bool test_regex(string* input, regex reg);

string string_trim(string* str);

unsigned long gethash(string* inp);

string mulstr(string src, unsigned int num);

extern class Folder *folderC;