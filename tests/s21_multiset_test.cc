#include "s21_main_test.h"

TEST(Multiset, DefaultConstructor) {
  s21::multiset<int> test;

  EXPECT_TRUE(test.empty());
  EXPECT_EQ(test.size(), 0UL);
  EXPECT_EQ(test.begin(), test.end());
}

TEST(Multiset, CopyConstructor) {
  s21::multiset<int> original = {1, 2, 2, 3, 3, 3};

  s21::multiset<int> copy(original);

  EXPECT_EQ(original.size(), 6UL);
  EXPECT_EQ(copy.size(), 6UL);
  EXPECT_TRUE(copy.contains(1));
  EXPECT_EQ(copy.count(2), 2UL);
  EXPECT_EQ(copy.count(3), 3UL);
  EXPECT_FALSE(copy.contains(4));

  EXPECT_EQ(original.size(), 6UL);
  EXPECT_TRUE(original.contains(1));
}

TEST(Multiset, MoveConstructor) {
  s21::multiset<int> original = {1, 2, 3, 1};

  s21::multiset<int> moved(std::move(original));

  EXPECT_TRUE(original.empty());
  EXPECT_EQ(original.size(), 0UL);

  EXPECT_EQ(moved.size(), 4UL);
  EXPECT_EQ(moved.count(1), 2UL);
  EXPECT_TRUE(moved.contains(2));
  EXPECT_TRUE(moved.contains(3));
  EXPECT_FALSE(moved.contains(4));
}

TEST(Multiset, MoveAssignmentOperator) {
  s21::multiset<int> source;
  source.insert(1);
  source.insert(2);
  source.insert(2);

  s21::multiset<int> destination;
  destination.insert(3);

  destination = std::move(source);

  EXPECT_TRUE(source.empty());
  EXPECT_EQ(source.size(), 0UL);
  EXPECT_EQ(source.begin(), source.end());
  EXPECT_FALSE(source.contains(1));
  EXPECT_FALSE(source.contains(2));

  EXPECT_EQ(destination.size(), 3UL);
  EXPECT_EQ(destination.count(1), 1UL);
  EXPECT_EQ(destination.count(2), 2UL);
  EXPECT_FALSE(destination.contains(3));
}

TEST(Multiset, InsertMethod) {
  s21::multiset<int> test;

  test.insert(1);
  EXPECT_FALSE(test.empty());
  EXPECT_EQ(test.size(), 1UL);

  test.insert(2);
  EXPECT_EQ(test.size(), 2UL);

  test.insert(2);
  EXPECT_EQ(test.size(), 3UL);

  test.insert(2);
  EXPECT_EQ(test.size(), 4UL);

  EXPECT_EQ(test.count(1), 1UL);
  EXPECT_EQ(test.count(2), 3UL);
}

TEST(Multiset, InitializerListConstructor) {
  s21::multiset<int> test_set = {1, 2, 2, 3, 3, 3};

  EXPECT_EQ(test_set.size(), 6UL);

  EXPECT_TRUE(test_set.contains(1));
  EXPECT_EQ(test_set.count(1), 1UL);

  EXPECT_TRUE(test_set.contains(2));
  EXPECT_EQ(test_set.count(2), 2UL);

  EXPECT_TRUE(test_set.contains(3));
  EXPECT_EQ(test_set.count(3), 3UL);

  EXPECT_FALSE(test_set.contains(0));
  EXPECT_FALSE(test_set.contains(4));
}

TEST(Multiset, FindMethod) {
  s21::multiset<int> test_set = {1, 2, 2, 3, 3, 3};

  auto it1 = test_set.find(1);
  auto it2 = test_set.find(2);
  auto it3 = test_set.find(3);

  EXPECT_NE(it1, test_set.end());
  EXPECT_EQ(*it1, 1);

  EXPECT_NE(it2, test_set.end());
  EXPECT_EQ(*it2, 2);

  EXPECT_NE(it3, test_set.end());
  EXPECT_EQ(*it3, 3);

  auto it4 = test_set.find(4);
  EXPECT_EQ(it4, test_set.end());
}

TEST(Multiset, ContainsMethod) {
  s21::multiset<int> test_set = {1, 2, 2, 3, 3, 3};

  EXPECT_TRUE(test_set.contains(1));
  EXPECT_TRUE(test_set.contains(2));
  EXPECT_TRUE(test_set.contains(3));

  EXPECT_FALSE(test_set.contains(0));
  EXPECT_FALSE(test_set.contains(4));

  EXPECT_EQ(test_set.count(1), 1UL);
  EXPECT_EQ(test_set.count(2), 2UL);
  EXPECT_EQ(test_set.count(3), 3UL);
}

TEST(Multiset, BeginMethod) {
  s21::multiset<int> test_set;

  EXPECT_EQ(test_set.begin(), test_set.end());

  test_set.insert(1);
  test_set.insert(2);
  test_set.insert(3);

  int expected_value = 1;
  for (auto it = test_set.begin(); it != test_set.end();
       ++it, ++expected_value) {
    EXPECT_EQ(*it, expected_value);
  }
}

TEST(Multiset, SizeMethod) {
  s21::multiset<int> test_set;

  EXPECT_EQ(test_set.size(), 0UL);

  test_set.insert(1);
  test_set.insert(2);
  test_set.insert(2);
  test_set.insert(3);

  EXPECT_EQ(test_set.size(), 4UL);

  test_set.insert(4);
  EXPECT_EQ(test_set.size(), 5UL);
}

TEST(Multiset, MaxSizeMethod) {
  s21::multiset<int> test_set;

  EXPECT_GE(test_set.max_size(), test_set.size());

  for (int i = 0; i < 1000; ++i) {
    test_set.insert(i);
  }

  EXPECT_GE(test_set.max_size(), test_set.size());

  std::cout << "Max size of the multiset: " << test_set.max_size() << std::endl;
}

TEST(Multiset, ClearMethod) {
  s21::multiset<int> test_set = {1, 2, 2, 3, 3, 3};

  EXPECT_EQ(test_set.size(), 6UL);

  test_set.clear();

  EXPECT_EQ(test_set.size(), 0UL);
  EXPECT_TRUE(test_set.empty());

  test_set.insert(10);
  EXPECT_EQ(test_set.size(), 1UL);
  EXPECT_FALSE(test_set.empty());
  EXPECT_TRUE(test_set.contains(10));
}

TEST(Multiset, EraseMethod) {
  s21::multiset<int> test_set = {1, 2, 2, 3, 3, 3};

  EXPECT_EQ(test_set.size(), 6UL);

  auto it = test_set.find(2);
  test_set.erase(it);

  EXPECT_EQ(test_set.size(), 5UL);

  EXPECT_TRUE(test_set.contains(2));

  it = test_set.find(2);
  test_set.erase(it);
  EXPECT_EQ(test_set.size(), 4UL);
  EXPECT_FALSE(test_set.contains(2));

  while (!test_set.empty()) {
    test_set.erase(test_set.begin());
  }

  EXPECT_TRUE(test_set.empty());
  EXPECT_EQ(test_set.size(), 0UL);
}

TEST(Multiset, SwapMethod) {
  s21::multiset<int> test_set1 = {1, 2, 2, 3, 3, 3};
  s21::multiset<int> test_set2 = {4, 5, 6};

  EXPECT_EQ(test_set1.size(), 6UL);
  EXPECT_EQ(test_set2.size(), 3UL);

  test_set1.swap(test_set2);

  EXPECT_EQ(test_set1.size(), 3UL);
  EXPECT_EQ(test_set2.size(), 6UL);

  EXPECT_TRUE(test_set1.contains(4));
  EXPECT_TRUE(test_set1.contains(5));
  EXPECT_TRUE(test_set1.contains(6));

  EXPECT_TRUE(test_set2.contains(1));
  EXPECT_TRUE(test_set2.contains(2));
  EXPECT_TRUE(test_set2.contains(3));

  s21::multiset<int> empty_set;
  test_set1.swap(empty_set);

  EXPECT_TRUE(test_set1.empty());
  EXPECT_EQ(empty_set.size(), 3UL);
}

TEST(Multiset, MergeMethod) {
  s21::multiset<int> test_set1 = {1, 2, 2, 3, 3, 3};
  s21::multiset<int> test_set2 = {4, 5, 6};

  test_set1.merge(test_set2);

  EXPECT_EQ(test_set1.size(), 9UL);

  EXPECT_TRUE(test_set2.empty());
}

TEST(Multiset, CountMethod) {
  s21::multiset<int> multiset = {1, 2, 2, 3, 2, 4, 4, 4, 5};
  EXPECT_EQ(multiset.count(1), 1UL);
  EXPECT_EQ(multiset.count(2), 3UL);
  EXPECT_EQ(multiset.count(3), 1UL);
  EXPECT_EQ(multiset.count(4), 3UL);
  EXPECT_EQ(multiset.count(5), 1UL);
  EXPECT_EQ(multiset.count(6), 0UL);
}

TEST(Multiset, TestLowerBound) {
  s21::multiset<int> my = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};

  auto lb1 = my.lower_bound(2);
  EXPECT_EQ((*lb1).first, 2);

  auto lb2 = my.lower_bound(5);
  EXPECT_EQ(lb2, my.end());

  auto lb3 = my.lower_bound(0);
  EXPECT_EQ(lb3, my.begin());
}

TEST(Multiset, TestUpperBound) {
  s21::multiset<int> my = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};

  auto ub1 = my.upper_bound(1);
  EXPECT_EQ((*ub1).first, 2);

  auto ub2 = my.upper_bound(5);
  EXPECT_EQ(ub2, my.end());

  auto ub3 = my.upper_bound(0);
  EXPECT_EQ(ub3, my.begin());
}

TEST(Multiset, TestEqualRange) {
  s21::multiset<int> myMultiset = {1, 2, 2, 3, 4, 4, 5};

  auto range1 = myMultiset.equal_range(2);
  EXPECT_EQ(range1.first, myMultiset.lower_bound(2));
  EXPECT_EQ(range1.second, myMultiset.upper_bound(2));

  auto range2 = myMultiset.equal_range(4);
  EXPECT_EQ(range2.first, myMultiset.lower_bound(4));
  EXPECT_EQ(range2.second, myMultiset.upper_bound(4));
}

TEST(Multiset, InsertMany) {
  s21::multiset<int> test({1, 2, 3, 4, 5});
  test.insert_many(1, 2, 3, 4, 5);

  EXPECT_EQ(test.size(), 10UL);

  auto iter = test.end();
  --iter;
  EXPECT_EQ(*iter, 5);

  --iter;
  EXPECT_EQ(*iter, 5);

  --iter;
  EXPECT_EQ(*iter, 4);

  --iter;
  EXPECT_EQ(*iter, 4);
}