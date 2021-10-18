#include <string>

namespace os {
inline std::string homedir() {
  std::string res;
#ifdef _WIN32
  const char *homeEnv = std::getenv("USERPROFILE");
#endif
#ifndef _WIN32
  const char *homeEnv = std::getenv("HOME");
#endif
  res = homeEnv;
  return res;
}
} // namespace os
