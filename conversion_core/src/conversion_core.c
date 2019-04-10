/*
 * conversion_core.c
 *
 *  Created on: Jul 18, 2018
 *      Author: bhart
 */

#include "stdafx.h"
#include "conversion_core.h"

int StringToLong(const char* pszInput, long* plOutput) {
	char*	pszTail;
	int 	nBase = 10; /* default to reading digits and interpreting as base 10 */
	long 	lResult = 0; /* variable holding return */

	/* reset errno to zero before call */
	errno = 0;

	/* call to strtol assigning return to number */
	lResult = strtol(pszInput, (char**)&pszTail, nBase);

	/* test return to number and errno values */
	if (strcmp(pszInput, pszTail) == 0) {
		//free(pszTail);
		return INVALID_CHARS;
	}
	else if (errno == ERANGE && lResult == LONG_MIN) {
		//free(pszTail);
		return BUFFER_UNDERFLOW;
	}
	else if (errno == ERANGE && lResult == LONG_MAX) {
		//free(pszTail);
		return BUFFER_OVERFLOW;
	}
	else if (errno == EINVAL) {/* not all c99 implementations; gcc ok */
		//free(pszTail);
		return INVALID_BASE;
	}
	else if (errno != 0 && lResult == 0) {
		//free(pszTail);
		return UNSPECIFIED_ERROR;
	}
	else if (errno == 0 && pszTail[0] == '\0') {
		/* result is valid and represents all characters read */
		*plOutput = lResult;
		//free(pszTail);

		return EXACTLY_CORRECT;
	} else if (errno == 0 && pszInput && strlen(pszTail) > 0) {
		/* result is valid but the string contained non-digit characters too */
		*plOutput = lResult;
		//free(pszTail);
		return OK;
	}

	//free(pszTail);

	return UNSPECIFIED_ERROR;
}
