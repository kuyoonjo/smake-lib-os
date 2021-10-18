#include <os/homedir.h>
#include <iostream>

int main() {
  auto dir = os::homedir();
  std::cout << dir << std::endl;
    return 0;
}