/*
 * conversion_core.c
 *
 *  Created on: Jul 18, 2018
 *      Author: bhart
 */

#include "stdafx.h"
#include "conversion_core.h"

int StringToLong(const char* pszInput, long* plOutput) {
	char	*pszEndingPtr = NULL; /* pointer to addtional chars */
	int 	nBase = 10; /* default to reading digits and interpreting as base 10 */
	long 	lResult = 0; /* variable holding return */

	/* reset errno to zero before call */
	errno = 0;

	/* call to strtol assigning return to number */
	lResult = strtol(pszInput, &pszEndingPtr, nBase);

	/* test return to number and errno values */
	if (pszInput == pszEndingPtr)
		return INVALID_CHARS;
	else if (errno == ERANGE && lResult == LONG_MIN)
		return BUFFER_UNDERFLOW;
	else if (errno == ERANGE && lResult == LONG_MAX)
		return BUFFER_OVERFLOW;
	else if (errno == EINVAL) /* not all c99 implementations; gcc ok */
		return INVALID_BASE;
	else if (errno != 0 && lResult == 0)
		return UNSPECIFIED_ERROR;
	else if (errno == 0 && pszInput && !*pszEndingPtr) {
		/* result is valid and represents all characters read */
		*plOutput = lResult;
		return EXACTLY_CORRECT;
	} else if (errno == 0 && pszInput && *pszEndingPtr != 0) {
		/* result is valid but the string contained non-digit characters too */
		*plOutput = lResult;
		return OK;
	}

	return UNSPECIFIED_ERROR;
}
