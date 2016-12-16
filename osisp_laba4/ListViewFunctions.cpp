#include "stdafx.h"
#include "ListViewFunctions.h"
#include <winnls.h>

extern HWND     g_hMainWindow;

int AddColumnToListView(HWND hList, TCHAR* str, int subitem, int width)
{
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = subitem;
	lvc.pszText = str;
	lvc.cx = width;
	lvc.fmt = LVCFMT_LEFT;

	return ListView_InsertColumn(hList, subitem, &lvc);
}

int AddItemToListViewW(HWND hList, LPSTR str, int nRow, int nCol)
{
	LVITEM lvItem;
	lvItem.mask = LVIF_IMAGE;
	lvItem.pszText = str;
	lvItem.iItem = nRow;
	lvItem.iSubItem = 0;
	if (nCol == 0) {
		ListView_InsertItem(hList, &lvItem);
	}
	ListView_SetItemText(hList, nRow, nCol, str);

	return 0;
}
