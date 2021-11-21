#include <ex/os.h>
#include <iostream>
#include <stdexcept>

int main() {
  std::cout << "totalmem:" << ex::os::totalmem() << std::endl;
  return 0;
}