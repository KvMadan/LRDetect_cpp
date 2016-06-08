#include "Html.h"
using namespace std;

namespace Html 
{

	string b(const string& content)
	{
		return "<b>" + content + "</b>";
	}

	string h1(const string& content)
	{
		return "<h1>" + content + "</h1>";
	}

	string h2(const string& content)
	{
		return "<h2>" + content + "</h2>";
	}
	
	string table_open(string id, string class_name)
	{
		return "\n\t<table id=\"" + id + "\" class=\"" + class_name + "\">";
	}

	string tr(const string& content)
	{
		return "\n\t\t<tr>" + content + "\n\t\t</tr>";
	}

	string td(const string& content)
	{
		return "\n\t\t\t<td>" + content + "</td>";
	}
	
	string td2(const string& content)
	{
		return "\n\t\t\t<td colspan=\"2\" class=\"colspan\">" + content + "</td>";
	}

	string div(const string& rawContent, const string& className)
	{
		return "\n\t\t\t<div class=\"" + className + "\">" + rawContent + "</div>";
	}

	string linkToHiddenContent(const string& rawContent, const string& text)
  {
    //string divId = Guid.NewGuid().ToString().Substring(0, 8);
		auto a = "<a href=\"#\" class=\"ShowMore\" title=\"Click here to show/hide additional content\">" + text + "</a>";

    return a + div(rawContent, "dontShow");
  }

	string error(const string& text)
	{
		return "<span class=\"error\">" + text + "</span>";
	}

	string errorMsg(long error)
	{
		return "<span class=\"error\">Unable to detect! Error " + std::to_string(error) + "</span>";
	}


	string intToYesNo(int i)
	{
		switch (i)
		{
			case 0 : return NO;
			case 1 : return YES;
			default: return Html::error("Unknown input value");
		}
	}

	string boolToYesNo(BOOL b)
	{
		return b ? YES : NO;
	}

}