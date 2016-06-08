#pragma once
#include <string>

void CollectData(HWND hWnd, HWND btnHandle);
LPSTR GetLastErrorAsLPSTR();
std::string FillTemplate(std::string content);