#include "stdafx.h"
#include "ButtonHandlers.h"
#include "LoadDatabase.h"
#include <Windows.h>
#include <cwchar>
#include <stdlib.h>


extern HWND             g_hListView, g_hMainWindow;

extern const TCHAR      *g_DatabasePtr;
extern int              g_cRecordsInBase;
extern HWND             g_hInputBox[8];

extern OPENDATABASEROUTINE      openDatabaseRoutine;
extern LOADPAGEDATABASEROUTINE  loadNextPageDatabaseRoutine;
extern LOADPAGEDATABASEROUTINE  loadPrevPageDatabaseRoutine;
extern CLOSEDATABASEROUTINE     closeDatabaseRoutine;
extern SEARCHINDATABASEROUTINE  searchInDatabaseRoutine;
extern FILLDATABASERECORD       fillDatabaseRoutine;

void Display(PHONE_DB_RECORD* recTemplate, int countOfNotEmptyFields, int recordLimit = 0);

void LoadDB(void)
{
	TCHAR szFile[MAX_PATH] = TEXT("\0");

	if (GetFilename(szFile)) {
		if (g_DatabasePtr) {
			closeDatabaseRoutine();
			g_DatabasePtr = nullptr;
		}
		if (!((g_DatabasePtr = openDatabaseRoutine(szFile, &g_cRecordsInBase)))) {
			MessageBox(
				g_hMainWindow,
				TEXT("Cannot Load DB!"),
				nullptr, MB_OK);
		}
		ShowDB();
	}
}

void LoadNextPageDB()
{
	if (!loadNextPageDatabaseRoutine(&g_cRecordsInBase)) {
		MessageBox(
			g_hMainWindow,
			TEXT("This is last page!"),
			nullptr, MB_OK);
	}
	else {
		ShowDB();
	}
}

void LoadPrevPageDB()
{
	if (!loadPrevPageDatabaseRoutine(&g_cRecordsInBase)) {
		MessageBox(
			g_hMainWindow,
			TEXT("This is first page!"),
			nullptr, MB_OK);
	}
	else {
		ShowDB();
	}
}

void ShowDB(void)
{
	PHONE_DB_RECORD recTemplate;

	if (g_DatabasePtr == nullptr) {
		MessageBox(g_hMainWindow, TEXT("Error: First you must download a DB"), nullptr, MB_OK);
		return;
	}

	Display(&recTemplate, 0);
}

void SearchDB(void)
{
	PHONE_DB_RECORD recTemplate;

	if (g_DatabasePtr == nullptr) {
		MessageBox(g_hMainWindow, TEXT("Error: First you must download a DB"), nullptr, MB_OK);
		return;
	}

	GetWindowTextA(g_hInputBox[0], (LPSTR)(recTemplate.phone), sizeof recTemplate.phone);
	GetWindowTextA(g_hInputBox[1], (LPSTR)(recTemplate.lastname), sizeof recTemplate.lastname);
	GetWindowTextA(g_hInputBox[2], (LPSTR)(recTemplate.name), sizeof recTemplate.name);
	GetWindowTextA(g_hInputBox[3], (LPSTR)(recTemplate.patronym), sizeof recTemplate.patronym);
	GetWindowTextA(g_hInputBox[4], (LPSTR)(recTemplate.street), sizeof recTemplate.street);
	GetWindowTextA(g_hInputBox[5], (LPSTR)(recTemplate.house), sizeof recTemplate.house);
	GetWindowTextA(g_hInputBox[6], (LPSTR)(recTemplate.housing), sizeof recTemplate.housing);
	GetWindowTextA(g_hInputBox[7], (LPSTR)(recTemplate.apartment), sizeof recTemplate.apartment);

	auto countOfNotEmtyFields = 0;

#define CHECK_IF_FIELD_NOT_EMPTY(field_name)\
    if (recTemplate.field_name[0] != '\0') countOfNotEmtyFields++;

	CHECK_IF_FIELD_NOT_EMPTY(phone);
	CHECK_IF_FIELD_NOT_EMPTY(lastname);
	CHECK_IF_FIELD_NOT_EMPTY(name);
	CHECK_IF_FIELD_NOT_EMPTY(street);
	CHECK_IF_FIELD_NOT_EMPTY(patronym);
	CHECK_IF_FIELD_NOT_EMPTY(house);
	CHECK_IF_FIELD_NOT_EMPTY(housing);
	CHECK_IF_FIELD_NOT_EMPTY(apartment);

#undef CHECK_IF_FIELD_NOT_EMPTY

	if (countOfNotEmtyFields > 0) {
		Display(&recTemplate, countOfNotEmtyFields, 1000);
	}
	else {
		MessageBox(g_hMainWindow, TEXT("Enter data to search!"), nullptr, MB_OK);
	}
}

void Display(PHONE_DB_RECORD* recTemplate, int countOfNotEmptyFields, int recordLimit)
{
	PHONE_DB_RECORD foundRec;
	auto recordsFounded = 0;
	ListView_DeleteAllItems(g_hListView);
	searchInDatabaseRoutine(nullptr, nullptr, 0);
	while (searchInDatabaseRoutine(&foundRec, recTemplate, countOfNotEmptyFields))
	{
		if (AddItemToListViewW(g_hListView, (LPSTR)(foundRec.phone), 0, 0)
			|| AddItemToListViewW(g_hListView, (LPSTR)(foundRec.lastname), 0, 1)
			|| AddItemToListViewW(g_hListView, (LPSTR)(foundRec.name), 0, 2)
			|| AddItemToListViewW(g_hListView, (LPSTR)(foundRec.patronym), 0, 3)
			|| AddItemToListViewW(g_hListView, (LPSTR)(foundRec.street), 0, 4)
			|| AddItemToListViewW(g_hListView, (LPSTR)(foundRec.house), 0, 5)
			|| AddItemToListViewW(g_hListView, (LPSTR)(foundRec.housing), 0, 6)
			|| AddItemToListViewW(g_hListView, (LPSTR)(foundRec.apartment), 0, 7))
		{
			DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
			HLOCAL hlocal = nullptr;

			FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_ALLOCATE_BUFFER,
				nullptr, GetLastError(), systemLocale,
				PTSTR(&hlocal), 0, nullptr);
			MessageBox(g_hMainWindow,
				TEXT("Something went wrong! Check your DB"),
				nullptr,
				MB_OK);

			MessageBox(nullptr, static_cast<TCHAR*>(hlocal), nullptr, MB_OK);
			LocalFree(hlocal);
			break;
		}

		recordsFounded++;
		if (recordsFounded == recordLimit) break;
	}
}