#ifndef TIMELIB_H
#define TIMELIB_H

#undef _GNU_SOURCE
#define _GNU_SOURCE
#undef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#undef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#include <stddef.h>
#include <time.h>
#include <sys/time.h>

int msleep(long millisecs);
int microsleep(long microsecs);
size_t timestamp(char* buffer, size_t bufsize);
long mspassed(struct timeval* t);
void gettimestamp(struct timeval* t);
/* return actual system time in usec granularity */
long long getutime(void);
/* get difference between 2 struct timespec in nanoseconds */
long long timespec_diff(struct timespec *a, struct timespec *b);

#pragma RcB2 DEP "../src/timelib/*.c"

#endif
