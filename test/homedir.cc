#include <ex/os.h>
#include <iostream>

int main() {
  auto dir = ex::os::homedir();
  std::cout << dir << std::endl;
    return 0;
}