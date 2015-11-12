// Compile with -std=c++11
#include <iostream>
#include <chrono>

class Profiler {
  public:
    Profiler() { start = std::chrono::high_resolution_clock::now(); }
    Profiler(const Profiler &) = delete;
    Profiler & operator=(const Profiler &) = delete;
    ~Profiler() {
      auto end = std::chrono::high_resolution_clock::now();
      auto duration = end - start;
      std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() << " ns" << std::endl;
    }
  private:
    std::chrono::high_resolution_clock::time_point start;
};
