#pragma once

#include <string>

namespace ex {
namespace os {
inline std::string arch() {
#if defined(__aarch64__)
  return "arm64";
#elif defined(__arm__)
  return "arm";
#elif defined(__x86_64__)
  return "x86_64";
#elif defined(__i386__)
  return "i386";
#elif defined(__ia64__)
  return "ia64";
#elif defined(__mips__)
  return "mips";
#elif defined(__riscv_64)
  return "rv64";
#elif defined(__riscv_32)
  return "rv32";
#elif defined(__riscv)
  return "riscv";
#elif defined(__alpha__)
  return "alpha";
#elif defined(__bfin)
  return "bfin";
#elif defined(__convex__)
  return "convex";
#elif defined(__epiphany__)
  return "epiphany";
#elif defined(__hppa__)
  return "hppa";
#elif defined(__m68k__)
  return "m68k";
#elif defined(__powerpc__)
  return "powerpc";
#elif defined(__sparc__)
  return "sparc";
#elif defined(__sh__)
  return "sh";
#elif defined(__sparc__)
  return "sparc";
#elif defined(__s390__)
  return "s390";
#elif defined(__s390x__)
  return "s390x";
#elif defined(__zarch__)
  return "zarch";
#endif
  return "Unknown";
}
} // namespace os
} // namespace ex