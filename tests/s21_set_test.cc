#include "s21_main_test.h"

TEST(Set, DefaultConstructor) {
  s21::set<int> test;
  EXPECT_TRUE(test.empty());
}

TEST(Set, InitializerConstructor) {
  s21::set<int> test = {1, 2, 3};
  EXPECT_EQ(test.size(), 3UL);
  EXPECT_TRUE(test.contains(1));
  EXPECT_TRUE(test.contains(2));
  EXPECT_TRUE(test.contains(3));
  EXPECT_FALSE(test.contains(4));
}

TEST(Set, CopyConstructor) {
  s21::set<int> test = {1, 2, 3};
  s21::set<int> Copy(test);
  EXPECT_EQ(test.size(), Copy.size());

  int i = 1;
  for (auto it = Copy.begin(); it != Copy.end(); ++it, ++i) {
    EXPECT_EQ(*it, i);
  }
}

TEST(Set, MoveConstructor) {
  s21::set<int> test = {1, 2, 3};
  s21::set<int> Copy = std::move(test);

  EXPECT_EQ(Copy.size(), 3UL);
  EXPECT_EQ(test.size(), 0UL);

  EXPECT_TRUE(Copy.contains(1));
  EXPECT_TRUE(Copy.contains(2));
  EXPECT_TRUE(Copy.contains(3));

  EXPECT_TRUE(test.empty());
}

TEST(Set, CopyAssignment) {
  s21::set<int> origin = {1, 2, 3};
  s21::set<int> copy = {4, 5, 6, 7, 8, 9};

  copy = origin;

  EXPECT_EQ(origin.size(), copy.size());

  EXPECT_TRUE(copy.contains(1));
  EXPECT_TRUE(copy.contains(2));
  EXPECT_TRUE(copy.contains(3));

  EXPECT_FALSE(copy.contains(4));
  EXPECT_FALSE(copy.contains(5));
  EXPECT_FALSE(copy.contains(6));
  EXPECT_FALSE(copy.contains(7));
  EXPECT_FALSE(copy.contains(8));
  EXPECT_FALSE(copy.contains(9));
}

TEST(Set, MoveAssignment) {
  s21::set<int> origin = {1, 2, 3};
  s21::set<int> copy = {4, 5, 6, 7, 8, 9};

  copy = std::move(origin);

  EXPECT_EQ(origin.size(), 0UL);
  EXPECT_TRUE(origin.empty());

  EXPECT_EQ(copy.size(), 3UL);

  EXPECT_TRUE(copy.contains(1));
  EXPECT_TRUE(copy.contains(2));
  EXPECT_TRUE(copy.contains(3));

  EXPECT_FALSE(copy.contains(4));
  EXPECT_FALSE(copy.contains(5));
  EXPECT_FALSE(copy.contains(6));
  EXPECT_FALSE(copy.contains(7));
  EXPECT_FALSE(copy.contains(8));
  EXPECT_FALSE(copy.contains(9));
}

TEST(Set, BeginEnd) {
  s21::set<int> mySet = {1, 2, 3, 4, 5};

  auto it = mySet.begin();
  int expectedValue = 1;

  for (; it != mySet.end(); ++it, ++expectedValue) {
    EXPECT_EQ(*it, expectedValue);
  }

  EXPECT_EQ(expectedValue, 6);
}

TEST(Set, Empty) {
  s21::set<int> emptySet;
  s21::set<int> nonEmptySet = {1, 2, 3};

  EXPECT_TRUE(emptySet.empty());

  EXPECT_FALSE(nonEmptySet.empty());

  EXPECT_EQ(emptySet.size(), 0UL);
  EXPECT_EQ(nonEmptySet.size(), 3UL);
}

TEST(Set, Size) {
  s21::set<int> emptySet;
  s21::set<double> setOfSize5 = {0, 1, 2, 3, 4};
  s21::set<int> setOfSize10 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  EXPECT_EQ(emptySet.size(), 0UL);

  EXPECT_EQ(setOfSize5.size(), 5UL);

  EXPECT_EQ(setOfSize10.size(), 10UL);
}

TEST(Set, MaxSize) {
  s21::set<int> mySet;
  EXPECT_EQ(mySet.size(), 0UL);
  mySet.insert(1);
  mySet.insert(2);
  mySet.insert(3);
  EXPECT_EQ(mySet.size(), 3UL);
  size_t maxSize = mySet.max_size();
  EXPECT_GE(maxSize, mySet.size());
}

TEST(Set, Insert) {
  s21::set<int> mySet;

  auto result = mySet.insert(42);
  EXPECT_TRUE(result.second);

  result = mySet.insert(42);
  EXPECT_FALSE(result.second);

  result = mySet.insert(10);
  EXPECT_TRUE(result.second);

  result = mySet.insert(20);
  EXPECT_TRUE(result.second);
}

TEST(Set, Erase) {
  s21::set<int> mySet = {1, 2, 3, 4};

  auto it = mySet.find(2);
  if (it != mySet.end()) {
    mySet.erase(it);
  }

  EXPECT_TRUE(mySet.find(1) != mySet.end());
  EXPECT_TRUE(mySet.find(3) != mySet.end());
  EXPECT_TRUE(mySet.find(4) != mySet.end());
  EXPECT_TRUE(mySet.find(2) == mySet.end());
}

TEST(Set, Swap) {
  s21::set<int> set1 = {1, 2, 3};
  s21::set<int> set2 = {4, 5, 6};

  set1.swap(set2);

  EXPECT_TRUE(set1.find(4) != set1.end());
  EXPECT_TRUE(set1.find(5) != set1.end());
  EXPECT_TRUE(set1.find(6) != set1.end());
  EXPECT_TRUE(set1.find(1) == set1.end());
  EXPECT_TRUE(set1.find(2) == set1.end());
  EXPECT_TRUE(set1.find(3) == set1.end());

  EXPECT_TRUE(set2.find(1) != set2.end());
  EXPECT_TRUE(set2.find(2) != set2.end());
  EXPECT_TRUE(set2.find(3) != set2.end());
  EXPECT_TRUE(set2.find(4) == set2.end());
  EXPECT_TRUE(set2.find(5) == set2.end());
  EXPECT_TRUE(set2.find(6) == set2.end());
}

TEST(Set, Merge) {
  s21::set<int> set1 = {1, 2, 3};
  s21::set<int> set2 = {3, 4, 5};

  set1.merge(set2);

  EXPECT_TRUE(set1.find(1) != set1.end());
  EXPECT_TRUE(set1.find(2) != set1.end());
  EXPECT_TRUE(set1.find(3) != set1.end());
  EXPECT_TRUE(set1.find(4) != set1.end());
  EXPECT_TRUE(set1.find(5) != set1.end());

  EXPECT_TRUE(set2.find(3) == set2.end());
  EXPECT_TRUE(set2.find(4) == set2.end());
  EXPECT_TRUE(set2.find(5) == set2.end());

  EXPECT_FALSE(set1.empty());

  EXPECT_TRUE(set2.empty());
}

TEST(Set, Find) {
  s21::set<int> mySet = {1, 2, 3, 4, 5};

  auto found1 = mySet.find(3);
  EXPECT_TRUE(found1 != mySet.end());
  EXPECT_EQ(*found1, 3);

  auto found2 = mySet.find(6);
  EXPECT_TRUE(found2 == mySet.end());
}

TEST(Set, Contains) {
  s21::set<int> mySet = {1, 2, 3, 4, 5};

  EXPECT_TRUE(mySet.contains(3));

  EXPECT_FALSE(mySet.contains(6));

  EXPECT_TRUE(mySet.contains(1));
  EXPECT_TRUE(mySet.contains(5));
}

TEST(Set, InsertMany) {
  s21::set<int> test({1, 2, 3, 3, 3, 4, 5});

  auto result = test.insert_many(3, 6, 5, 3, 4, 7, 8, 8, 9, 10);

  EXPECT_EQ(test.size(), 10UL);

  EXPECT_FALSE(result[0].second);
  EXPECT_TRUE(result[1].second);
  EXPECT_FALSE(result[2].second);
  EXPECT_FALSE(result[3].second);
  EXPECT_FALSE(result[4].second);
  EXPECT_TRUE(result[5].second);
  EXPECT_TRUE(result[6].second);
  EXPECT_FALSE(result[7].second);
  EXPECT_TRUE(result[8].second);
  EXPECT_TRUE(result[9].second);

  auto iter = test.end();
  for (int i = test.size(); i > 0; --i) {
    --iter;
    EXPECT_EQ(*iter, i);
  }
}