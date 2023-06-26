#include "../src/common/queue.h"

#include <thread> 
#include <ranges>
#include <vector>
#include <iostream> 
#include <numeric>
void single_thread(Queue<size_t, 3000>& q) {
  
  for(int i = 0; i < 3000; ++i) {
    if(!q.push(i)) {
      std::cout << "queue full at " << i  << "elements.\n";
      break;
    }
  }
  return;
}

int main() {
  Queue<size_t, 3000> q1;

  int av = 0;
  int n = 10000;
  std::vector<size_t> times;
  times.reserve(n);
  for(int i = 0; i < n; ++i) {
    size_t s = std::chrono::steady_clock::now().time_since_epoch().count();
    single_thread(q1);
    times.push_back((std::chrono::steady_clock::now().time_since_epoch().count() - s));
  }
  std::cout << "Single Thread: " << std::accumulate(times.begin(), times.end(), 0, std::plus<size_t>()) / n << "\n";
}
