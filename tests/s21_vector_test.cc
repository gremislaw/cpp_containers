#include <vector>

#include "s21_main_test.h"

template <typename T>
void compare_vectors(std::vector<T>& std_vec, s21::vector<T>& s21_vec) {
  EXPECT_EQ(std_vec.size(), s21_vec.size());
  if (std_vec.size() == s21_vec.size()) {
    for (size_t i = 0; i < std_vec.size(); ++i)
      EXPECT_EQ(std_vec[i], s21_vec[i]);
  }
  return;
}

TEST(Vector, ConstructorDefault) {
  s21::vector<int> a;
  EXPECT_EQ(a.size(), 0UL);
  EXPECT_EQ(a.capacity(), 0UL);
}

TEST(Vector, ConstructorParametric) {
  s21::vector<int> a(5);
  EXPECT_EQ(a.size(), 5UL);
  EXPECT_EQ(a.capacity(), 5UL);
}

TEST(Vector, ConstructorInitializeList) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  compare_vectors(a, b);
}

TEST(Vector, ConstructorCopy) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  std::vector<int> c(a);
  s21::vector<int> d(b);
  compare_vectors(c, d);
}

TEST(Vector, MoveConstructor) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  std::vector<int> c(std::move(a));
  s21::vector<int> d(std::move(b));
  compare_vectors(c, d);
}

TEST(Vector, OperatorAssignmentMove) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  std::vector<int> c = std::move(a);
  s21::vector<int> d = std::move(b);
  compare_vectors(c, d);
}

TEST(Vector, OperatorBrackets) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  a[0] = 10;
  a[1] = 20;
  a[2] = 30;
  b[0] = 10;
  b[1] = 20;
  b[2] = 30;
  compare_vectors(a, b);
}

TEST(Vector, MethodAt) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  a.at(0) = 10;
  a.at(1) = 20;
  a.at(2) = 30;
  b.at(0) = 10;
  b.at(1) = 20;
  b.at(2) = 30;
  compare_vectors(a, b);
}

TEST(Vector, MethodFront) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  EXPECT_EQ(a.front(), b.front());
}

TEST(Vector, MethodBack) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  EXPECT_EQ(a.back(), b.back());
}

TEST(Vector, MethodBeginEnd) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  EXPECT_EQ(*(a.begin()), *(b.begin()));
}

TEST(Vector, MethodSizeCapacity) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  a.push_back(6);
  b.push_back(6);
  EXPECT_EQ(a.size(), b.size());
  EXPECT_GT(a.capacity(), a.size());
  EXPECT_GT(b.capacity(), b.size());
}

TEST(Vector, MethodEmpty) {
  std::vector<int> a;
  s21::vector<int> b;
  EXPECT_EQ(a.empty(), true);
  EXPECT_EQ(b.empty(), true);
}

TEST(Vector, MethodReserve) {
  std::vector<int> a{1, 2};
  s21::vector<int> b{1, 2};
  a.reserve(10);
  b.reserve(10);
  EXPECT_EQ(a.capacity(), b.capacity());
  EXPECT_EQ(a.capacity(), 10UL);
}

TEST(Vector, MethodShrink) {
  std::vector<int> a;
  s21::vector<int> b;
  a.push_back(1);
  a.push_back(2);
  b.push_back(1);
  b.push_back(2);
  a.shrink_to_fit();
  b.shrink_to_fit();
  EXPECT_EQ(a.capacity(), a.size());
  EXPECT_EQ(b.capacity(), b.size());
}

TEST(Vector, MethodClear) {
  s21::vector<int> a;
  ;
  a.push_back(1);
  a.push_back(2);
  a.clear();
  EXPECT_EQ(a.size(), 0UL);
}

TEST(Vector, MethodInsert) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  a.insert(a.begin(), 10);
  b.insert(b.begin(), 10);
  EXPECT_EQ(a[0], b[0]);
  EXPECT_EQ(a[0], 10);
}

TEST(Vector, MethodErase) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  a.erase(a.begin());
  b.erase(b.begin());
  compare_vectors(a, b);
}

TEST(Vector, MethodPushBack) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  a.push_back(6);
  b.push_back(6);
  ;
  compare_vectors(a, b);
  EXPECT_EQ(b[5], 6);
}

TEST(Vector, MethodPopBack) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  a.pop_back();
  b.pop_back();
  compare_vectors(a, b);
  EXPECT_EQ(b.size(), 4UL);
}

TEST(Vector, MethodSwap) {
  std::vector<int> a{1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  std::vector<int> c{6, 7};
  s21::vector<int> d{6, 7};
  b.swap(d);
  compare_vectors(a, d);
  compare_vectors(c, b);
}

TEST(Vector, MethodInsertMany) {
  std::vector<int> a{6, 7, 8, 1, 2, 3, 4, 5};
  s21::vector<int> b{1, 2, 3, 4, 5};
  b.insert_many(b.begin(), 6, 7, 8);
  EXPECT_EQ(b.size(), 8UL);
  compare_vectors(a, b);
}

TEST(Vector, MethodInsertManyBack) {
  std::vector<int> a{1, 2, 3, 4, 5, 6, 7, 8};
  s21::vector<int> b{1, 2, 3, 4, 5};
  b.insert_many_back(6, 7, 8);
  EXPECT_EQ(b.size(), 8UL);
  compare_vectors(a, b);
}
