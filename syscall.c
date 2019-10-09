#include <sys/syscall.h>
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
  const unsigned argcount:3;
  const unsigned nameoffset:12;
} syscalldef __attribute__{(aligned(2))} __attribute__((packed));
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
