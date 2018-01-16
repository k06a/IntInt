// Copyright 2017 <ggarber@github>

#include "IntInt.hpp"

#include <iostream>

using bootstrap::IntInt;

void IntInt::run() const {
  std::cout << "Hello world!\n";
}

int IntInt::sum(int a, int b) const {
  return a + b;
}
