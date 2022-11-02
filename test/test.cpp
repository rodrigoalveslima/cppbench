// Copyright (C) 2022 Georgia Tech Center for Experimental Research in Computer
// Systems

#include <functional>
#include <string>
#include <thread>

#include "cppbench.h"

int fibonacci(int n) {
  if (n == 0) return 0;
  if (n == 1) return 1;
  return fibonacci(n - 1) + fibonacci(n - 2);
}

void thread_creation() {
  std::thread t(fibonacci, 1);
  t.join();
}

int main(int argc, char *argv[]) {
  run_benchmark<int>(std::bind(fibonacci, 1), nullptr, nullptr, 100000, 60,
                     std::string(argv[1]) + "fibonacci.csv");
  run_benchmark<void>(std::bind(thread_creation), nullptr, nullptr, 100000, 60,
                      std::string(argv[1]) + "thread_creation.csv");
  return 0;
}
