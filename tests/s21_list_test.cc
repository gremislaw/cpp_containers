#include <list>

#include "s21_main_test.h"

template <typename T>
void compare_lists(std::list<T>& std_list, s21::list<T>& s21_list) {
  while ((!std_list.empty()) && (!s21_list.empty())) {
    T a = std_list.front();
    T b = s21_list.front();
    EXPECT_EQ(a, b);
    std_list.pop_front();
    s21_list.pop_front();
  }
  EXPECT_EQ(std_list.size(), s21_list.size());
}

TEST(List, ConstructorDefault) {
  s21::list<int> a;
  EXPECT_EQ(a.size(), 0UL);
}

TEST(List, ConstructorParameterized) {
  std::list<int> a(5);
  EXPECT_EQ(a.size(), 5UL);
}

TEST(List, ConstructorInitializeList) {
  std::list<int> a{1, 2, 3, 4, 5};
  s21::list<int> b{1, 2, 3, 4, 5};
  compare_lists(a, b);
}

TEST(List, ConstructorCopy) {
  std::list<int> a{1, 2, 3, 4, 5};
  s21::list<int> b{1, 2, 3, 4, 5};
  std::list<int> c(a);
  s21::list<int> d(b);
  compare_lists(c, d);
}

TEST(List, ConstructorMove) {
  std::list<int> a{1, 2, 3, 4, 5};
  s21::list<int> b{1, 2, 3, 4, 5};
  std::list<int> c(std::move(a));
  s21::list<int> d(std::move(b));
  compare_lists(c, d);
}

TEST(List, OperatorAssigmentMove) {
  std::list<int> a{1, 2, 3, 4, 5};
  s21::list<int> b{1, 2, 3, 4, 5};
  std::list<int> c = std::move(a);
  s21::list<int> d = std::move(b);
  compare_lists(c, d);
}

TEST(List, MethodFrontBack) {
  std::list<int> a{1, 2, 3, 4, 5};
  s21::list<int> b{1, 2, 3, 4, 5};
  EXPECT_EQ(a.front(), b.front());
  EXPECT_EQ(a.back(), b.back());
}

TEST(List, MethodBeginEnd) {
  std::list<int> a{1, 2, 3, 4, 5};
  s21::list<int> b{1, 2, 3, 4, 5};
  auto it1 = a.begin();
  auto it2 = b.begin();
  for (; (it1 != a.end()) && (it2 != b.end()); ++it1, ++it2)
    EXPECT_EQ(*it1, *it2);
  EXPECT_EQ(it1, a.end());
  EXPECT_EQ(it2, b.end());
}

TEST(List, MethodEmpty) {
  s21::list<int> a;
  s21::list<int> b{1, 2, 3, 4, 5};
  EXPECT_EQ(a.empty(), true);
  EXPECT_EQ(b.empty(), false);
}

TEST(List, MethodSize) {
  s21::list<int> a{1, 2, 3, 4, 5};
  EXPECT_EQ(a.size(), 5UL);
  a.push_back(6);
  EXPECT_EQ(a.size(), 6UL);
}

TEST(List, MethodClear) {
  s21::list<int> a{1, 2, 3};
  EXPECT_EQ(a.size(), 3UL);
  a.clear();
  EXPECT_EQ(a.size(), 0UL);
}

TEST(List, MethodInsert) {
  s21::list<int> a{1, 2, 3, 4, 5};
  a.insert(a.begin(), 10);
  a.insert(a.end(), 20);
  EXPECT_EQ(a.front(), 10);
  EXPECT_EQ(a.back(), 20);
}

TEST(List, MethodErase) {
  s21::list<int> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  a.erase(a.begin());
  EXPECT_EQ(a.front(), 2);
  auto it = a.end();
  --it;
  a.erase(it);
  EXPECT_EQ(a.back(), 9);
  it = a.begin();
  ++it;
  a.erase(it);
  EXPECT_EQ(a.size(), 7UL);
}

TEST(List, MethodPushBack) {
  s21::list<int> a;
  a.push_back(1);
  EXPECT_EQ(a.size(), 1UL);
  EXPECT_EQ(a.back(), 1);
  a.push_back(2);
  EXPECT_EQ(a.size(), 2UL);
  EXPECT_EQ(a.back(), 2);
  a.push_back(4);
  EXPECT_EQ(a.size(), 3UL);
  EXPECT_EQ(a.back(), 4);
}

TEST(List, MethodPushFront) {
  s21::list<int> a;
  a.push_front(1);
  EXPECT_EQ(a.size(), 1UL);
  EXPECT_EQ(a.front(), 1);
  a.push_front(2);
  EXPECT_EQ(a.size(), 2UL);
  EXPECT_EQ(a.front(), 2);
  a.push_front(4);
  EXPECT_EQ(a.size(), 3UL);
  EXPECT_EQ(a.front(), 4);
}

TEST(List, MethodPopFront) {
  s21::list<int> a{1, 2};
  a.pop_front();
  EXPECT_EQ(a.size(), 1UL);
  EXPECT_EQ(a.front(), 2);
  a.pop_front();
  EXPECT_EQ(a.size(), 0UL);
  a.pop_front();
  EXPECT_EQ(a.size(), 0UL);
}

TEST(List, MethodPopBack) {
  s21::list<int> a{1, 2};
  a.pop_back();
  EXPECT_EQ(a.size(), 1UL);
  EXPECT_EQ(a.back(), 1);
  a.pop_back();
  EXPECT_EQ(a.size(), 0UL);
  a.pop_back();
  EXPECT_EQ(a.size(), 0UL);
}

TEST(List, MethodSwap) {
  std::list<int> a{1, 2, 3, 4, 5};
  s21::list<int> b{1, 2, 3, 4, 5};
  std::list<int> c{6, 7};
  s21::list<int> d{6, 7};
  b.swap(d);
  compare_lists(a, d);
  compare_lists(c, b);
}

TEST(List, MethodSort) {
  std::list<int> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  s21::list<int> b{1, 3, 6, 8, 2, 5, 4, 9, 10, 7};
  b.sort();
  compare_lists(a, b);
}

TEST(List, MethodReverse) {
  std::list<int> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  s21::list<int> b{10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  b.reverse();
  compare_lists(a, b);
}

TEST(List, MethodUnique) {
  std::list<int> a{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  s21::list<int> b{10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 1, 2,  3, 6, 6,
                   4,  4, 7, 8, 9, 2, 3, 4, 5, 6, 7, 10, 1, 1};
  b.sort();
  b.unique();
  compare_lists(a, b);
}

TEST(List, MethodMerge) {
  std::list<int> a{1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  s21::list<int> b{10, 9, 8, 7, 6, 1};
  s21::list<int> c{1, 2, 4, 5, 3};
  b.sort();
  c.sort();
  b.merge(c);
  compare_lists(a, b);
}

TEST(List, MethodSplice) {
  std::list<int> a{1, 6, 7, 8, 9, 10, 2, 3, 4, 5};
  s21::list<int> b{1, 2, 3, 4, 5};
  s21::list<int> c{6, 7, 8, 9, 10};
  auto it = b.begin();
  ++it;
  b.splice(it, c);
  compare_lists(a, b);
}

TEST(List, MethodInsertManyBack) {
  std::list<int> a{1, 2, 3, 4, 5, 6, 7, 8};
  s21::list<int> b{1, 2, 3, 4, 5};
  b.insert_many_back(6, 7, 8);
  EXPECT_EQ(b.size(), 8UL);
  compare_lists(a, b);
}

TEST(List, MethodInsertManyFront) {
  std::list<int> a{1, 2, 3, 4, 5, 6, 7, 8};
  s21::list<int> b{4, 5, 6, 7, 8};
  b.insert_many_front(1, 2, 3);
  EXPECT_EQ(b.size(), 8UL);
  compare_lists(a, b);
}

TEST(List, MethodInsertMany) {
  std::list<int> a{1, 2, 3, 4, 5, 6, 7, 8};
  s21::list<int> b{1, 5, 6, 7, 8};
  auto it = b.begin();
  ++it;
  b.insert_many(it, 2, 3, 4);
  EXPECT_EQ(b.size(), 8UL);
  compare_lists(a, b);
}
