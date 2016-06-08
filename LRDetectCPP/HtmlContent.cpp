#include "HtmlContent.h"
#include "Html.h"
using namespace std;


const string HtmlContent::REPORT_FILENAME = "LRDetect_Report.html";

HtmlContent::HtmlContent(void)
{
	body = "";
}

string HtmlContent::ToString()
{
	return body;
}
