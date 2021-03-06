#pragma once

#include <signal.h>
#include <cstring>
namespace ex {
namespace os {
    using sig_handler_t = void(*)(int);
#if defined(_WIN32) || defined(__CYGWIN__)
inline const char *sys_signame[] = {
    "zero", "HUP", "INT",  "QUIT", "ILL",    "TRAP", "IOT",   "EMT",  "FPE",  "KILL", "BUS",
    "SEGV", "SYS", "PIPE", "ALRM", "TERM",   "URG",  "STOP",  "TSTP", "CONT", "CHLD", "TTIN",
    "TTOU", "IO",  "XCPU", "XFSZ", "VTALRM", "PROF", "WINCH", "PWR",  "USR1", "USR2", NULL};
inline sig_handler_t signal(int sig, sig_handler_t handler, int flag = 0) {
  return ::signal(sig, handler);
}
#else
#if defined(__linux__)
inline const char *sys_signame[] = {
    "zero", "HUP",  "INT",  "QUIT", "ILL",    "TRAP",   "ABRT",  "UNUSED", "FPE",  "KILL", "USR1",
    "SEGV", "USR2", "PIPE", "ALRM", "TERM",   "STKFLT", "CHLD",  "CONT",   "STOP", "TSTP", "TTIN",
    "TTOU", "URG",  "XCPU", "XFSZ", "VTALRM", "PROF",   "WINCH", "IO",     "PWR",  "SYS",  NULL};
#else
inline auto sys_signame = ::sys_signame;
#endif
inline sig_handler_t signal(int sig, sig_handler_t handler, int flag = 0) {
  struct sigaction sa, old;
  memset(&sa, 0, sizeof(sa));
  sigemptyset(&sa.sa_mask);
  if (flag > 0)
    sa.sa_flags = flag;
  sa.sa_handler = handler;
  int r = sigaction(sig, &sa, &old);
  return r == 0 ? old.sa_handler : SIG_ERR;
}
#endif
} // namespace os
} // namespace ex