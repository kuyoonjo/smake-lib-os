#include <os/arch.h>
#include <iostream>

int main() {
  auto arch = os::arch();
  std::cout << arch << std::endl;
    return 0;
}