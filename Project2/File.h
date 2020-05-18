#pragma once
#include <string>
#include <time.h>

using namespace std;

namespace explorer {
	class File {
	public:
		File(string content);
		File(void);
		File(int modifiers);
		File(string content, int modifiers);
		string read(void);
		time_t *getLastModified(void);
		void modify(string newcontent);
		int getAccess(void);
		static string accessToString(int access) {
			string res("");
			if (access & ACCESS_READ) {
				res = res + "READ ";
			}
			if (access & ACCESS_WRITE) {
				res = res + "WRITE ";
			}
			if (access & ACCESS_DELETE) {
				res = res + "DELETE ";
			}
			return res;
		}
		static const int DEFAULT_ACCESS = 7;
		static const int ACCESS_READ = 1;
		static const int ACCESS_WRITE = 2;
		static const int ACCESS_DELETE = 4;
	private:
		string content;
		time_t modified;
		int access;
	};
}