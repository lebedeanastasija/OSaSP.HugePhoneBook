// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <CommCtrl.h>
#include <Uxtheme.h>
#include <Commdlg.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <stdio.h>


// TODO: reference additional headers your program requires here
#include "..\PhoneDB\PhoneDB.h"
#include "ListViewFunctions.h"



typedef const TCHAR *(*OPENDATABASEROUTINE)(const TCHAR *filename, int *cRecordsPtr);
/*/*/
typedef const TCHAR *(*LOADPAGEDATABASEROUTINE)(int *cRecordsPtr);
/*/*/
typedef BOOL(*CLOSEDATABASEROUTINE)(void);
typedef int(*SEARCHINDATABASEROUTINE)(PHONE_DB_RECORD *foundRecordPtr,
	PHONE_DB_RECORD *templateRecordPtr,
	int cEqualFieldsNeed);
typedef void(*FILLDATABASERECORD)(
	const PHONE_DB_RECORD *recordPtr,
	const TCHAR *phone,
	const TCHAR *lastname,
	const TCHAR *name,
	const TCHAR *patronym,
	const TCHAR *street,
	const TCHAR *house,
	const TCHAR *housing,
	const TCHAR *apartment);