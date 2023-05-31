#include <gtest/gtest.h>

#include "../src/queue.h"

TEST(queue, queue_push_norm) {
  Queue<size_t, 32> q{};
  q.push(21);
  q.push(45);
  EXPECT_EQ(q.front().value(), 21);
  q.pop();
  EXPECT_EQ(q.front().value(), 45);
}
