#pragma once

#include <thread>

namespace ex {
namespace os {
inline auto cpunum() { return std::thread::hardware_concurrency(); }
} // namespace os
} // namespace ex