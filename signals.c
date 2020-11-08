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
#include "include/signals.h"
#include "../lib/include/macros.h"

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

const char* get_signal_name(int sig) {
    if (sig >= 0 && (unsigned)sig < ARRAY_SIZE(signal_names))
        return signal_names[sig];
    return "unknown signal";
}
