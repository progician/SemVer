#include <iostream>

int main(int argc, const char*[]) {
  if (argc < 2) {
    std::cerr << "error: command required, please use --help" << std::endl;
    return 1;
  }
  std::cout << "Hello, World!" << std::endl;
  return 1;
}