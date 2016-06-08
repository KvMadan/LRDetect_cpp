#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
	/// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c
#define COLLECT_BUTTON 101
#define SEND_BUTTON 102

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include "Collect.h"
#include "CmdArgs.h"

// The main window class name.
static LPCTSTR szWindowClass = "LRDetect";

// The string that appears in the application's title bar.
static LPCTSTR szTitle = "LRDetect Tool";

HINSTANCE hInst; //instance handle
HWND hwndCollectButton; //collect button handle
HWND hWnd;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex;

  wcex.cbSize					= sizeof(WNDCLASSEX);
  wcex.style          = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc    = WndProc;
  wcex.cbClsExtra     = 0;
  wcex.cbWndExtra     = 0;
  wcex.hInstance      = hInstance;
  wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
  wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName   = NULL;
  wcex.lpszClassName  = szWindowClass;
  wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
  {
      MessageBox(NULL, "Call to RegisterClassEx failed!", "LRDetect Tool", NULL);
      return 1;
  }

	hInst = hInstance; // Store instance handle in our global variable
	
	//Parse CMD arguments
	CmdArgs::GetArgs();

  // The parameters to CreateWindow explained:
  // szWindowClass: the name of the application
  // szTitle: the text that appears in the title bar
  // WS_OVERLAPPEDWINDOW: the type of window to create
  // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
  // 500, 100: initial size (width, length)
  // NULL: the parent of this window
  // NULL: this application dows not have a menu bar
  // hInstance: the first parameter from WinMain
  // NULL: not used in this application
		hWnd = CreateWindow(
      szWindowClass,
      szTitle,
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      500, 400,
      NULL,
      NULL,
      hInstance,
      NULL
  );

  if (!hWnd)
  {
      MessageBox(NULL, "Window class creation failed!", "LRDetect Failed", MB_ICONERROR);
      return 1;
  }


  // hWnd: the value returned from CreateWindow
  // nCmdShow: the fourth parameter from WinMain
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

	//If started from CMD
	if (CmdArgs::collect)
		CollectData(hWnd, hwndCollectButton);

  // Main message loop:	
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
  }

  return (int) msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = "Click start to begin!";
		int i = 0;


    switch (message)
    {
			case WM_PAINT:
					hdc = BeginPaint(hWnd, &ps);

					// Here your application is laid out.
					// For this introduction, we just print out "Hello, World!"
					// in the top left corner.
					TextOut(hdc, 10, 50, greeting, _tcslen(greeting));
					// End application-specific layout section.

					EndPaint(hWnd, &ps);
					break;
			case WM_DESTROY:
					PostQuitMessage(0);
					break;
			case WM_CREATE:
				{
				//Create the "Collect" button
				hwndCollectButton = CreateWindowEx(NULL, 
					"BUTTON",  // Predefined class; Unicode assumed 
					"Collect data",      // Button text 
					WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
					10,         // x position 
					10,         // y position 
					100,        // Button width
					30,        // Button height
					hWnd,     // Parent window
					(HMENU)COLLECT_BUTTON,
					(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
					NULL);      // Pointer not needed.
	
				//Create the "Send" button
				/*HWND hwndSendButton = CreateWindow( 
					"BUTTON",  // Predefined class; Unicode assumed 
					"Send data",      // Button text 
					WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,  // Styles 
					120,         // x position 
					10,         // y position 
					100,        // Button width
					30,        // Button height
					hWnd,     // Parent window
					NULL,       // No menu.
					(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), 
					NULL);      // Pointer not needed.
			*/
				break;
				}
			case WM_COMMAND : 
					switch (LOWORD(wParam))
					{
						case COLLECT_BUTTON:
							CollectData(hWnd, hwndCollectButton);
						break;
						/*case SEND_BUTTON :
							::MessageBeep(MB_ICONERROR);
						break;*/
					}

				break;
			default:
					return DefWindowProc(hWnd, message, wParam, lParam);
					break;
    }

    return 0;
}

