#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include "stdafx.h"
#include "PhoneDB.h"
#include "delimstrings.h"
#include <Windows.h>

#define PHONE_DB_MAP_OBJECT_NAME TEXT("Osisp_lab4, PhoneDB. Tsar Art & Turuta Sa 851002")
#define ULL unsigned long long


static HANDLE           g_hFileMapping = nullptr;
static const TCHAR       *g_DbBeginPtr = nullptr;
static const TCHAR       *g_DbLimitPtr = nullptr;
static BOOL             g_DatabaseOpened = FALSE;
static int              g_CountOfRecords = 0;

static ULL              fileSize = 0;
static ULL              fileOffset = 0;
static DWORD            pageSize = 0;
static LPVOID			viewMapping = nullptr;
/*/*/

BOOL APIENTRY
DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_DETACH && g_DatabaseOpened) {
		CloseDatabase();
	}

	return TRUE;
}

const char* Load(int *cRecordsPtr)
{
	auto viewMappingLength = int(pageSize);

	if (fileOffset + viewMappingLength > fileSize) {
		viewMappingLength = int(fileSize - fileOffset);
	}

	if (viewMappingLength < 0) {
		return nullptr;
	}

	auto high = DWORD(fileOffset >> 32 & 0xFFFFFFFFul);
	auto low = DWORD(fileOffset & 0xFFFFFFFFul);

	if (viewMapping) {
		UnmapViewOfFile(viewMapping);
	}

	viewMapping = PCHAR(MapViewOfFile(
		g_hFileMapping,
		FILE_MAP_READ,
		high, low, viewMappingLength));

	g_DbBeginPtr = PCHAR(viewMapping);
	g_DbLimitPtr = PCHAR(PCHAR(g_DbBeginPtr) + viewMappingLength);

	while (g_DbLimitPtr[-1] != '\n') {
		g_DbLimitPtr--;
	}

	g_CountOfRecords = 0;

	for (auto c = g_DbBeginPtr; c < g_DbLimitPtr; c++) {
		if (*c == '\n') {
			g_CountOfRecords++;
		}
	}

	if (cRecordsPtr) {
		*cRecordsPtr = g_CountOfRecords;
	}

	g_DatabaseOpened = viewMapping != nullptr;
	return PCHAR(viewMapping);
}

PHONEDB_API const TCHAR*
OpenDatabase(const TCHAR *filename, int *cRecordsPtr)

#define RETURN(ret)\
            g_DatabaseOpened = (ret != NULL);\
            g_DbBeginPtr = ret;\
            return ret
{
	if (g_DatabaseOpened) {
		return nullptr;
	}

auto hFile = CreateFile(
	filename,
	GENERIC_READ,
	FILE_SHARE_READ,
	nullptr,
	OPEN_EXISTING,
	FILE_ATTRIBUTE_NORMAL,
	nullptr);

if (hFile == INVALID_HANDLE_VALUE) {
	RETURN(NULL);
}

LARGE_INTEGER fs;
if (GetFileSizeEx(hFile, &fs) == FALSE) {
	CloseHandle(hFile);
	RETURN(NULL);
}

g_hFileMapping = CreateFileMapping(
	hFile,
	nullptr,
	PAGE_READONLY,
	fs.HighPart, fs.LowPart,
	PHONE_DB_MAP_OBJECT_NAME);

	fileSize = ULL(fs.QuadPart);

	SYSTEM_INFO sysinfo = { 0 };
	GetSystemInfo(&sysinfo);
	pageSize = sysinfo.dwAllocationGranularity;

	fileOffset = 0;

	auto retval = Load(cRecordsPtr);

	RETURN(retval);
#undef RETURN
}

PHONEDB_API const TCHAR*
LoadNextPage(int *cRecordsPtr)
{
	if (!g_DatabaseOpened || fileOffset + pageSize > fileSize) {
		return nullptr;
	}

	fileOffset += pageSize;
	return Load(cRecordsPtr);
}

PHONEDB_API const TCHAR*
LoadPrevPage(int *cRecordsPtr)
{
	if (!g_DatabaseOpened || int(fileOffset - pageSize) < 0) {
		return nullptr;
	}

	fileOffset -= pageSize;
	return Load(cRecordsPtr);
}


PHONEDB_API BOOL
CloseDatabase(void)
{
	if (!g_DatabaseOpened || UnmapViewOfFile(g_DbBeginPtr) == FALSE) {
		return FALSE;
	}

	CloseHandle(g_hFileMapping);

	g_hFileMapping = nullptr;
	g_DbBeginPtr = nullptr;
	g_DbLimitPtr = nullptr;
	g_DatabaseOpened = FALSE;
	g_CountOfRecords = 0;

	return TRUE;
}

PHONEDB_API BOOL
SearchInDatabase(PHONE_DB_RECORD *foundRecordPtr,
	const PHONE_DB_RECORD *templateRecordPtr,
	int cEqualFieldsNeed)

#define MAKE_COPY(field_name)\
    cchCpied = DelimStrCpy(foundRecordPtr->field_name, searchPos, ":\n\r",\
            g_DbLimitPtr, _countof(foundRecordPtr->field_name));\
        \
        foundRecordPtr->field_name[cchCpied-1] = '\0';\
        if (cchCpied == 0) {\
            return FALSE;\
        }\
        \
        searchPos += cchCpied;\
        if (searchPos[-1] == '\n') {\
            goto end_of_record_reached;\
        }
{
	static const TCHAR *searchEndPos = nullptr;
	int records;

	if (foundRecordPtr == nullptr) {
		searchEndPos = g_DbBeginPtr;
		return TRUE;
	}

	if (searchEndPos == nullptr) {
		searchEndPos = g_DbBeginPtr;
	}

	foundRecordPtr->phone[0] = '\0';
	foundRecordPtr->lastname[0] = '\0';
	foundRecordPtr->name[0] = '\0';
	foundRecordPtr->patronym[0] = '\0';
	foundRecordPtr->street[0] = '\0';
	foundRecordPtr->house[0] = '\0';
	foundRecordPtr->housing[0] = '\0';
	foundRecordPtr->apartment[0] = '\0';

	auto searchPos = searchEndPos;
	auto retval = FALSE;
	while (searchPos < g_DbLimitPtr) {
		int cchCpied;

		MAKE_COPY(phone);
		MAKE_COPY(lastname);
		MAKE_COPY(name);
		MAKE_COPY(patronym);
		MAKE_COPY(street);
		MAKE_COPY(house);
		MAKE_COPY(housing);
		MAKE_COPY(apartment);

	end_of_record_reached:

		if (!strlen(foundRecordPtr->apartment)) continue;

		auto cEqualFields = 0;

#define MAKE_COMPARE(field_name)\
        if (templateRecordPtr->field_name[0] != '\0') {\
            cEqualFields += !lstrcmpiA(foundRecordPtr->field_name, templateRecordPtr->field_name);\
        }

		MAKE_COMPARE(phone);
		MAKE_COMPARE(lastname);
		MAKE_COMPARE(name);
		MAKE_COMPARE(street);
		MAKE_COMPARE(patronym);
		MAKE_COMPARE(house);
		MAKE_COMPARE(housing);
		MAKE_COMPARE(apartment);

#undef MAKE_COMPARE

		if (cEqualFields >= cEqualFieldsNeed) {
			retval = TRUE;
			break;
		}
	}

	searchEndPos = searchPos;

	if (!retval && cEqualFieldsNeed != 0 && LoadNextPage(&records)) {
		SearchInDatabase(nullptr, nullptr, 0);
		return SearchInDatabase(foundRecordPtr, templateRecordPtr, cEqualFieldsNeed);
	}

	return retval;
#undef MAKE_COPY
}

inline void FillRecordField(TCHAR *dest, const TCHAR *src, int maxFieldCch)
{
	if (src != nullptr) {
		strncpy(dest, src, maxFieldCch);
		dest[maxFieldCch - 1] = '\0';
	}
	else {
		dest[0] = '\0';
	}
}

PHONEDB_API void
FillDatabaseRecord(
	PHONE_DB_RECORD *recordPtr,
	const TCHAR *phone,
	const TCHAR *lastname,
	const TCHAR *name,
	const TCHAR *patronym,
	const TCHAR *street,
	const TCHAR *house,
	const TCHAR *housing,
	const TCHAR *apartment)

#define FILL_FIELD(field_name)\
    FillRecordField(recordPtr->field_name,\
                    field_name,\
                    _countof(recordPtr->field_name))
{
	FILL_FIELD(phone);
	FILL_FIELD(lastname);
	FILL_FIELD(name);
	FILL_FIELD(patronym);
	FILL_FIELD(street);
	FILL_FIELD(house);
	FILL_FIELD(housing);
	FILL_FIELD(apartment);
#undef FILL_FIELD
}
