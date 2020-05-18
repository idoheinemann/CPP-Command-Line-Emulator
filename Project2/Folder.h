#pragma once
#include <string>
#include <time.h>
#include <unordered_map>
#include "File.h"

using namespace std;

namespace explorer {
	class Folder {
	public:
		Folder(Folder* parent, string name);
		Folder(string name);
		Folder();
		Folder *getParent(void);
		File *getFile(string fname);
		Folder *getFolder(string fname);
		bool isRoot(void);
		bool hasFile(string fname);
		bool hasFolder(string fname);
		string getname(void);
		void rename(string newname);
		void mkdir(string name);
		void rmdir(string name);
		void mkfile(string name);
		void mkfile(string name,string content);
		void mkfile(string name,int access);
		void mkfile(string name, string content,int access);
		void delfile(string name);
		string tree();
	private:
		string name;
		string tree(string nopathname,int numtab);
		unordered_map< string, File > files = {};
		unordered_map< string, Folder > folders = {};
		Folder* parent;
	};
}