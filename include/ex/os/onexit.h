#pragma once

#include "./signal.h"
#include <cstdlib>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

#ifdef _WIN32
#define _M_WIN_ONLY(x) x
#else
#define _M_WIN_ONLY(x)
#endif

namespace ex {
namespace os {
_M_WIN_ONLY(using pexception = PEXCEPTION_POINTERS;)
using exit_handler_t = void (*)(int sig, void* p);

inline void onexit(exit_handler_t cb) {
  static exit_handler_t exit_handler = cb;
#pragma remove_line
  _M_WIN_ONLY(static bool exception_not_caught = true;)
  std::atexit([]() {
    std::cout << "atexit" << std::endl;
    _M_WIN_ONLY(if (exception_not_caught))
    exit_handler(0, nullptr);
  });
  static auto on_signal = [](int sig) {
    _M_WIN_ONLY(if (exception_not_caught))
    exit_handler(sig, nullptr);
    signal(sig, SIG_DFL);
    raise(sig);
  };
  for (auto sig : {SIGINT, SIGTERM})
    signal(sig, on_signal);
#ifdef _WIN32
  signal(SIGABRT, on_signal);
  static auto on_exception = [](PEXCEPTION_POINTERS p) -> LONG {
    exception_not_caught = false;
    exit_handler(-1, p);
    return EXCEPTION_EXECUTE_HANDLER;
  };
  // Signal handler for SIGSEGV and SIGFPE installed in main thread does
  // not work for other threads. Use AddVectoredExceptionHandler instead.
  AddVectoredExceptionHandler(1, static_cast<LONG (__stdcall *)(PEXCEPTION_POINTERS)>(on_exception));
#else
  signal(SIGQUIT, on_signal);
  constexpr int flag = SA_SIGINFO | SA_ONSTACK | SA_NODEFER | SA_RESETHAND;
  for (auto sig : {
           // Signals for which the default action is "Core".
           SIGABRT, // Abort signal from abort(3)
           SIGBUS,  // Bus error (bad memory access)
           SIGFPE,  // Floating point exception
           SIGILL,  // Illegal Instruction
           SIGIOT,  // IOT trap. A synonym for SIGABRT
           SIGQUIT, // Quit from keyboard
           SIGSEGV, // Invalid memory reference
           SIGSYS,  // Bad argument to routine (SVr4)
           SIGTRAP, // Trace/breakpoint trap
           SIGXCPU, // CPU time limit exceeded (4.2BSD)
           SIGXFSZ, // File size limit exceeded (4.2BSD)
#ifdef __APPLE__
           SIGEMT,  // emulation instruction executed
#endif
       })
    signal(sig, on_signal, flag);
  signal(SIGPIPE, SIG_IGN);
#endif
}

} // namespace os
} // namespace ex

#undef _M_WIN_ONLY