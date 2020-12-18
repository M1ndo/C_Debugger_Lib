/*
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 */

#ifndef STRINGPTR_H_
#define STRINGPTR_H_

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "stringptr_struct.h"

#define SPDECLAREC(X, Y) stringptr X##_storage, *X = stringptr_fromchar(Y, &X##_storage)

#define stringptr_grow(a, b) (stringptr*) realloc(a, b)
#define stringptr_free(a) do { if(a) free(a); (a) = NULL; } while(0)

#define stringptr_getsize(a) ((a)->size)
#define stringptr_get(a) ((a)->ptr)
#define stringptr_empty(a) ((a)->size == 0)

#define stringptr_getsize_s(a) ((a) ? (a)->size : 0)
#define stringptr_get_s(a) ((a) ? (a)->ptr : NULL)
#define stringptr_empty_s(a) (!(a) || ((a)->size == 0))


#ifndef DISABLE_STRINGPTR_SHORT_MACROS
#define EQ(X, Y) stringptr_eq(X, Y) 
#define SPL(X) SPLITERAL(X)
#define SDUP(X) stringptr_copy(X)
#define CHOMP(X) stringptr_chomp(X)
#endif

uint32_t stringptr_hash(stringptr* str);
int stringptr_here(stringptr* haystack, size_t bufpos, stringptr* needle);
int stringptr_contains(stringptr* haystack, stringptr* needle);
stringptr* stringptr_new(size_t size);
stringptr* stringptr_concat(stringptr* self, ...);
stringptr* stringptr_copy(stringptr* s);
int stringptr_eq(stringptr* a, stringptr* b);

/* shifts the start of a stringptr by count characters */
int stringptr_shiftright(stringptr* s, size_t count);

/* shifts(cuts) the tail of a stringptr by count characters */
int stringptr_shiftleft(stringptr* s, size_t count);

size_t stringptr_chomp(stringptr* s);
char* stringptr_strdup(stringptr* s);
char* stringptr_rchr(stringptr* haystack, int needle);
stringptr* stringptr_format(char* fmt, ...);
stringptr* stringptr_fromfile(char* filename);
size_t stringptr_tofile(char* filename, stringptr* buffer);
stringptr* stringptr_fromchar(char* s, stringptr* out);
stringptr read_stdin_line(char* buf, size_t bufsize, int chomp);

#ifdef __cplusplus
}
#endif

#pragma RcB2 DEP "../src/stringptr/stringptr_*.c"
#pragma RcB2 DEP "../src/stringptr/read_stdin_line.c"

#endif
