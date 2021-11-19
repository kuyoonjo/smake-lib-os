#include "ex/os/cpunum.h"
#include <ex/os.h>
#include <iostream>
#include <stdexcept>

int main() {
  std::cout << "cpunum:" << ex::os::cpunum() << std::endl;
  return 0;
}