#include "stdafx.h"
#include "LoadDatabase.h"

OPENDATABASEROUTINE     openDatabaseRoutine;
LOADPAGEDATABASEROUTINE     loadNextPageDatabaseRoutine;
LOADPAGEDATABASEROUTINE     loadPrevPageDatabaseRoutine;
CLOSEDATABASEROUTINE    closeDatabaseRoutine;
SEARCHINDATABASEROUTINE searchInDatabaseRoutine;
FILLDATABASERECORD      fillDatabaseRoutine;

extern HINSTANCE        hInst;
extern HWND             g_hMainWindow;
HMODULE                 g_hDatabaseLibrary;
const TCHAR            *g_DatabasePtr = nullptr;
int                     g_cRecordsInBase = 0;

BOOL LoadDatabaseDLL(void)
{
	auto hModule = LoadLibrary(TEXT("PhoneDB.dll"));
	DWORD error = 0;

	if (hModule == nullptr) {
		error = GetLastError();
	}
	else {
		openDatabaseRoutine = OPENDATABASEROUTINE(GetProcAddress(hModule, "OpenDatabase"));
		closeDatabaseRoutine = CLOSEDATABASEROUTINE(GetProcAddress(hModule, "CloseDatabase"));
		loadNextPageDatabaseRoutine = LOADPAGEDATABASEROUTINE(GetProcAddress(hModule, "LoadNextPage"));
		loadPrevPageDatabaseRoutine = LOADPAGEDATABASEROUTINE(GetProcAddress(hModule, "LoadPrevPage"));
		searchInDatabaseRoutine = SEARCHINDATABASEROUTINE(GetProcAddress(hModule, "SearchInDatabase"));
		fillDatabaseRoutine = FILLDATABASERECORD(GetProcAddress(hModule, "FillDatabaseRecord"));

		if (openDatabaseRoutine == nullptr
			|| closeDatabaseRoutine == nullptr
			|| searchInDatabaseRoutine == nullptr
			|| fillDatabaseRoutine == nullptr) {
			error = GetLastError();
		}
	}

	if (error) {
		DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
		HLOCAL hlocal = nullptr;

		FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_ALLOCATE_BUFFER,
			nullptr, GetLastError(), systemLocale,
			PTSTR(&hlocal), 0, nullptr);

		MessageBox(nullptr, PTCHAR(hlocal), nullptr, MB_OK);
		LocalFree(hlocal);
		return FALSE;
	}

	return TRUE;
}

BOOL GetFilename(TCHAR *szFile)
{
	OPENFILENAME ofn;

	memset(&ofn, 0, sizeof ofn);
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hInstance = hInst;
	ofn.hwndOwner = g_hMainWindow;
	ofn.lpstrFilter = nullptr;
	ofn.lpstrCustomFilter = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH * sizeof *szFile;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	ofn.lpstrTitle = nullptr;

	return GetOpenFileName(&ofn);
}
