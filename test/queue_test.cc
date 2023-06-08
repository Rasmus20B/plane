#include <gtest/gtest.h>

#include "../src/common/queue.h"

TEST(queue, queue_push_norm) {
  Queue<size_t, 32> q{};
  q.push(21);
  q.push(45);
  EXPECT_EQ(q.front().value(), 21);
  q.pop();
  EXPECT_EQ(q.front().value(), 45);
}

TEST(queue, queue_wrap) {
  Queue<size_t, 8> q{};
  for(int i = 0; i < 8; i++) {
    q.push(i);
  }

  for(int i = 0; i < 8; i++) {
    EXPECT_EQ(q.pop_and_get(), i);
  }

  q.push(1);
  q.push(2);
  q.push(3);
  EXPECT_EQ(q.pop_and_get(), 1);
  EXPECT_EQ(q.pop_and_get(), 2);
  EXPECT_EQ(q.pop_and_get(), 3);
  EXPECT_EQ(q.pop_and_get(), std::nullopt);
  q.push(5);
  EXPECT_EQ(q.pop_and_get(), 5);
}
