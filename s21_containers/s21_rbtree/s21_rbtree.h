#ifndef S21_RBTREE_HPP
#define S21_RBTREE_HPP

#include <cstddef>
#include <iostream>
#include <limits>
#include <utility>

namespace s21 {
enum class Color { kRed, kBlack };

template <typename Key, typename T, bool unique_values = false>
class RBTree {
 public:
  class Node;
  template <typename ret_value>
  class RBTreeTempIterator;
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = RBTreeTempIterator<reference>;
  using const_iterator = RBTreeTempIterator<const_reference>;
  using size_type = size_t;

  RBTree();
  RBTree(std::initializer_list<value_type> const &);
  RBTree(const RBTree &);
  RBTree &operator=(const RBTree &);
  RBTree(RBTree &&) noexcept;
  RBTree &operator=(RBTree &&) noexcept;
  ~RBTree();

  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  const_iterator cbegin() const noexcept;
  const_iterator cend() const noexcept;

  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;

  std::pair<iterator, bool> insert(const value_type &);
  void erase(iterator);
  iterator find(key_type) noexcept;
  bool contains(key_type) noexcept;
  iterator lower_bound(const Key &) noexcept;
  iterator upper_bound(const Key &) noexcept;
  void merge(RBTree &) noexcept;
  void clear() noexcept;
  void swap(RBTree &) noexcept;

 private:
  void initNullNode() noexcept;
  void destroyTree(Node *node) noexcept;
  void copyTree(const RBTree &other);
  Node *copyNodes(Node *src_node, Node *parent);
  void rotateLeft(Node *node) noexcept;
  void rotateRight(Node *node) noexcept;
  std::pair<Node *, bool> insertNodeDirectly(Node *root,
                                             Node *new_node) noexcept;
  void mergeTreeUnique(RBTree &other) noexcept;
  Node *extractNode(iterator pos);
  void balanceAfterInsert(Node *node) noexcept;
  void balanceAfterRemove(Node *node) noexcept;
  Node *searchMin(Node *node) noexcept;
  Node *searchMax(Node *node) noexcept;
  void setMinMax(Node *node) noexcept;
  void swapNodesValues(Node *n1, Node *n2) noexcept;

 private:
  Node *nullNode_ = nullptr;
  Node *root_ = nullptr;
  size_type size_ = 0;
};

template <typename Key, typename T, bool unique_values>
RBTree<Key, T, unique_values>::RBTree() : nullNode_(new Node) {}

template <typename Key, typename T, bool unique_values>
RBTree<Key, T, unique_values>::RBTree(
    std::initializer_list<value_type> const &items)
    : nullNode_(new Node) {
  for (auto item : items) {
    insert(item);
  }
}

template <typename Key, typename T, bool unique_values>
RBTree<Key, T, unique_values>::RBTree(const RBTree &other)
    : nullNode_(new Node) {
  if (other.root_ != nullptr) {
    copyTree(other);
  }
}

template <typename Key, typename T, bool unique_values>
RBTree<Key, T, unique_values> &RBTree<Key, T, unique_values>::operator=(
    const RBTree &other) {
  if (this == &other) {
    return *this;
  }
  if (other.root_ != nullptr) {
    copyTree(other);
  } else {
    clear();
  }
  return *this;
}

template <typename Key, typename T, bool unique_values>
RBTree<Key, T, unique_values>::RBTree(RBTree &&other) noexcept {
  swap(other);
}

template <typename Key, typename T, bool unique_values>
RBTree<Key, T, unique_values> &RBTree<Key, T, unique_values>::operator=(
    RBTree &&other) noexcept {
  if (this == &other) {
    return *this;
  }
  clear();
  swap(other);
  return *this;
}

template <typename Key, typename T, bool unique_values>
RBTree<Key, T, unique_values>::~RBTree() {
  clear();
  delete nullNode_;
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::iterator
RBTree<Key, T, unique_values>::begin() noexcept {
  return iterator(nullNode_->left);
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::iterator
RBTree<Key, T, unique_values>::end() noexcept {
  return iterator(nullNode_);
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::const_iterator
RBTree<Key, T, unique_values>::begin() const noexcept {
  return iterator(nullNode_->left);
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::const_iterator
RBTree<Key, T, unique_values>::end() const noexcept {
  return iterator(nullNode_);
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::const_iterator
RBTree<Key, T, unique_values>::cbegin() const noexcept {
  return const_iterator(nullNode_->left);
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::const_iterator
RBTree<Key, T, unique_values>::cend() const noexcept {
  return const_iterator(nullNode_);
}

template <typename Key, typename T, bool unique_values>
bool RBTree<Key, T, unique_values>::empty() const noexcept {
  return root_ == nullptr;
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::size_type
RBTree<Key, T, unique_values>::size() const noexcept {
  return size_;
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::size_type
RBTree<Key, T, unique_values>::max_size() const noexcept {
  return std::numeric_limits<size_type>::max() / sizeof(Node);
}

template <typename Key, typename T, bool unique_values>
std::pair<typename RBTree<Key, T, unique_values>::iterator, bool>
RBTree<Key, T, unique_values>::insert(const value_type &value) {
  Node *new_node = new Node(value);
  auto result = insertNodeDirectly(root_, new_node);
  if (result.second) {
    setMinMax(result.first);
    ++size_;
    balanceAfterInsert(result.first);
  } else {
    delete new_node;
  }
  return std::make_pair(iterator(result.first), result.second);
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::erase(iterator pos) {
  Node *delete_node = extractNode(pos);
  if (delete_node == root_) {
    clear();
    return;
  }
  delete delete_node;
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::iterator
RBTree<Key, T, unique_values>::find(Key key) noexcept {
  Node *current = root_;
  while (current) {
    if (key == current->data.first) {
      return iterator(current);
    } else if (key < current->data.first) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  return end();
}

template <typename Key, typename T, bool unique_values>
bool RBTree<Key, T, unique_values>::contains(Key key) noexcept {
  return find(key) != end();
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::iterator
RBTree<Key, T, unique_values>::lower_bound(const Key &key) noexcept {
  Node *search = root_;
  Node *result = nullptr;

  while (search != nullptr) {
    if (search->data.first >= key) {
      result = search;
      search = search->left;
    } else {
      search = search->right;
    }
  }
  return (result == nullptr ? end() : iterator(result));
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::iterator
RBTree<Key, T, unique_values>::upper_bound(const Key &key) noexcept {
  Node *search = root_;
  Node *result = nullptr;

  while (search != nullptr) {
    if (search->data.first > key) {
      result = search;
      search = search->left;
    } else {
      search = search->right;
    }
  }
  return (result == nullptr ? end() : iterator(result));
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::merge(RBTree &other) noexcept {
  if constexpr (unique_values) {
    mergeTreeUnique(other);
    return;
  }

  if (this == &other) {
    return;
  }
  iterator iter = other.begin();
  while (other.size_ != 0) {
    Node *move_node = iter.current_;
    ++iter;
    if (move_node->left != nullptr) {
      move_node->left->parent = move_node->parent;
    }
    if (move_node->right != nullptr) {
      move_node->right->parent = move_node->parent;
    }
    if (move_node->parent->left == move_node) {
      move_node->parent->left = nullptr;
    }
    if (move_node->parent->right == move_node) {
      move_node->parent->right = nullptr;
    }
    move_node->ClearPointers();
    --other.size_;
    auto result = insertNodeDirectly(root_, move_node);
    setMinMax(result.first);
    ++size_;
    balanceAfterInsert(result.first);
  }
  other.initNullNode();
  other.root_ = nullptr;
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::mergeTreeUnique(RBTree &other) noexcept {
  if (this == &other) {
    return;
  }
  iterator iter = other.begin();
  while (iter != other.end()) {
    iterator possible = find((*iter).first);
    if (possible == end()) {
      iterator move_iter = iter;
      ++iter;
      Node *move_node = other.extractNode(move_iter);
      if (move_node == other.root_) {
        other.initNullNode();
        other.size_ = 0;
        other.root_ = nullptr;
      }
      auto result = insertNodeDirectly(root_, move_node);
      setMinMax(result.first);
      ++size_;
      balanceAfterInsert(result.first);
    } else {
      ++iter;
    }
  }
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::clear() noexcept {
  destroyTree(root_);
  if (nullNode_ != nullptr) {
    initNullNode();
  }
  root_ = nullptr;
  size_ = 0;
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::swap(RBTree &other) noexcept {
  std::swap(nullNode_, other.nullNode_);
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::initNullNode() noexcept {
  nullNode_->parent = nullptr;
  nullNode_->left = nullNode_;
  nullNode_->right = nullNode_;
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::destroyTree(Node *node) noexcept {
  if (node == nullptr) {
    return;
  }
  destroyTree(node->left);
  destroyTree(node->right);
  delete node;
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::copyTree(const RBTree &other) {
  Node *tmp = copyNodes(other.root_, nullptr);
  clear();
  root_ = tmp;
  size_ = other.size_;
  root_->parent = nullNode_;
  nullNode_->parent = root_;
  nullNode_->left = searchMin(root_);
  nullNode_->right = searchMax(root_);
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::Node *
RBTree<Key, T, unique_values>::copyNodes(Node *src_node, Node *parent) {
  if (!src_node) {
    return nullptr;
  }
  Node *new_node = new Node(src_node->data);
  new_node->parent = parent;
  new_node->color = src_node->color;
  new_node->left = copyNodes(src_node->left, new_node);
  new_node->right = copyNodes(src_node->right, new_node);

  return new_node;
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::rotateLeft(Node *node) noexcept {
  if (node == nullptr || node->right == nullptr) {
    return;
  }
  Node *pivot = node->right;
  node->right = pivot->left;
  if (pivot->left != nullptr) {
    pivot->left->parent = node;
  }
  pivot->parent = node->parent;
  if (node == root_) {
    root_ = pivot;
    nullNode_->parent = root_;
  } else if (node == node->parent->left) {
    node->parent->left = pivot;
  } else {
    node->parent->right = pivot;
  }
  pivot->left = node;
  node->parent = pivot;
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::rotateRight(Node *node) noexcept {
  if (node == nullptr || node->left == nullptr) {
    return;
  }
  Node *pivot = node->left;
  node->left = pivot->right;
  if (pivot->right != nullptr) {
    pivot->right->parent = node;
  }
  pivot->parent = node->parent;
  if (node == root_) {
    root_ = pivot;
    nullNode_->parent = root_;
  } else if (node == node->parent->left) {
    node->parent->left = pivot;
  } else {
    node->parent->right = pivot;
  }
  pivot->right = node;
  node->parent = pivot;
}

template <typename Key, typename T, bool unique_values>
std::pair<typename RBTree<Key, T, unique_values>::Node *, bool>
RBTree<Key, T, unique_values>::insertNodeDirectly(Node *root,
                                                  Node *new_node) noexcept {
  Node *current = root;
  Node *parent = nullptr;

  while (current) {
    parent = current;
    if (new_node->data.first < current->data.first) {
      current = current->left;
    } else if (!unique_values || new_node->data.first > current->data.first) {
      current = current->right;
    } else {
      return std::make_pair(current, false);
    }
  }
  if (parent == nullptr) {
    root_ = new_node;
  } else {
    if (new_node->data.first < parent->data.first) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }
    new_node->parent = parent;
  }
  return std::make_pair(new_node, true);
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::Node *
RBTree<Key, T, unique_values>::extractNode(iterator pos) {
  if (pos == end()) {
    return nullptr;
  }
  Node *delete_node = pos.current_;
  if (delete_node->left != nullptr && delete_node->right != nullptr) {
    Node *replace_node = searchMax(delete_node->left);
    swapNodesValues(delete_node, replace_node);
  }
  if (delete_node->color == Color::kBlack &&
      ((delete_node->left == nullptr) != (delete_node->right == nullptr))) {
    Node *replace_node;
    if (delete_node->left != nullptr) {
      replace_node = delete_node->left;
    } else {
      replace_node = delete_node->right;
    }
    swapNodesValues(delete_node, replace_node);
  }
  if (delete_node->color == Color::kBlack && delete_node->left == nullptr &&
      delete_node->right == nullptr) {
    balanceAfterRemove(delete_node);
  }
  if (delete_node == root_) {
    return delete_node;
  } else {
    if (delete_node == delete_node->parent->left) {
      delete_node->parent->left = nullptr;
    } else {
      delete_node->parent->right = nullptr;
    }
    if (delete_node == nullNode_->left) {
      nullNode_->left = searchMin(root_);
    }
    if (delete_node == nullNode_->right) {
      nullNode_->right = searchMax(root_);
    }
  }
  --size_;
  delete_node->ClearPointers();
  return delete_node;
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::balanceAfterInsert(Node *node) noexcept {
  if (node->parent == nullptr) {
    node->color = Color::kBlack;
    root_ = node;
    root_->parent = nullNode_;
    nullNode_->parent = root_;
    nullNode_->left = root_;
    nullNode_->right = root_;
    return;
  }
  while (node != root_ && node->parent->color == Color::kRed) {
    Node *parent = node->parent;
    Node *grandparent = parent->parent;
    Node *pibling =
        (parent == grandparent->left) ? grandparent->right : grandparent->left;
    if (pibling != nullptr && pibling->color == Color::kRed) {
      parent->color = Color::kBlack;
      pibling->color = Color::kBlack;
      grandparent->color = Color::kRed;
      node = grandparent;
    } else {
      if (node == parent->right && parent == grandparent->left) {
        rotateLeft(parent);
        std::swap(node, parent);
      } else if (node == parent->left && parent == grandparent->right) {
        rotateRight(parent);
        std::swap(node, parent);
      }
      if (node == parent->left) {
        rotateRight(grandparent);
      } else {
        rotateLeft(grandparent);
      }
      parent->color = Color::kBlack;
      grandparent->color = Color::kRed;
    }
  }
  root_->color = Color::kBlack;
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::balanceAfterRemove(Node *node) noexcept {
  Node *parent = node->parent;
  while (node != root_ && (node == nullptr || node->color == Color::kBlack)) {
    if (node == parent->left) {
      Node *sibling = parent->right;
      if (sibling->color == Color::kRed) {
        parent->color = Color::kRed;
        sibling->color = Color::kBlack;
        rotateLeft(parent);
        sibling = parent->right;
      }

      if (sibling->color == Color::kBlack &&
          (sibling->left == nullptr || sibling->left->color == Color::kBlack) &&
          (sibling->right == nullptr ||
           sibling->right->color == Color::kBlack)) {
        sibling->color = Color::kRed;
        if (parent->color == Color::kRed) {
          parent->color = Color::kBlack;
          break;
        }
        node = parent;
        parent = node->parent;
      } else {
        if (sibling->left != nullptr && sibling->left->color == Color::kRed &&
            (sibling->right == nullptr ||
             sibling->right->color == Color::kBlack)) {
          sibling->color = Color::kRed;
          sibling->left->color = Color::kBlack;
          rotateRight(sibling);
          sibling = parent->right;
        }

        sibling->color = parent->color;
        parent->color = Color::kBlack;
        sibling->right->color = Color::kBlack;
        rotateLeft(parent);
        break;
      }
    } else {
      Node *sibling = parent->left;
      if (sibling->color == Color::kRed) {
        parent->color = Color::kRed;
        sibling->color = Color::kBlack;
        rotateRight(parent);
        sibling = parent->left;
      }

      if (sibling->color == Color::kBlack &&
          (sibling->left == nullptr || sibling->left->color == Color::kBlack) &&
          (sibling->right == nullptr ||
           sibling->right->color == Color::kBlack)) {
        sibling->color = Color::kRed;
        if (parent->color == Color::kRed) {
          parent->color = Color::kBlack;
          break;
        }
        node = parent;
        parent = node->parent;
      } else {
        if (sibling->right != nullptr && sibling->right->color == Color::kRed &&
            (sibling->left == nullptr ||
             sibling->left->color == Color::kBlack)) {
          sibling->color = Color::kRed;
          sibling->right->color = Color::kBlack;
          rotateLeft(sibling);
          sibling = parent->left;
        }

        sibling->color = parent->color;
        parent->color = Color::kBlack;
        sibling->left->color = Color::kBlack;
        rotateRight(parent);
        break;
      }
    }
  }
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::Node *
RBTree<Key, T, unique_values>::searchMin(Node *node) noexcept {
  while (node->left) {
    node = node->left;
  }
  return node;
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::Node *
RBTree<Key, T, unique_values>::searchMax(Node *node) noexcept {
  while (node->right) {
    node = node->right;
  }
  return node;
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::setMinMax(Node *node) noexcept {
  if (node->data.first < nullNode_->left->data.first) {
    nullNode_->left = node;
  }
  if (node->data.first >= nullNode_->right->data.first) {
    nullNode_->right = node;
  }
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::swapNodesValues(Node *n1,
                                                    Node *n2) noexcept {
  if (n2->parent->left == n2) {
    n2->parent->left = n1;
  } else {
    n2->parent->right = n1;
  }
  if (n1 == root_) {
    root_ = n2;
    nullNode_->parent = root_;
  } else {
    if (n1->parent->left == n1) {
      n1->parent->left = n2;
    } else {
      n1->parent->right = n2;
    }
  }
  std::swap(n1->parent, n2->parent);
  std::swap(n1->left, n2->left);
  std::swap(n1->right, n2->right);
  std::swap(n1->color, n2->color);
  if (n1->left) {
    n1->left->parent = n1;
  }
  if (n1->right) {
    n1->right->parent = n1;
  }
  if (n2->left) {
    n2->left->parent = n2;
  }
  if (n2->right) {
    n2->right->parent = n2;
  }
}

template <typename Key, typename T, bool unique_values>
class RBTree<Key, T, unique_values>::Node {
 public:
  value_type data;
  Node *parent = nullptr;
  Node *left;
  Node *right;
  Color color = Color::kRed;

  Node() : left(this), right(this) {}
  explicit Node(value_type value)
      : data(value), left(nullptr), right(nullptr) {}

  Node *NextNode() const noexcept;
  Node *PrevNode() const noexcept;
  void ClearPointers() noexcept;
};

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::Node *
RBTree<Key, T, unique_values>::Node::NextNode() const noexcept {
  Node *node = const_cast<Node *>(this);
  if (node->color == Color::kRed &&
      (node->parent == nullptr || node->parent->parent == node)) {
    node = node->left;
  } else if (node->right != nullptr) {
    node = node->right;
    while (node->left != nullptr) {
      node = node->left;
    }
  } else {
    Node *parent = node->parent;
    while (node == parent->right) {
      node = parent;
      parent = parent->parent;
    }
    if (node->right != parent) {
      node = parent;
    }
  }
  return node;
}

template <typename Key, typename T, bool unique_values>
typename RBTree<Key, T, unique_values>::Node *
RBTree<Key, T, unique_values>::Node::PrevNode() const noexcept {
  Node *node = const_cast<Node *>(this);
  if (node->color == Color::kRed &&
      (node->parent == nullptr || node->parent->parent == node)) {
    node = node->right;
  } else if (node->left != nullptr) {
    node = node->left;
    while (node->right != nullptr) {
      node = node->right;
    }
  } else {
    Node *parent = node->parent;
    while (node == parent->left) {
      node = parent;
      parent = parent->parent;
    }
    if (node->left != parent) {
      node = parent;
    }
  }
  return node;
}

template <typename Key, typename T, bool unique_values>
void RBTree<Key, T, unique_values>::Node::ClearPointers() noexcept {
  left = nullptr;
  right = nullptr;
  parent = nullptr;
  color = Color::kRed;
}

template <typename Key, typename T, bool unique_values>
template <typename ret_value>
class RBTree<Key, T, unique_values>::RBTreeTempIterator {
 public:
  template <typename>
  friend class RBTreeTempIterator;
  friend class RBTree<Key, T, unique_values>;

  RBTreeTempIterator() = default;
  RBTreeTempIterator(const RBTreeTempIterator &other) noexcept = default;

  explicit RBTreeTempIterator(Node *node) : current_(node){};
  RBTreeTempIterator &operator=(const RBTreeTempIterator &other) noexcept =
      default;
  template <typename U>
  RBTreeTempIterator(const RBTreeTempIterator<U> &it)
      : current_{it.current_} {};

  ret_value operator*() const;
  RBTreeTempIterator &operator++();
  RBTreeTempIterator operator++(int);
  RBTreeTempIterator &operator--();
  RBTreeTempIterator operator--(int);
  bool operator==(const RBTreeTempIterator &other) const noexcept;
  bool operator!=(const RBTreeTempIterator &other) const noexcept;

 protected:
  Node *current_;
};

template <typename Key, typename T, bool unique_values>
template <typename ret_value>
ret_value RBTree<Key, T, unique_values>::RBTreeTempIterator<
    ret_value>::operator*() const {
  return current_->data;
}

template <typename Key, typename T, bool unique_values>
template <typename ret_value>
typename RBTree<Key, T, unique_values>::template RBTreeTempIterator<ret_value> &
RBTree<Key, T, unique_values>::RBTreeTempIterator<ret_value>::operator++() {
  current_ = current_->NextNode();
  return *this;
}

template <typename Key, typename T, bool unique_values>
template <typename ret_value>
typename RBTree<Key, T, unique_values>::template RBTreeTempIterator<ret_value>
RBTree<Key, T, unique_values>::RBTreeTempIterator<ret_value>::operator++(int) {
  iterator tmp(current_);
  ++(*this);
  return tmp;
}

template <typename Key, typename T, bool unique_values>
template <typename ret_value>
typename RBTree<Key, T, unique_values>::template RBTreeTempIterator<ret_value> &
RBTree<Key, T, unique_values>::RBTreeTempIterator<ret_value>::operator--() {
  current_ = current_->PrevNode();
  return *this;
}

template <typename Key, typename T, bool unique_values>
template <typename ret_value>
typename RBTree<Key, T, unique_values>::template RBTreeTempIterator<ret_value>
RBTree<Key, T, unique_values>::RBTreeTempIterator<ret_value>::operator--(int) {
  iterator tmp({current_});
  --(*this);
  return tmp;
}

template <typename Key, typename T, bool unique_values>
template <typename ret_value>
bool RBTree<Key, T, unique_values>::RBTreeTempIterator<ret_value>::operator==(
    const RBTreeTempIterator &other) const noexcept {
  return current_ == other.current_;
}

template <typename Key, typename T, bool unique_values>
template <typename ret_value>
bool RBTree<Key, T, unique_values>::RBTreeTempIterator<ret_value>::operator!=(
    const RBTreeTempIterator &other) const noexcept {
  return current_ != other.current_;
}
}  // namespace s21

#endif