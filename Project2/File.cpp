#include <iostream>
#include <string>
#include <time.h>
#include "File.h"

using namespace std;
namespace explorer {
	File::File(string content) {
		this->content = content;
		this->access = DEFAULT_ACCESS;
		this->modified = time(0);
	}
	File::File(string content,int modifiers) {
		this->content = content;
		this->modified = time(0);
		access = modifiers;
	}
	File::File(void) {
		this->access = DEFAULT_ACCESS;
		this->content = "";
	}
	File::File(int modifiers) {
		this->access = modifiers;
	}
	time_t* File::getLastModified(void) {
		return &modified;
	}
	string File::read(void) {
		if (access & ACCESS_READ) {
			return content;
		}
		return NULL;
	}
	void File::modify(string newcontent) {
		if (access & ACCESS_WRITE) {
			content = newcontent;
			modified = time(0);
		}
	}
	int File::getAccess(void) {
		return access;
	}
}