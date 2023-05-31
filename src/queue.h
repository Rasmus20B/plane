#include <atomic>
#include <mutex>
#include <concepts>

#include <iostream>

template<class T, size_t N>
requires(std::has_single_bit(N))
struct alignas(N) Queue{
  bool push(T val) {
    std::scoped_lock<std::mutex> lock(m);
    if(head == capacity)
      return false;
    
    buf[head] = val;
    populated[head] = true;
    head++;
    return true;
  }

  void pop() {
    std::scoped_lock<std::mutex> lock(m);
    if(head > tail) {
      populated[tail] = false;
      tail++;
    }
  }
  
  std::optional<T> pop_and_get() {
    std::scoped_lock<std::mutex> lock(m);
    if(populated[tail] && head > tail){ 

      populated[tail] = false;
      tail++;
      return buf[tail-1];
    }
    return {};
  }

  std::optional<T> front() {
    std::scoped_lock<std::mutex> lock(m);
    return buf[tail];
  }

  std::array<T, N> buf{};
  std::array<bool, N> populated{};
  size_t head = 0, tail = 0;
  size_t capacity = N, size = 0;
  std::mutex m;
};
