#include <array>

#include "s21_main_test.h"

template <typename T, std::size_t N>
void compare_arrays(std::array<T, N>& std_arr, s21::array<T, N>& s21_arr) {
  EXPECT_EQ(std_arr.size(), s21_arr.size());
  if (std_arr.size() == s21_arr.size()) {
    for (size_t i = 0; i < std_arr.size(); ++i)
      EXPECT_EQ(std_arr[i], s21_arr[i]);
  }
  return;
}

TEST(Array, ConstructorDefault) {
  s21::array<int, 5> a;
  EXPECT_EQ(a.size(), 5UL);
  for (size_t i = 0; i < 5; ++i) EXPECT_EQ(a[i], 0);
}

TEST(Array, ConstructorInitializeList) {
  std::array<int, 5> a{1, 2, 3, 4, 5};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  compare_arrays(a, b);
}

TEST(Array, ConstructorCopy) {
  std::array<int, 5> a{1, 2, 3, 4, 5};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  std::array<int, 5> c(a);
  s21::array<int, 5> d(b);
  compare_arrays(c, d);
}

TEST(Array, MoveConstructor) {
  std::array<int, 5> a{1, 2, 3, 4, 5};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  std::array<int, 5> c(std::move(a));
  s21::array<int, 5> d(std::move(b));
  compare_arrays(c, d);
}

TEST(Array, OperatorAssignmentMove) {
  std::array<int, 5> a{1, 2, 3, 4, 5};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  std::array<int, 5> c = std::move(a);
  s21::array<int, 5> d = std::move(b);
  compare_arrays(c, d);
}

TEST(Array, OperatorBrackets) {
  std::array<int, 5> a{1, 2, 3, 4, 5};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  a[0] = 10;
  a[1] = 20;
  a[2] = 30;
  b[0] = 10;
  b[1] = 20;
  b[2] = 30;
  compare_arrays(a, b);
}

TEST(Array, MethodAt) {
  std::array<int, 5> a{1, 2, 3, 4, 5};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  a.at(0) = 10;
  a.at(1) = 20;
  a.at(2) = 30;
  b.at(0) = 10;
  b.at(1) = 20;
  b.at(2) = 30;
  compare_arrays(a, b);
}

TEST(Array, MethodFront) {
  std::array<int, 5> a{1, 2, 3, 4, 5};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  EXPECT_EQ(a.front(), b.front());
}

TEST(Array, MethodBack) {
  std::array<int, 5> a{1, 2, 3, 4, 5};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  EXPECT_EQ(a.back(), b.back());
}

TEST(Array, MethodBeginEnd) {
  std::array<int, 5> a{1, 2, 3, 4, 5};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  EXPECT_EQ(*(a.begin()), *(b.begin()));
}

TEST(Array, MethodEmpty) {
  std::array<int, 0> a;
  s21::array<int, 0> b;
  EXPECT_EQ(a.empty(), true);
  EXPECT_EQ(b.empty(), true);
}

TEST(Array, MethodSize) {
  std::array<int, 5> a{1, 2, 3, 4, 5};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  EXPECT_EQ(a.size(), b.size());
}

TEST(Array, MethodSwap) {
  std::array<int, 5> a{1, 2, 3, 4, 5};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  std::array<int, 5> c{6, 7, 8, 9, 10};
  s21::array<int, 5> d{6, 7, 8, 9, 10};
  b.swap(d);
  compare_arrays(a, d);
  compare_arrays(c, b);
}

TEST(Array, MethodFill) {
  std::array<int, 5> a{10, 10, 10, 10, 10};
  s21::array<int, 5> b{1, 2, 3, 4, 5};
  b.fill(10);
  compare_arrays(a, b);
}
