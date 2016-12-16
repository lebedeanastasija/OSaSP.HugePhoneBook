#pragma once

#include "stdafx.h"

size_t DelimStrCpy(TCHAR *dst,
	const TCHAR *src,
	const TCHAR *delim,
	const TCHAR *limit,
	size_t cchDstSize);
