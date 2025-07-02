#include "file.hpp"
#include "helpers.hpp"
#include "program.hpp"

#include <iostream>

int main(int argc, const char **argv) {
  if (Helpers::check_args(argc)) {
    Program::execute(argv[1]);
  }
}
