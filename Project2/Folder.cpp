#include <iostream>
#include <string>
#include <map>
#include "Parser.h"
#include "Folder.h"
#include "File.h"

using namespace std;

namespace explorer {
	Folder::Folder(Folder* parent, string name) {
		this->parent = parent;
		this->name = name;
	}
	Folder::Folder(string name) {
		this->name = name;
		this->parent = NULL;
	}
	Folder::Folder() {
		this->name = "NULL";
		this->parent = NULL;
	}
	Folder* Folder::getParent(void) {
		return this->parent;
	}
	bool Folder::isRoot(void) {
		return parent == NULL;
	}
	File* Folder::getFile(std::string fname) {
		return &files[fname];
	}
	Folder* Folder::getFolder(string fname) {
		return &folders.find(fname)->second;
	}
	void Folder::rename(string newname) {
		string path = name.substr(0, name.find_last_of('\\'));
		parent->rmdir(name.substr(name.find_last_of('\\')+1,name.length()-name.find_last_of('\\')-1));
		name = path + "\\" + newname;
		parent->folders[newname] = *this;
	}
	void Folder::mkfile(string name) {
		File temp = File();
		files[name] = temp;
	}
	void Folder::mkfile(string name, string content) {
		File temp(content);
		files[name] = temp;
	}
	void Folder::mkfile(string name, int access) {
		File temp(access);
		files[name]=temp;
	}
	void Folder::mkfile(string name, string content, int access) {
		File temp(content, access);
		files[name]=temp;
	}
	void Folder::mkdir(string name) {
		Folder temp(this, this->name + "\\" + name);
		folders[name]=temp;
	}
	void Folder::rmdir(string name) {
		folders.erase(name);
	}
	string Folder::getname(void) {
		return name;
	}
	void Folder::delfile(string name) {
		if (files[name].getAccess()&explorer::File::ACCESS_DELETE) {
			files.erase(name);
		}
	}
	bool Folder::hasFile(string fname) {
		return files.find(fname) != files.end();
	}
	bool Folder::hasFolder(string fname) {
		return folders.find(fname) != folders.end();
	}
	string Folder::tree() {
		int tabnum = 1;
		string res(name + " -> Folders: \r\n");
		for (auto folder : folders) {
			res += folder.second.tree(folder.first,tabnum+1);
		}
		res = res + name + " -> Files:\r\n";
		for (auto file : files) {
			res += mulstr("    ", tabnum)+file.first+"\r\n";
		}
		return res;
	}
	string Folder::tree(string nopathname,int tabnum) {
		string res(mulstr("    ", tabnum-1) + nopathname + " -> Folders: \r\n");
		for (auto folder : folders) {
			res += folder.second.tree(folder.first,tabnum + 1);
		}
		res = res + mulstr("    ", tabnum - 1) + nopathname + " -> Files: \r\n";
		for (auto file : files) {
			res += mulstr("    ", tabnum) + file.first + "\r\n";
		}
		return res;
	}
}