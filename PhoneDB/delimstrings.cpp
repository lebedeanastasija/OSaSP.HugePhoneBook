#include "stdafx.h"
#include "delimstrings.h"

size_t DelimStrCpy(TCHAR *dst,
	const TCHAR *src,
	const TCHAR *delim,
	const TCHAR *limit,
	size_t cchDstSize)
{
	size_t cchCopied = 0;

	while (cchCopied < cchDstSize && src < limit) {
		*dst = *src;
		cchCopied++;

		if (strchr(delim, *src) != nullptr)
			break;

		dst++; src++;
	}

	return cchCopied;
}
