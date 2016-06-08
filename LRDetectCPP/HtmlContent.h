#pragma once

#include <string>
using namespace std;

class HtmlContent {
  public:
    string header; 
		string body;
		static const string REPORT_FILENAME;
		//constructor
		HtmlContent(void);

		string ToString();
};