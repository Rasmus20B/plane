#include <atomic>
#include <array>
#include <mutex>
#include <concepts>
#include <optional> 
#include <bit> 

template<class T, size_t N>
requires(std::has_single_bit(N), sizeof(T) < 64)
struct alignas(64) Queue{

  [[nodiscard]]
  bool empty() {
    return head == tail;
  }

  [[nodiscard]]
  bool full() {
    return tail == head + 1;
  }

  bool push(T val) {
    std::scoped_lock<std::mutex> lock(m);
    if(this->full()) return false;
    if(head == capacity && !populated[0]) {
      head = 0;
      buf[head] = val;
      populated[head] = true;
      head++;
      return true;
    }
    buf[head] = val;
    populated[head] = true;
    head++;
    return true;
  }

  void pop() {
    std::scoped_lock<std::mutex> lock(m);
    if(tail == head) return;
    if(tail == capacity) tail = 0;
    if(head > tail) {
      populated[tail] = false;
      tail++;
    }
  }
  
  std::optional<T> pop_and_get() {
    std::scoped_lock<std::mutex> lock(m);
    if(populated[tail]){ 
      if(tail == capacity - 1) {
        tail = 0;
        populated[capacity - 1] = false;
        return buf[capacity - 1];
      }
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
