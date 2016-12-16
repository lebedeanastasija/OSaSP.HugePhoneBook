#pragma once

#include <Windows.h>

int     AddColumnToListView(HWND hList, TCHAR* str, int subitem, int width);
int     AddItemToListViewW(HWND hList, LPSTR str, int nRow, int nCol);
