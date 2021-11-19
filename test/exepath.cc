#include <ex/os.h>
#include <iostream>

int main() {
  auto dir = ex::os::exepath();
  std::cout << dir << std::endl;
    return 0;
}