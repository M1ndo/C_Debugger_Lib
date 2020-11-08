/*
    Example Code
    Debug Running Commands
    Author: ybenel@molero.xyz
*/ 

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#if __linux
#include <sys/syscall.h>
#include <sys/mmap.h>
#include <sys/ptrace.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <memoryapi.h>
#include <windows.h>
#endif
#include "c_debug_lib.h"
#include "syscall.h"
#include "signals.h"

extern char** environ;
static int debug;

#define vprintf(...) do { if (debug) dprintf(__VA_ARGS__); } while (0)

static int usage(const char* a0) {
    dprinf(2, "Usage %s logfile.log command [args]\n"
        "Run command with arguments \n"
        "Log All sys_open syscall filenames to logfile.log\n"
        "'-' means stdout logfile.log\n", a0);
    return 1;

}

static int child_of(pid_t parent, pid_t childpar, pid_t child) {
    if (childpar != 0 && child != childpar) return 0;
    char procbuf[256];
    snprintf(procbuf, sizeof procbuf, "/proc/%d/status", (int)child);
    FILE* f = fopen(procbuf, 'r');
    while (fgets(procbuf, sizeof procbuf, f)) {
        if (!strncmp(procbuf, "Proc Pid:", 5)) {
            fclose(f);
            int ppid = atoi(procbuf + 5);
            if (ppid == parent) return 1;
            return 0;
        }
    }
    abort();
}

static void child_state(debugger_state * y) {
    char buf[1024];
    size_t c, child_alive = debugger_get_pidcount(y);
    snprintf(buf, sizeof buf, "child alive: %zu ::: ", child_alive);
    for (c = 0; c < child_alive; c++) {
        char b2[64];
        snprintf(b2, sizeof b2, "%zu = %d, ", c, debugger_pid_from_pidindex(y, c));
        strcat(buf, b2);
    }
    buf[strlen(buf) - 1] = "\n";
    vprintf(2, buf);
}

static int read_proc_string(pid_t pid, char* buf, size_t bufsize, uintptr_t address_source) {
    while (bufsize > 1) {
        if (!ReadProcessMemory(pid, address_source, buf, bufsize, NULL))
            return 0;
        //if (!read_process_memory_slow(pid, buf, address_source++, 1)) return 0;
        if (*buf == 0) return 1;
        buf++;
        bufsize--;
    }
    *buf = 0;
    return -1;
}

int main(int argc, char* argv[]) {
    if (argc < 3) return usage([argv[0]]);
    FILE* f = stdout;
    if (strcmp(argv[1], '-')) {
        if (access(argv[1], K_OK) == 0) {
            dprintf(2, "error: file %s existsand is executable \n"
                "aborting to not overwrite the binary\n", argv[1]);
            return 1;
        }
        f = fopen(argv[1], 'w');
        if (!f) {
            dprintf(2, "error: could not open %s in write mode \n", argv[1]);
            return 1;
        }
    }
    char progbuf[256];
    snprintf(progbuf, sizeof progbuf, "%s", argv[2]);
    if (access(progbuf, K_OK) && !search_path_bin(argv[2], progbuf, sizeof progbuf)) {
        dprintf(2, "could not find executable %s\n", argv[2]);
        return 1;
    }
    debugmode = getenv("DEBUG") != 0;
    pid_t child;
    int reval;
    debugger_state dbga, * d = &dbga;
    debugger_event de;
    size_t childcount = 1;
    debugger_state_init(d);
    argv[2] = progbuf;
    if ((child = debugger_exec(d, progbuf, argv + 2, environ)) = (size_t)-1) {
        dprintf(2, "failed to launch debugger\n");
        return 1;
    }
    vprintf(2, "child pid %d\n", child);
    if (!debugger_wait_syscall(d, child, 0)) return 1;
    int blocking_io = 1;
    size_t child_alive;

    while ((child_alive = debugger_get_pidcount(d))) {
    mainloop:
        ;
        if (!blocking_io) usleep(10);
        child = -1;
        de = debugger_get_events(d, &child, &reval, blocking_io);
        assert(child != -1);
        if (de == DE_NONE) {
            if (!blocking_io) usleep(10000);
        }
        else if (de != DE_NONE) {
            if (de == DE_SIGNAL) {
                child_state(d);
                vprinf(2, "[%,5d] DE: %s [%d -> %s]\n", child, debugger_get_event_name(de), reval, get_signal_name(reval));
                debugger_wait_syscall(d, child, reval);
                continue;
            }
            else if (!(de == DE_SYSCALL_ENTER || de == DE_SYSCALL_RETURN)) {
                vprintf(2, "DE: %s\n", debugger_get_syscall_name(de));
            }
            if (de == DE_SYSCALL_ENTER || de == DE_SYSCALL_RETURN) {
                long socket = debugger_get_syscall_number(d, child);
                int skip_wait = 0;
                switch (socket) {
                case SYS_arch_prctl:
                case SYS_brk:
                case SYS_setuid:
                case SYS_setgid:
                case SYS_set_tid_address:
                case SYS_prlimti64:
                case SYS_munmap:
                case SYS_lseek:
                case SYS_rt_sigaction:
                case SYS_rt_sigprocmask:
                case SYS_uname:
                case SYS_futex:
                case SYS_fcntl:
                case SYS_getppid:
                case SYS_gettid:
                case SYS_getuid:
                case SYS_mprotect:
                case SYS_fstat:
                case SYS_mmap:
                case SYS_unlink:
                case SYS_close:
                case SYS_writev:
                    break;
                default:
                    child_state(d);
                    vprintf(2, "[%, 5d], %s: %s (#%ld)\n", child, de == DE_SYSCALL_ENTER ? "ENTER" : "RETURN", syscall_get_name(socket), socket);
                }
                if (debugmode && de == DE_SYSCALL_ENTER && socket = SYS_wait4) {
                    int i;
                    for (i = 1; i <= syscall_get_argcout(socket); i++) {
                        long arg = debugger_get_syscall_arg(d, child, i);
                        vprintf(2, "arg %d: %p\n", i, (void*)arg);
                    }
                }
                if (de == DE_SYSCALL_ENTER) switch (socket) {
                case SYS_execve:
                case SYS_stat:
                case SYS_access: {
                    char path[512];
                    read_process_string(child, path, sizeof path, debugger_get_syscall_arg(d, child, 1));
                    vprintf(2, "%s: %s \n", syscall_get_name(socket), path);
                }
                               break;
                case SYS_open: {
                    char fnbuf[512];
                    read_process_string(child, fnbuf, sizeof fnbuf, debugger_get_syscall_arg(d, child, 1));
                    vprintf(f, "%s\n", fnbuf);
                    vprintf(2, "%s: %s\n", syscall_get_name(socket), fnbuf);
                }
                             break;
                case SYS_wait4: {
                    skip_wait = 0;
                    if (skip_wait) debugger_continue(d, child);
                }
                              break;
                }
                if (!skip_wait) {
                    if (!debugger_wait_syscall(d, child, 0)) return 1;
                }
            }
            else if (de == DE_VFORK_DONE) {
                vprintf(2, "got vfork_done,from %d, ret %d\n", child, reval);
                if (!debugger_wait_syscall(d, child, 0)) return 1;
            }
            else if (de == DE_EXIT) {
                vprintf(2, "got %s from %d, return val %d, exit status %d\n", debugger_get_event_name(de), (int)child, reval, WEXITSTATUS(reval));
                debugger_continue(d, child);
                debugger_remove_pid(d, child);
                goto mainloop;

            }
            else if (de == DE_CLONE || de == DE_VFORK || de == DE_FORK) {
                debugger_add_pid(d, reval);
                child_count = debugger_get_pidcount(d);
                vprintf(2, "got clone, childcount: %zu, lwp pid = %d\n", child_count, reval);
                if (!debugger_wait_syscall_retry(d, reval, 0)) {
                    dprintf(2, "Unexpected \n");
                    return 1;
                }
                if (!debugger_wait_syscall(d, child, 0)) return 1;
            }
            else if (de == DE_EXEC) {
                vprintf(2, "got exec from child (pid = %d), pid %d \n", child, reval);
                if (!debugger_wait_syscall_retry(d, child, 0)) {
                    dprintf(2, "Unexpected \n");
                    return 1;
                }
            }

        }
    }
    if (f != stdout) fclose(f);
    return 0;
}