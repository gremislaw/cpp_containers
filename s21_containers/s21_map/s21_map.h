#ifndef S21_MAP_HPP
#define S21_MAP_HPP

#include <stdbool.h>

#include <iostream>
#include <limits>
#include <utility>

namespace s21 {
template <typename Key, typename T>
class map {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

  class node {
    friend class const_iterator;
    friend class iterator;
    friend class map<Key, T>;

    std::pair<const Key, T> elem;
    node* left_node;
    node* right_node;
    node* parent_node;

   public:
    node()
        : elem(std::make_pair<const Key, T>(0, 0)),
          left_node(nullptr),
          right_node(nullptr),
          parent_node(nullptr) {}
    node(const std::pair<const Key, T> new_elem)
        : elem(new_elem),
          left_node(nullptr),
          right_node(nullptr),
          parent_node(nullptr) {}
    node(Key k, T v)
        : elem(std::make_pair(k, v)),
          left_node(nullptr),
          right_node(nullptr),
          parent_node(nullptr) {}
    node(Key k, T v, node* lptr, node* rptr, node* pptr)
        : elem(k, v), left_node(lptr), right_node(rptr), parent_node(pptr) {}
    node& operator=(node&& other) {
      left_node = other.left_node;
      right_node = other.right_node;
      parent_node = other.parent_node;
      other.left_node = nullptr;
      other.right_node = nullptr;
      other.parent_node = nullptr;
      elem = std::move(other.elem);
    }
    ~node() = default;

   private:
    node* find_ceil_down() {
      node* rt = this->right_node;
      while (rt->left_node) rt = rt->left_node;
      return rt;
    }

    node* find_ceil_up() {
      node* rt = this;
      node* res = this;
      while (rt->parent_node && (rt->parent_node->right_node == rt))
        rt = rt->parent_node;
      if (rt->parent_node) res = rt->parent_node;
      return res;
    }

    node* find_floor_down() {
      node* rt = this->left_node;
      while (rt->right_node) rt = right_node;
      return rt;
    }

    node* find_floor_up() {
      node* rt = this;
      node* res = this;
      while (rt->parent_node && (rt->parent_node->left_node == rt))
        rt = rt->parent_node;
      if (rt->parent_node) res = rt->parent_node;
      return res;
    }
  };

  class const_iterator {
   public:
    const_iterator() : ptr(nullptr), stub(nullptr) {}
    const_iterator(node* tmp, node* st) : ptr(tmp), stub(st) {}
    const_iterator(const_iterator& other) : ptr(other.ptr), stub(other.stub) {}
    const_iterator& operator=(const_iterator& other) {
      ptr = other.ptr;
      stub = other.stub;
      return *this;
    }
    ~const_iterator() = default;

    bool operator==(const const_iterator& other) {
      return this->ptr == other.ptr;
    }

    bool operator!=(const const_iterator& other) {
      return this->ptr != other.ptr;
    }

    const_iterator& operator++() {
      increment();
      return *this;
    }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      increment();
      return tmp;
    }

    const_iterator operator+=(size_t n) {
      while (n-- > 0) operator++();
      return *this;
    }

    const_iterator& operator--() {
      decrement();
      return *this;
    }

    const_iterator operator--(int) {
      const_iterator tmp(*this);
      decrement();
      return tmp;
    }

    const_iterator operator-=(size_t n) {
      while (n-- > 0) operator--();
      return *this;
    }

    const std::pair<const Key, T>& operator*() const { return ptr->elem; }

   protected:
    node* ptr;
    node* stub;

    void increment() {
      node* rt = ptr;
      while (rt->parent_node) rt = rt->parent_node;
      while (rt->right_node) rt = rt->right_node;
      if (ptr == rt)
        ptr = stub;
      else {
        if (ptr->right_node)
          ptr = ptr->find_ceil_down();
        else if (ptr->parent_node)
          ptr = ptr->find_ceil_up();
      }
    }

    void decrement() {
      if (ptr->left_node)
        ptr = ptr->find_floor_down();
      else if (ptr->parent_node)
        ptr = ptr->find_floor_up();
    }

    node* get_ptr() { return ptr; }
  };

  class iterator : public const_iterator {
   public:
    iterator() : const_iterator() {}
    iterator(node* tmp, node* st) : const_iterator(tmp, st) {}
    iterator(const iterator& other) : const_iterator() {
      this->ptr = other.ptr;
      this->stub = other.stub;
    }
    iterator(iterator&& other) {
      this->ptr = other.ptr;
      this->stub = other.stub;
      other.ptr = nullptr;
      other.stub = nullptr;
    }
    iterator& operator=(const iterator& other) {
      this->ptr = other.ptr;
      this->stub = other.stub;
      return *this;
    }
    iterator& operator=(iterator&& other) {
      this->ptr = other.ptr;
      this->stub = other.stub;
      other.ptr = nullptr;
      other.stub = nullptr;
      return *this;
    }
    ~iterator() {
      this->ptr = nullptr;
      this->stub = nullptr;
    }

    std::pair<const Key, T>& operator*() { return this->ptr->elem; }

    friend void map::erase(iterator pos);
  };

  map();
  map(std::initializer_list<std::pair<const Key, T>> const& items);
  map(const map& m);
  map(map&& m);
  ~map();
  map& operator=(map&& m);

  T& at(const Key& key);
  T& operator[](const Key& key);

  iterator begin();
  iterator end();
  bool empty();
  size_t size();
  size_t max_size();

  void clear();
  std::pair<iterator, bool> insert(const std::pair<const Key, T>& value);
  std::pair<iterator, bool> insert(const Key& key, const T& obj);
  std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj);
  void erase(iterator pos);
  void swap(map& other);
  void merge(map& other);
  bool contains(const Key& key);

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args&&... args);

  void print();

 private:
  node* root;
  size_t sz;
  node* stub;

  void recursive_insert(node* rt);
  void recursive_clear(node* rt);
  iterator get_iter(const Key& key);
  void tie_nodes(node* rt, node* nrt);
  void print_recursive(node* rt);
};

template <typename Key, typename T>
map<Key, T>::map() : root(nullptr), sz(0) {
  stub = new node();
}

template <typename Key, typename T>
map<Key, T>::map(std::initializer_list<std::pair<const Key, T>> const& items)
    : map() {
  for (std::pair<const Key, T> a : items) insert(a);
}

template <typename Key, typename T>
map<Key, T>::map(const map& m) : map() {
  insert(m.root->elem);
  recursive_insert(m.root);
  sz = m.sz;
}

template <typename Key, typename T>
map<Key, T>::map(map&& m) {
  root = m.root;
  sz = m.sz;
  stub = m.stub;
  m.root = nullptr;
  m.stub = nullptr;
  m.sz = 0;
}

template <typename Key, typename T>
map<Key, T>::~map() {
  if (root) recursive_clear(root);
  if (stub) delete stub;
}

template <typename Key, typename T>
map<Key, T>& map<Key, T>::operator=(map&& m) {
  if (root) recursive_clear(root);
  if (stub) delete stub;
  root = m.root;
  sz = m.sz;
  stub = m.stub;
  m.root = nullptr;
  m.stub = nullptr;
  m.sz = 0;
  return *this;
}

template <typename Key, typename T>
T& map<Key, T>::at(const Key& key) {
  bool cn = true;
  node* rt = root;
  while (cn && sz) {
    if (key > rt->elem.first && rt->right_node)
      rt = rt->right_node;
    else if (key < rt->elem.first && rt->left_node)
      rt = rt->left_node;
    else
      cn = false;
  }
  if (sz && rt->elem.first == key)
    return rt->elem.second;
  else
    throw std::out_of_range("invalid key");
}

template <typename Key, typename T>
T& map<Key, T>::operator[](const Key& key) {
  bool cn = true;
  node* rt = root;
  while (cn && sz) {
    if (key > rt->elem.first && rt->right_node)
      rt = rt->right_node;
    else if (key < rt->elem.first && rt->left_node)
      rt = rt->left_node;
    else
      cn = false;
  }
  if (sz && rt->elem.first == key)
    return rt->elem.second;
  else {
    insert(key, T());
    return operator[](key);
  }
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::begin() {
  if (empty())
    return iterator(stub, stub);
  else {
    node* rt = root;
    while (rt->left_node) rt = rt->left_node;
    return iterator(rt, stub);
  }
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::end() {
  return iterator(stub, stub);
}

template <typename Key, typename T>
bool map<Key, T>::empty() {
  bool res;
  if (sz == 0)
    res = true;
  else
    res = false;
  return res;
}

template <typename Key, typename T>
size_t map<Key, T>::size() {
  return sz;
}

template <typename Key, typename T>
size_t map<Key, T>::max_size() {
  return std::numeric_limits<size_t>::max() / sizeof(map) * 2;
}

template <typename Key, typename T>
void map<Key, T>::clear() {
  if (root) {
    recursive_clear(root);
    root = nullptr;
  }
  delete stub;
  stub = nullptr;
  sz = 0;
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const std::pair<const Key, T>& value) {
  bool res = true;
  node* rt = root;
  node* fn = root;
  if (empty())
    fn = root = new node(value);
  else {
    bool cn = true;
    while (cn) {
      if (value.first > rt->elem.first) {
        if (rt->right_node)
          rt = rt->right_node;
        else {
          fn = rt->right_node = new node(value);
          rt->right_node->parent_node = rt;
          cn = false;
        }
      } else if (value.first < rt->elem.first) {
        if (rt->left_node)
          rt = rt->left_node;
        else {
          fn = rt->left_node = new node(value);
          rt->left_node->parent_node = rt;
          cn = false;
        }
      } else {
        cn = false;
        res = false;
      }
    }
  }
  if (res) ++sz;
  return std::make_pair(iterator(fn, stub), res);
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const Key& key, const T& obj) {
  return insert(std::make_pair(key, obj));
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert_or_assign(
    const Key& key, const T& obj) {
  if (contains(key)) {
    operator[](key) = obj;
    return std::make_pair(get_iter(key), true);
  } else
    return insert(key, obj);
}

template <typename Key, typename T>
void map<Key, T>::erase(iterator pos) {
  node* rt = pos.get_ptr();
  if ((!rt->left_node) && (!rt->right_node))
    tie_nodes(rt, nullptr);
  else if (rt->left_node && rt->right_node) {
    node* ceil = rt->find_ceil_down();
    tie_nodes(ceil, nullptr);
    ceil->left_node = rt->left_node;
    ceil->right_node = rt->right_node;
    tie_nodes(rt, ceil);
  } else if (rt->left_node)
    tie_nodes(rt, rt->left_node);
  else if (rt->right_node)
    tie_nodes(rt, rt->right_node);
  delete rt;
  --sz;
}

template <typename Key, typename T>
void map<Key, T>::swap(map& other) {
  map n = std::move(*this);
  *this = std::move(other);
  other = std::move(n);
}

template <typename Key, typename T>
void map<Key, T>::merge(map& other) {
  for (iterator it = other.begin(); it != other.end(); ++it) {
    if (!contains((*it).first)) insert(*it);
  }
  other.clear();
}

template <typename Key, typename T>
bool map<Key, T>::contains(const Key& key) {
  int res = 0;
  node* rt = root;
  while (res == 0) {
    if (key > rt->elem.first) {
      if (rt->right_node)
        rt = rt->right_node;
      else
        res = 2;
    } else if (key < rt->elem.first) {
      if (rt->left_node)
        rt = rt->left_node;
      else
        res = 2;
    } else
      res = 1;
  }
  if (res == 1)
    return true;
  else
    return false;
}

template <typename Key, typename T>
void map<Key, T>::recursive_insert(node* rt) {
  if (rt->left_node) {
    insert(rt->left_node->elem);
    recursive_insert(rt->left_node);
  }
  if (rt->right_node) {
    insert(rt->right_node->elem);
    recursive_insert(rt->right_node);
  }
  return;
}

template <typename Key, typename T>
void map<Key, T>::recursive_clear(node* rt) {
  if (rt->left_node) recursive_clear(rt->left_node);
  if (rt->right_node) recursive_clear(rt->right_node);
  delete rt;
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::get_iter(const Key& key) {
  node* rt = root;
  bool cn = true;
  while (cn) {
    if (key > rt->elem.first)
      rt = rt->right_node;
    else if (key < rt->elem.first)
      rt = rt->left_node;
    else
      cn = false;
  }
  return iterator(rt, stub);
}

template <typename Key, typename T>
void map<Key, T>::tie_nodes(node* rt, node* nrt) {
  if (rt->parent_node && (rt->parent_node->left_node == rt))
    rt->parent_node->left_node = nrt;
  else if (rt->parent_node && (rt->parent_node->right_node == rt))
    rt->parent_node->right_node = nrt;
  else
    root = nrt;
}

template <typename Key, typename T>
void map<Key, T>::print() {
  print_recursive(root);
  std::cout << "\nsize " << sz << "\nroot = " << root << "\nstub = " << stub
            << '\n';
}

template <typename Key, typename T>
void map<Key, T>::print_recursive(node* rt) {
  if (rt->left_node) print_recursive(rt->left_node);
  std::cout << "{ " << rt->elem.first << ", " << rt->elem.second << " },  ";
  if (rt->right_node) print_recursive(rt->right_node);
}

template <typename Key, typename T>
template <typename... Args>
vector<std::pair<typename map<Key, T>::iterator, bool>>
map<Key, T>::insert_many(Args&&... args) {
  vector<std::pair<Key, T>> tmp{args...};
  vector<std::pair<typename map<Key, T>::iterator, bool>> res;
  for (auto it = tmp.begin(); it != tmp.end(); ++it)
    res.push_back(this->insert(*it));
  return res;
}
}  // namespace s21

#endif