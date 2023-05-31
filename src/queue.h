#include <atomic>
#include <mutex>
#include <concepts>

#include <iostream>

template<class T, size_t N>
requires(std::has_single_bit(N))
struct alignas(N) Queue{
  bool push(T val) {
    std::scoped_lock<std::mutex> lock(m);
    if(size + 1 > capacity)
      return false;
    
    buf[head] = val;
    head++;
    return true;
  }

  void pop() {
    std::scoped_lock<std::mutex> lock(m);
    if(head >= tail)
      tail++;
  }
  
  std::optional<T> pop_and_get() {
    std::scoped_lock<std::mutex> lock(m);
    if(tail == head) return {};
    tail++;
    return buf[tail--];
  }

  std::optional<T> front() {
    std::scoped_lock<std::mutex> lock(m);
    return buf[tail];
  }

  std::array<T, N> buf{};
  size_t head = 0, tail = 0;
  size_t capacity = N, size = 0;
  std::mutex m;
};
