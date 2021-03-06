#pragma once

#include <string>
#include <filesystem>

namespace ex {
namespace os {
inline auto tmpdir() {
  return std::filesystem::temp_directory_path();
}
} // namespace os
} // namespace ex