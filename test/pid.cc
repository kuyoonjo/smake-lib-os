#include <ex/os.h>
#include <iostream>
#include <stdexcept>

int main() {
  std::cout << "pid:" << ex::os::pid() << std::endl;
  return 0;
}