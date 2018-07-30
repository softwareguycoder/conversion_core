/*
 * conversion_core.c
 *
 *  Created on: Jul 18, 2018
 *      Author: bhart
 */

#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#include "conversion_core.h"

int char_to_long(const char* input, long* output) {
	char *endptr = NULL; /* pointer to addtional chars */
	int base = 10; /* default to reading digits and interpreting as base 10 */
	long number = 0; /* variable holding return */

	/* reset errno to zero before call */
	errno = 0;

	/* call to strtol assigning return to number */
	number = strtol(input, &endptr, base);

	/* test return to number and errno values */
	if (input == endptr)
		return INVALID_CHARS;
	else if (errno == ERANGE && number == LONG_MIN)
		return BUFFER_UNDERFLOW;
	else if (errno == ERANGE && number == LONG_MAX)
		return BUFFER_OVERFLOW;
	else if (errno == EINVAL) /* not all c99 implementations; gcc ok */
		return INVALID_BASE;
	else if (errno != 0 && number == 0)
		return UNSPECIFIED_ERROR;
	else if (errno == 0 && input && !*endptr) {
		/* result is valid and represents all characters read */
		*output = number;
		return EXACTLY_CORRECT;
	} else if (errno == 0 && input && *endptr != 0) {
		/* result is valid but the string contained non-digit characters too */
		*output = number;
		return OK;
	}

	return UNSPECIFIED_ERROR;
}
