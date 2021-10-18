#include <os/tmpdir.h>
#include <iostream>

int main() {
  auto dir = os::tmpdir();
  std::cout << dir << std::endl;
    return 0;
}