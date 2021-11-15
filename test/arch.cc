#include <ex/os.h>
#include <iostream>

int main() {
  auto arch = ex::os::arch();
  std::cout << arch << std::endl;
    return 0;
}