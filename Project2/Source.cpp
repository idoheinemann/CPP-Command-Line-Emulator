#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include "Parser.h"
#include "Folder.h"
#include "File.h"

using namespace std;
using namespace explorer;

Folder* folderC;

int main(void)
{
	folderC = new Folder("C:");
	string input;
	// main loop
	while (true)
	{
		cout << folderC->getname() << "> ";
		getline(cin, input);
		if (parse_string(&input)) {
			break;
		}
	}
	return 0;
};