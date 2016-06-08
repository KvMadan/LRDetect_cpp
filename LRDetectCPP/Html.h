#pragma once

#include <string>
#include "Constants.h"
#include <Windows.h>
using namespace std;

namespace Html {

		string b(const string& content);
		string h1(const string& content);
		string h2(const string& content);

		//Table
		string table_open(string id = "", string class_name = "section");
		const string table_close = "\n\t</table>";
		const string br = "<br />";
		string tr(const string& content);
		string td(const string& content);
		string td2(const string& content);
		string div(const string& rawContent, const string& className = "");
		string linkToHiddenContent(const string& rawContent, const string& text = ShowMore);

		string error(const string& text);
		string intToYesNo(int i);
		string boolToYesNo(BOOL b);
};

