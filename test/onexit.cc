#include <ex/os.h>
#include <iostream>
#include <stdexcept>

int main() {
  ex::os::onexit([](int sig, void *p) {
    if (sig == -1) {
#ifdef _WIN32
      auto pe = (ex::os::pexception)p;
      std::cout << "onexit. ExceptionCode: 0x"
                << std::hex 
                << pe->ExceptionRecord->ExceptionCode << std::endl;
#endif
      return;
    }
    std::cout << "onexit. sig: " << ex::os::sys_signame[sig] << '(' << sig << ')' << std::endl;
  });
  // throw std::runtime_error("...");
  struct A {
    int b;
  };
  A *a = nullptr;
  std::cout << a->b << std::endl;
  return 0;
}