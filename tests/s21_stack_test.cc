#include <stack>

#include "s21_main_test.h"

void initialize_std_stack(std::stack<int>& st) {
  for (int item : {1, 2, 3, 4, 5}) st.push(item);
}

template <typename T>
void compare_stacks(std::stack<T, std::deque<T>>& std_stack,
                    s21::stack<T>& s21_stack) {
  while ((!std_stack.empty()) && (!s21_stack.empty())) {
    T a = std_stack.top();
    T b = s21_stack.top();
    EXPECT_EQ(a, b);
    std_stack.pop();
    s21_stack.pop();
  }
  EXPECT_EQ(std_stack.empty(), s21_stack.empty());
}

TEST(Stack, ConstructorDefault) {
  s21::stack<int> a;
  EXPECT_EQ(a.size(), 0UL);
}

TEST(Stack, ConstructorInitializeList) {
  std::stack<int> a;
  initialize_std_stack(a);
  s21::stack<int> b{1, 2, 3, 4, 5};
  compare_stacks(a, b);
}

TEST(Stack, ConstructorCopy) {
  std::stack<int> a;
  initialize_std_stack(a);
  s21::stack<int> b{1, 2, 3, 4, 5};
  std::stack<int> c(a);
  s21::stack<int> d(b);
  compare_stacks(c, d);
}

TEST(Stack, MoveConstructor) {
  std::stack<int> a;
  initialize_std_stack(a);
  s21::stack<int> b{1, 2, 3, 4, 5};
  std::stack<int> c(std::move(a));
  s21::stack<int> d(std::move(b));
  compare_stacks(c, d);
}

TEST(Stack, OperatorEqualMove) {
  std::stack<int> a;
  initialize_std_stack(a);
  s21::stack<int> b{1, 2, 3, 4, 5};
  std::stack<int> c = std::move(a);
  s21::stack<int> d = std::move(b);
  compare_stacks(c, d);
}

TEST(Stack, MethodTop) {
  std::stack<int> a;
  initialize_std_stack(a);
  s21::stack<int> b{1, 2, 3, 4, 5};
  EXPECT_EQ(a.top(), b.top());
}

TEST(Stack, MethodEmpty) {
  s21::stack<int> a;
  s21::stack<int> b{1, 2, 3, 4, 5};
  EXPECT_EQ(a.empty(), true);
  EXPECT_EQ(b.empty(), false);
}

TEST(Stack, MethodSize) {
  s21::stack<int> a{1, 2, 3, 4, 5};
  EXPECT_EQ(a.size(), 5UL);
  a.push(6);
  EXPECT_EQ(a.size(), 6UL);
}

TEST(Stack, MethodPush) {
  s21::stack<int> a{1, 2, 3};
  a.push(4);
  EXPECT_EQ(a.top(), 4);
  a.push(5);
  EXPECT_EQ(a.top(), 5);
}

TEST(Stack, MethodPop) {
  s21::stack<int> a{1, 2, 3, 4, 5};
  EXPECT_EQ(a.top(), 5);
  a.pop();
  EXPECT_EQ(a.top(), 4);
  a.pop();
  EXPECT_EQ(a.top(), 3);
}

TEST(Stack, MethodSwap) {
  std::stack<int> a;
  initialize_std_stack(a);
  s21::stack<int> b{1, 2, 3, 4, 5};
  std::stack<int> c;
  c.push(6);
  c.push(7);
  s21::stack<int> d{6, 7};
  b.swap(d);
  compare_stacks(a, d);
  compare_stacks(c, b);
}

TEST(Stack, MethodInsertManyBack) {
  std::stack<int> a;
  initialize_std_stack(a);
  a.push(6);
  a.push(7);
  a.push(8);
  s21::stack<int> b{1, 2, 3, 4, 5};
  b.insert_many_back(6, 7, 8);
  EXPECT_EQ(b.size(), 8UL);
  compare_stacks(a, b);
}
