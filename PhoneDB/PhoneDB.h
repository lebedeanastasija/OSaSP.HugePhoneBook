#pragma once

#define PHONEDB_API extern "C" __declspec(dllexport)

#define PHONE_DB_PHONE_LEN      8
#define PHONE_DB_LASTNAME_LEN   20
#define PHONE_DB_NAME_LEN       20
#define PHONE_DB_PATRONYM_LEN   20
#define PHONE_DB_STREET_LEN     30
#define PHONE_DB_HOUSE_LEN      5
#define PHONE_DB_HOUSING_LEN    5
#define PHONE_DB_APARTMENT_LEN  5

#define PHONE_DB_MAX_LEN        30

typedef struct _PHONE_DB_RECORD {
	TCHAR phone[PHONE_DB_PHONE_LEN];
	TCHAR lastname[PHONE_DB_LASTNAME_LEN];
	TCHAR name[PHONE_DB_NAME_LEN];
	TCHAR patronym[PHONE_DB_PATRONYM_LEN];
	TCHAR street[PHONE_DB_STREET_LEN];
	TCHAR house[PHONE_DB_HOUSE_LEN];
	TCHAR housing[PHONE_DB_HOUSING_LEN];
	TCHAR apartment[PHONE_DB_APARTMENT_LEN];
} PHONE_DB_RECORD;

#define PHONE_DB_CFIELDS 8

#include <Windows.h>

PHONEDB_API const TCHAR*
OpenDatabase(const TCHAR *filename, int *cRecordsPtr);

PHONEDB_API BOOL
CloseDatabase(void);

PHONEDB_API const TCHAR*
LoadNextPage(int *cRecordsPtr);

PHONEDB_API const TCHAR*
LoadPrevPage(int *cRecordsPtr);

PHONEDB_API BOOL
SearchInDatabase(PHONE_DB_RECORD *foundRecordPtr,
	const PHONE_DB_RECORD *templateRecordPtr,
	int cEqualFieldsNeed);

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
	const TCHAR *apartment);
