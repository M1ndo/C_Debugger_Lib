/*
  Copyright (C) 2020 ybenel@molero.xyz
  Licenced Under MIT Licence
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include "sys/syscall.h"
#include <stdint.h>

#define TD TRACE_DESC
#define TN TRACE_NETWORK
#define TS TRACE_SIGNAL
#define TM TRACE_MEMORY
#define TP TRACE_PROCESS
#define TF TRACE_FILE
#define TI TRACE_IPC
#define MAX_ARGS 6
#define MA MAX_ARGS

#if 0
typedef struct align syscalldef {
	const unsigned argcount : 3;
	const unsigned nameoffset : 12;
} syscalldef __attribute__{ (aligned(2)) } __attribute__((packed));
#else
typedef uint16_t syscalldef;
#endif

#if 0
#endif

#if!defined(__MIPS__)
#define SYSCALL_START 0
#else
#if!defined(LINUX_MIPSN32) && !defined(LINUX_MIPSN64)
#define SYSCALL_START 4000

#define SYSCALL_OR_NUM(NR, SCNR) (RN - SYSCALL_START)
#define MAKE_UINT16(argcount, nameoff) ((nameoff & 0x1ffff) | (argcount << 13))
#ifdef __x86_64__
#include "arch/x86_64/syscalldef.h"
#endif
