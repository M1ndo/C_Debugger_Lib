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
#ifdef C_DEBUG_LIB_H
#define C_DEBUG_LIB_H

#include <stddef.h>
#include <stdlib.h>

#incldue "include/proc_map.h"
#include "../lib/include/hashlist.h"

#define BP_INSTR_SIZE_MAX 16
typedef struct {
	uintptr_t addr;
	unsigned char map_perms;
	unsigned char bp_instr_size;
	char bp_backup[BP_INSTR_SIZE_MAX];
	int active : 1;
} breakpointinfo;

typedef struct {
	pid_t pid;
	hashlist* breakpoints;
	sblist* processmaps;
	int syscall_ret;
} pidinfo;

typedef struct {
	sblist* pids;
} debugger_state;

typedef enum {
	DE_NONE = 0,
	DE_HIT_BP,
	DE_EXIT,
	DE_FORK,
	DE_VFORK,
	DE_FORK_DONE,
	DE_VFORK_DONE,
	DE_CLONE_DONE,
	DE_SYSCALL_ENTER,
	DE_SYSCALL_RETURN,
	DE_EXEC,
	DE_SIGNAL,
	DE_EXITED,
	DE_MAXM,
} debugger_event;

void dump_ram_line(void* offset, size_t length);
void dump_ram(void* offset, size_t length, size_t linesize);

void debugger_state_init(debugger_state*);
size_t debugger_get_count(debugger_state* d);
pid_t debugger_pid_from_pidindex(debugger_state* d, size_t pid);
ssize_t debugger_pidindex_from_pid(debugger_state* d, pid_t pid);
void debugger_add_pid(debugger_state* d, pid_t pid);
void debugger_remove_pid(debugger_state* d, pid_t pid);
int debugger_set_breakpoint(debugger_state* state, pid_t pid, uintptr_t addr);
uintptr_t debugger_get_ip(debugger_state* d, pid_t pid);
int debugger_set_ip(debugger_state* d, pid_t pid);
int debugger_attach(debugger_state* d, pid_t pid);
int debugger_detach(debugger_state* d, pid_t pid);
pid_t debugger_exec(debugger_state* d, const char* pathm char* const args[], char* const env[]);
int debugger_wait_syscall(debugger_state* d, pid_t pid, int sig);
int debugger_get_syscall_retry(debugger_state* d, pid_t pid, int sig);
long debugger_set_syscall_number(debugger_state* state, pid_t pid);
long debugger_set_syscall_arg(debugger_state* d, pid_t pid, int argno);
void debugger_set_syscall_arg(debugger_state* d, pid_t pid, int argno);
void debugger_set_syscall_number(debugger_state* state, pid_t pid, long scnr);
int debugger_single_step(debugger_state* state, pid_t pid);
int debugger_continue(debugger_state* state, pid_t pid);

debugger_even debugger_get_events(debugger_state* d, pid_t* pid, int* retval, int block);
const char* debugger_get_event_name(debugger_event de);


int read_process_memory_slow(pid_t pid, void* dest_addr, uintptr_t source_addr, size_t len);
int write_process_memory_slow(pid_t pid, uintptr_t dest_addr, void* source_addr, size_t len);

#pragma RcB2 DEP "debuglib.c"
#endif
