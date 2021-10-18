#include <string>

namespace os {
inline std::string tmpdir() {
  std::string res;
#ifdef _WIN32
  const char *homeEnv = std::getenv("TMP");
#endif
#ifndef _WIN32
  const char *homeEnv = std::getenv("TMPDIR");
#endif
  res = homeEnv;
  return res;
}
} // namespace os
