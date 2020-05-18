#include <iostream>
#include <string>
#include <regex>
#include <iterator>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <time.h>
#include "Parser.h"
#include "Folder.h"
#include "File.h"
#include "string_generator.h"
#pragma warning(disable:4996)

#define _SCL_SECURE_NO_WARNINGS

using namespace std;

using namespace explorer;

unordered_map<string, string> help_map = {
	{ "mkdir","mkdir name\r\ncreates a new directory" },
	{ "mkfile","mkfile name [\\a access] [\\c context]\r\ncreates a new file with the specified access modifiers and context\r\naccess -> r - read, w - write, d - delete\r\nExample: mkfile file.txt \\a rw \\c I am a File" },
	{ "exit","exits the command line" },
	{ "checkdir","checkdir name\r\ngives you dir information about the specified directory" },
	{ "cd","cd path/\r\nchanges the home directory to the last directory in the path\r\nmust end with a / sign to inform the system that it is a directory" },
	{ "read","read file\r\nechos the file's content" },
	{ "delete","delete file\r\ndeletes the file" },
	{ "write","write file content\r\nre-writes the file, if file does not exist it creates a new file with default access premition" },
	{ "finfo","finfo file\r\ngives information about the file" },
	{ "tree","shows all existing folders and files inside this directory and all child directories" },
	{ "help","help [command]\r\nshows the help page about the command\r\nif the command is not specified, it will show information about all commands" }
};

bool parse_string(string* raw_input) {
	string full_input = string_trim(raw_input);
	string oporation = regex_replace(full_input, regex("\\s+.*"),"");
	string input = string_trim(&full_input.substr(oporation.length(),full_input.length()-oporation.length()));
	if (full_input.empty()) {
		return false;
	}
	else if (test_string(&oporation,"exit")) { // exit program
		return true;
	}
	else if (test_string(&oporation,"mkdir")) { // make new dir
		folderC->mkdir(input);
		cout << "Created Directory " << folderC->getFolder(input)->getname() << endl;
	}
	else if (test_string(&oporation,"checkdir")){
		if(folderC->hasFolder(input)) {
			cout << "Adderss: " << folderC->getFolder(input) << endl;
			cout << "Path: " << (*(folderC->getFolder(input))).getname() << endl;
		}
		else {
			cout << "Folder does not exist!" << endl;
		}
	}
	else if (test_string(&oporation, "cd")) {
		string cdir = input;
		string tdir;
		while (cdir.find('/')!=-1) {
			tdir = regex_replace(cdir, regex("\\/.*"), "");
			if (test_string(&tdir, "..")) {
				if (!folderC->isRoot()) {
					folderC = folderC->getParent();
				}
			}
			else if (folderC->hasFolder(tdir)) {
				folderC = folderC->getFolder(tdir);
			}
			else {
				cout << "Folder does not exist!" << endl;
				break;
			}
			cdir = cdir.substr(tdir.length() + 1, cdir.length() - tdir.length() - 1);
		}
	}
	else if (test_string(&oporation,"mkfile")) {
		string_generator generator(input.c_str(), " ");
		string name = *generator.getNext();
		if (generator.is_end()) {
			return false;
		}
		string gennext = *generator.getNext();
		int ac = 0;
		if (test_string(&gennext, "\\a")) {
			string access = *generator.getNext();
			if (access.find('r') != string::npos) {
				ac += File::ACCESS_READ;
			}
			if (access.find('w') != string::npos) {
				ac += File::ACCESS_WRITE;
			}
			if (access.find('d') != string::npos) {
				ac += File::ACCESS_DELETE;
			}
		}
		else {
			ac = File::DEFAULT_ACCESS;
		}
		string content = "";
		if (test_string(&gennext, "\\c") || test_string(generator.getNext(), "\\c")) {
			while (!generator.is_end()) {
				content += *generator.getNext() + " ";
			}
		}
		folderC->mkfile(name,content,ac);
	}
	else if (test_string(&oporation, "read")) {
		if (folderC->hasFile(input)) {
			if (folderC->getFile(input)->getAccess() & File::ACCESS_READ) {
				cout << folderC->getFile(input)->read() << endl;
			}
			else {
				cout << "Access violation!\r\nFile cannot be read" << endl;
			}
		}
		else {
			cout << "File does not exist!" << endl;
		}
	}
	else if (test_string(&oporation,"delete")) {
		if (folderC->hasFile(input)) {
			if (folderC->getFile(input)->getAccess() & File::ACCESS_DELETE) {
				cout << "File " << folderC->getname() << "\\" << input << " deleted" << endl;
				folderC->delfile(input);
			}
			else {
				cout << "Access violation!\r\nFile cannot be deleted" << endl;
			}
		}
		else {
			cout << "File does not exist!" << endl;
		}
	}
	else if (test_string(&oporation, "write")) {
		string_generator generator(input.c_str()," ");
		string name;
		if (folderC->hasFile(name = *generator.getNext())) {
			if (folderC->getFile(name)->getAccess() & File::ACCESS_WRITE) {
				string res;
				while (!generator.is_end()) {
					res = res + *generator.getNext();
				}
				folderC->getFile(name)->modify(res);
			}
			else {
				cout << "Access violation!\r\nFile cannot be re-writen" << endl;
			}
		}
		else {
			string res;
			while (!generator.is_end()) {
				res = res + *generator.getNext();
			}
			folderC->mkfile(name,res);
		}
	}
	else if (test_string(&oporation, "finfo")) {
		if (folderC->hasFile(input)) {
			cout << "Path: " << folderC->getname() << "\\" << input << endl;
			cout << "Access: " << File::accessToString(folderC->getFile(input)->getAccess()) << endl;
			_SCL_SECURE_NO_WARNINGS  
			cout << "Last Modified: " << ctime(folderC->getFile(input)->getLastModified()) << endl;
		}
		else {
			cout << "File does not exist!" << endl;
		}
	}
	else if (test_string(&oporation, "tree")) {
		cout << folderC->tree() << endl;
	}
	else if (test_string(&oporation,"help")) {
		if (input.empty()) {
			for (auto command : help_map) {
				cout << command.first << ":" << endl;
				cout << command.second << endl;
			}
		}
		else {
			if (help_map.find(input) != help_map.end()) {
				cout << help_map[input] << endl;
			}
			else {
				cout << "Command not found\r\ntype 'help' for additional information" << endl;
			}
		}
	}

	else {
		cout << "Unknown command\r\nType 'help' for list of all commands" << endl;
	}
	return false;
}

bool test_string(string* input, const char* test) {
	return !strncmp(input->c_str(), test, input->length());
}

bool test_regex(string* input, regex reg) {
	return regex_match(input->c_str(), reg);
}
string string_trim(string* str) {
	return regex_replace(regex_replace(*str,regex("^\\s*"),""),regex("\\s*$"),"");
	
}

// deprecated
unsigned long gethash(string* inp){
	const char* chptr = inp->c_str();
	int index = 0;
	unsigned long hash = 0;
	while ((int)(*(chptr + index)) != 0) {
		hash += (unsigned long)(*(chptr + index))*(unsigned long)(powl(100, index));
		index++;
	}
	return hash;
}

string mulstr(string src, unsigned int num) {
	string res = "";
	for (register int i = 0; i != num; i++) {
		res = res + src;
	}
	return res;
}
