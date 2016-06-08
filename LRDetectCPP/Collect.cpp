#include <windows.h>
#include <fstream>
#include "resource.h"
#include <time.h>
#include <ctime>
#include "Html.h"
#include "HtmlContent.h"
#include "OsInfo.h"
#include "CmdArgs.h"
#include "HardwareInfo.h"
#include "ProductsInfo.h"

using namespace std;

#pragma warning(disable : 4996)
//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
LPSTR GetLastErrorAsLPSTR()
{
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return "No error message has been recorded";

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		return messageBuffer;
    //std::string message(messageBuffer, size);

    //Free the buffer.
    //LocalFree(messageBuffer);

    //return message;
}



string FillTemplate(string content)
{
	//Load HTML Template
	HRSRC hRes = FindResource( 0, MAKEINTRESOURCE(IDR_HTML2), MAKEINTRESOURCE(HTML));
	HGLOBAL res_handle  = LoadResource( 0, hRes );
	LPVOID data = LockResource( res_handle );
	//char *charData = (char*)data;
	FreeResource(res_handle);
	string html_template = (char*)data;

	size_t found = html_template.find("{DATE}", 107150);
	time_t now = time(0);
	tm* localtm = localtime(&now);
	html_template.replace(found, 6, asctime(localtm));

	//string user = domain + "\\" + username;
	found = html_template.find("{USER}");
	html_template.replace(found, 6, Html::b(OsInfo::computerName() + "\\" + getenv("USERNAME")));

	found = html_template.find("{BODY}");
	html_template.replace(found, 6, content);

  /*DWORD size = SizeofResource(NULL, hRes);
  char* resText = (char*)LockResource(res_handle);
  char* text = (char*)malloc(size + 1);
  memcpy(text, resText, size);
  text[size] = 0;*/
  

	return html_template;
}

void CollectData(HWND hWnd, HWND btnHandle)
{
	//Disable the collect button
	::EnableWindow(btnHandle, false);

	//Get the current time
	clock_t begin = clock();
	//1. Create a new instance of the HTML report
	HtmlContent r;
	//::MessageBox(hWnd, r.ToString().c_str(), NULL, MB_OK);

	//Collect Data
	//Collect OS info
	OsInfo::collectData(r.body);
	//Sleep(100);
	HardwareInfo::collectData(r.body);

	ProductInfo::collectData(r.body);


	//Get the current time
  clock_t end = clock();
	int time_consumed = end - begin;
	r.body.append(Html::br + "Report generation time: " + to_string(time_consumed) + "ms");
	r.body.append(Html::br + "LR Detect version: 0.1");
	
	//Save data in a HTML file
		//Load HTML Template
	string html_template = FillTemplate(r.ToString());

	ofstream  out("LRDetect_Report.html");
	if (!out.bad())
	{
		// Dump the contents of the file.
		out << html_template;
		out.close();
	}

	//Open the HTML file
	if (!CmdArgs::hideReport)
		ShellExecute(NULL, "open", r.REPORT_FILENAME.c_str(), NULL, NULL, SW_SHOWNORMAL);

	//Enable the collect button
	::EnableWindow(btnHandle, true);
}
