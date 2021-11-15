#pragma once

#include <string>
#include <filesystem>

namespace ex {
namespace os {
inline std::string tmpdir() {
  return std::filesystem::temp_directory_path();
}
} // namespace os
} // namespace ex