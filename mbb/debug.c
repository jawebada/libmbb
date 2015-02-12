/* * Copyright (C) 2015 Jan Weil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <time.h>
#include <stdio.h>
#include <stdarg.h>

int mdbg_printf(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	return vfprintf(stderr, format, ap);
}

int mdbg_timestamp(char *out, int size)
{
	time_t current_time;
	struct tm *bdtime;

	if (size < 24)
		return -1;

	if (time(&current_time) == -1)
		return -1;

	bdtime = localtime(&current_time);
	if (bdtime == NULL)
		return -1;

	if (strftime(out, size, "%Y-%m-%dT%H:%M:%S.000", bdtime) != 23)
		return -1;

	out[23] = '\0';

	return 0;
}
