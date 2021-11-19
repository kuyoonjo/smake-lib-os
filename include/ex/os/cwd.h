#pragma once

#include <filesystem>

namespace ex {
namespace os {
inline auto cwd() { return std::filesystem::current_path(); }
} // namespace os
} // namespace ex