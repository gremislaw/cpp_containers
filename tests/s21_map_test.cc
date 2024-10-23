#include <map>

#include "s21_main_test.h"

template <typename Key, typename T>
void compare_maps(std::map<Key, T>& std_map, s21::map<Key, T>& s21_map) {
  EXPECT_EQ(std_map.size(), s21_map.size());
  if (std_map.size() == s21_map.size()) {
    auto it1 = std_map.begin();
    auto it2 = s21_map.begin();
    while (it1 != std_map.end() && it2 != s21_map.end()) {
      EXPECT_EQ((*it1).first, (*it2).first);
      EXPECT_EQ((*it1).second, (*it2).second);
      ++it1, ++it2;
    }
  }
  return;
}

TEST(Map, ConstructorDefault) {
  s21::map<int, int> a;
  EXPECT_EQ(a.size(), 0UL);
}

TEST(Map, ConstructorInitializeList) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  compare_maps(a, b);
}

TEST(Map, ConstructorCopy) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  std::map<int, int> c(a);
  s21::map<int, int> d(b);
  compare_maps(c, d);
}

TEST(Map, MoveConstructor) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  std::map<int, int> c(std::move(a));
  s21::map<int, int> d(std::move(b));
  compare_maps(c, d);
}

TEST(Map, OperatorAssignmentMove) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  std::map<int, int> c = std::move(a);
  s21::map<int, int> d = std::move(b);
  compare_maps(c, d);
}

TEST(Map, OperatorBrackets) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  a[1] = 10;
  a[2] = 20;
  a[3] = 30;
  b[1] = 10;
  b[2] = 20;
  b[3] = 30;
  compare_maps(a, b);
}

TEST(Map, MethodAt) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  a.at(1) = 10;
  a.at(2) = 20;
  a.at(3) = 30;
  b.at(1) = 10;
  b.at(2) = 20;
  b.at(3) = 30;
  compare_maps(a, b);
}

TEST(Map, MethodBeginEnd) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  EXPECT_EQ(*(a.begin()), *(b.begin()));
}

TEST(Map, MethodSize) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  b.insert(6, 6);
  EXPECT_EQ(a.size(), b.size());
  EXPECT_EQ(6UL, b.size());
}

TEST(Map, MethodEmpty) {
  std::map<int, int> a;
  s21::map<int, int> b;
  EXPECT_EQ(a.empty(), true);
  EXPECT_EQ(b.empty(), true);
}

TEST(Map, MethodClear) {
  s21::map<int, int> a;
  a.insert(1, 1);
  a.insert(2, 2);
  a.clear();
  EXPECT_EQ(a.size(), 0UL);
}

TEST(Map, MethodInsertPair) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  a.insert(std::make_pair<int, int>(6, 6));
  b.insert(std::make_pair<int, int>(6, 6));
  EXPECT_EQ(a[6], b[6]);
  EXPECT_EQ(b[6], 6);
}

TEST(Map, MethodInsertKeyValue) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  b.insert(6, 6);
  EXPECT_EQ(a[6], b[6]);
  EXPECT_EQ(b[6], 6);
  compare_maps(a, b);
}

TEST(Map, MethodInsertOrAssign) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  a.insert_or_assign(5, 10);
  a.insert_or_assign(6, 6);
  b.insert_or_assign(5, 10);
  b.insert_or_assign(6, 6);
  EXPECT_EQ(b.size(), 6UL);
  compare_maps(a, b);
}

TEST(Map, MethodSwap) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  std::map<int, int> c{{6, 6}};
  s21::map<int, int> d{{6, 6}};
  b.swap(d);
  compare_maps(a, d);
  compare_maps(c, b);
}

TEST(Map, MethodMerge) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  std::map<int, int> c{{6, 6}};
  s21::map<int, int> d{{6, 6}};
  a.merge(c);
  b.merge(d);
  compare_maps(a, b);
  compare_maps(c, d);
  EXPECT_EQ(b.size(), 6UL);
}

TEST(Map, MethodContains) {
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  EXPECT_EQ(b.contains(1), true);
  EXPECT_EQ(b.contains(6), false);
}

TEST(Map, MethodInsertMany) {
  std::map<int, int> a{{1, 1}, {2, 2}, {3, 3}, {4, 4},
                       {5, 5}, {6, 6}, {7, 7}, {8, 8}};
  s21::map<int, int> b{{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}};
  b.insert_many(std::make_pair<int, int>(6, 6), std::make_pair<int, int>(7, 7),
                std::make_pair<int, int>(8, 8));
  EXPECT_EQ(b.size(), 8UL);
  compare_maps(a, b);
}
