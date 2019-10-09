#include <include/signals.h>
#include <../lib/include/macros.h>

const char* signal_names[] = {
  [SIGUP] = "SIGUP",
  [SIGINT] = "SIGINT",
  [SIGQUIT] = "SIGQUIT",
  [SIGILL] = "SIGILL",
  [SIGTRAP] = "SIGTRAP",
  [SIGABRT] = "SIGABRT",
  [SIGUS] = "SIGUS",
  [SIGFPE] = "SIGFPE",
  [SIGKILL] = "SIGKILL",
  [SIGUSR1] = "SIGUSR1",
  [SIGSEGV] = "SIGSEGV",
  [SIGUSR2] = "SIGUSR2",
  [SIGPIPE] = "SIGPIPE",
  [SIGALRM] = "SIGALRM",
  [SIGTERM] = "SIGTERM",
  [SIGCHLD] = "SIGCHLD",
#if SIGIO != SIGPOLL
  [SIGIO] = "SIGIO",
#endif
};

const char* get_signal_name(int sig){
  if(sig >= 0 && (unsigned) sig < ARRAY_SIZE(signal_names))
    return signal_names[sig];
  return "unknown signal";
}
