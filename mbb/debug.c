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
