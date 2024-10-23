#ifndef S21_LIST_HPP
#define S21_LIST_HPP

#include <stdbool.h>

#include <iostream>
#include <limits>
#include <utility>

namespace s21 {
template <typename T>
class list {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  class node {
    value_type value;
    node* next_node;
    node* prev_node;

   public:
    node() : value(0), next_node(nullptr), prev_node(nullptr) {}
    node(value_type value)
        : value(value), next_node(nullptr), prev_node(nullptr) {}
    friend class list;
  };

  class const_iterator {
   public:
    const_iterator() : ptr(nullptr) {}
    const_iterator(node* tmp) : ptr(tmp) {}
    const_iterator(const_iterator& other) : ptr(other.ptr) {}
    const_iterator& operator=(const_iterator& other) {
      ptr = other.ptr;
      return *this;
    }

    ~const_iterator() = default;

    bool operator!=(const const_iterator& other) const {
      return (ptr != other.ptr);
    }
    bool operator==(const const_iterator& other) const {
      return (ptr == other.ptr);
    }
    const_reference operator*() { return ptr->value; }

    const_iterator operator++(int) {
      const_iterator tmp(*this);
      increment();
      return tmp;
    }

    const_iterator& operator++() {
      increment();
      return *this;
    }

    const_iterator operator--(int) {
      const_iterator tmp(*this);
      decrement();
      return tmp;
    }

    const_iterator& operator--() {
      decrement();
      return *this;
    }

    const_iterator operator+(size_type n) {
      auto it_tmp = *this;
      while (n-- > 0) it_tmp.increment();
      return it_tmp;
    }

    const_iterator operator-(size_type n) {
      auto it_tmp = *this;
      while (n-- > 0) it_tmp.decrement();
      return it_tmp;
    }

    const_iterator& operator+=(size_type n) {
      while (n-- > 0) increment();
      return *this;
    }

    const_iterator& operator-=(size_type n) {
      while (n-- > 0) decrement();
      return *this;
    }

    node* get_iter() { return ptr; }

   protected:
    node* ptr;

    void increment() {
      if (ptr && ptr->next_node) ptr = ptr->next_node;
    }

    void decrement() {
      if (ptr && ptr->prev_node) ptr = ptr->prev_node;
    }
  };

  class iterator : public const_iterator {
   public:
    iterator() : const_iterator() {}
    iterator(node* nd) : const_iterator(nd) {}
    iterator(const iterator& other) : const_iterator() {
      this->ptr = other.ptr;
    }
    ~iterator() = default;
    iterator& operator=(const iterator& other) {
      this->ptr = other.ptr;
      return *this;
    }

    reference operator*() { return this->ptr->value; }
    friend class list;
  };

  list();
  list(size_type n);
  list(std::initializer_list<value_type> const& items);
  list(const list& other);
  list(list&& other);
  ~list();

  list& operator=(const list& other);
  list& operator=(list&& other);

  iterator begin();
  iterator end();

  const_reference front() const;
  const_reference back() const;

  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  void clear();
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);
  void push_back(const_reference data);
  void push_front(const_reference data);
  void pop_front();
  void pop_back();
  void swap(list& other);
  void merge(list& other);
  void splice(iterator pos, list& other);
  void reverse();
  void unique();
  void sort();

  template <typename... Args>
  iterator insert_many(const_iterator cpos, Args&&... args);

  template <typename... Args>
  void insert_many_back(Args&&... args);

  template <typename... Args>
  void insert_many_front(Args&&... args);

  void print();

 private:
  node* head;
  node* tail;
  node* stub;
  size_type sz;
};

template <typename T>
list<T>::list() {
  stub = new node;
  head = tail = stub;
  sz = 0;
}

template <typename T>
list<T>::list(size_t n) : list() {
  if (n < 0) throw std::out_of_range("Index out of range");
  while (n-- > 0) push_back(T());
}

template <typename T>
list<T>::list(std::initializer_list<T> const& items) : list() {
  for (T el : items) push_back(el);
}

template <typename T>
list<T>::list(const list& other) : list() {
  *this = other;
}

template <typename T>
list<T>::list(list&& other)
    : head(other.head), tail(other.tail), stub(other.stub), sz(other.sz) {
  if (this != &other) {
    other.sz = 0;
    other.head = nullptr;
    other.tail = nullptr;
    other.stub = nullptr;
  }
}

template <typename T>
list<T>::~list() {
  clear();
}

template <typename T>
list<T>& list<T>::operator=(const list& other) {
  if (this != &other) {
    clear();
    stub = new node;
    head = tail = stub;
    node* tmp_node = other.head;
    while (tmp_node != other.stub) {
      push_back(tmp_node->value);
      tmp_node = tmp_node->next_node;
    }
  }
  return *this;
}

template <typename T>
void list<T>::push_front(const T& data) {
  node* new_ptr = new node(data);
  if (head != stub) {
    head->prev_node = new_ptr;
    new_ptr->next_node = head;
    head = new_ptr;
  } else {
    head = new_ptr;
    tail = new_ptr;
    tail->next_node = stub;
    stub->prev_node = tail;
  }
  ++sz;
  return;
}

template <typename T>
void list<T>::push_back(const T& data) {
  node* new_ptr = new node(data);
  if (tail != stub) {
    tail->next_node = new_ptr;
    new_ptr->prev_node = tail;
    tail = new_ptr;
  } else {
    head = new_ptr;
    tail = new_ptr;
  }
  tail->next_node = stub;
  stub->prev_node = tail;
  ++sz;
  return;
}

template <typename T>
void list<T>::pop_front() {
  node* prev_head = head;
  if (head != stub) {
    head = head->next_node;
    head->prev_node = nullptr;
    if (tail == prev_head) tail = stub;
    delete prev_head;
    --sz;
  }
}

template <typename T>
void list<T>::pop_back() {
  node* prev_tail = tail;
  if (tail != stub) {
    if (tail->prev_node) {
      tail = tail->prev_node;
      tail->next_node = stub;
      stub->prev_node = tail;
    } else {
      tail = stub;
      head = stub;
    }
    delete prev_tail;
    --sz;
  }
}

template <typename T>
typename list<T>::iterator list<T>::begin() {
  return iterator(head);
}

template <typename T>
typename list<T>::iterator list<T>::end() {
  return iterator(stub);
}

template <typename T>
const T& list<T>::front() const {
  return head->value;
}

template <typename T>
const T& list<T>::back() const {
  return tail->value;
}

template <typename T>
bool list<T>::empty() const {
  return (sz == 0);
}

template <typename T>
size_t list<T>::size() const {
  return sz;
}

template <typename T>
size_t list<T>::max_size() const {
  return (std::numeric_limits<std::size_t>::max() / sizeof(node) / 2);
}

template <typename T>
void list<T>::clear() {
  while (sz != 0) pop_front();
  delete stub;
  stub = nullptr;
}

template <typename T>
typename list<T>::iterator list<T>::insert(iterator pos, const T& value) {
  node* tmp = pos.ptr;
  node* new_ptr = nullptr;
  if (tmp) {
    if (tmp == stub) {
      push_back(value);
      new_ptr = stub->prev_node;
    } else if (tmp == head) {
      push_front(value);
      new_ptr = head;
    } else {
      new_ptr = new node(value);
      new_ptr->next_node = tmp;
      new_ptr->prev_node = tmp->prev_node;
      tmp->prev_node->next_node = new_ptr;
      tmp->prev_node = new_ptr;
      ++sz;
    }
  }
  return iterator(new_ptr);
}

template <typename T>
void list<T>::erase(iterator pos) {
  node* tmp = pos.ptr;
  if (tmp && (tmp != stub)) {
    if (tmp == head)
      pop_front();
    else if (tmp == tail)
      pop_back();
    else {
      tmp->prev_node->next_node = tmp->next_node;
      tmp->next_node->prev_node = tmp->prev_node;
      --sz;
      delete tmp;
    }
  }
  return;
}

template <typename T>
void list<T>::swap(list& other) {
  std::swap(sz, other.sz);
  std::swap(head, other.head);
  std::swap(tail, other.tail);
  std::swap(stub, other.stub);
}

template <typename T>
void list<T>::sort() {
  if (sz >= 2) {
    for (node* it1 = head; it1 != tail; it1 = it1->next_node)
      for (node* it2 = it1->next_node; it2 != stub; it2 = it2->next_node)
        if (it1->value > it2->value) std::swap(it1->value, it2->value);
  }
}

template <typename T>
void list<T>::unique() {
  auto it = begin();
  T prev = it.ptr->value;
  ++it;
  while (it != end()) {
    if (it.ptr->value == prev) {
      auto tmp_it = it;
      prev = it.ptr->value;
      ++it;
      erase(tmp_it);
    } else {
      prev = it.ptr->value;
      ++it;
    }
  }
}

template <typename T>
void list<T>::reverse() {
  if (sz >= 2) {
    auto it1 = begin();
    auto it2 = end();
    --it2;
    for (; (it1 != it2) && (it1.ptr->prev_node != it2.ptr); ++it1, --it2)
      std::swap(it1.ptr->value, it2.ptr->value);
  }
}

template <typename T>
void list<T>::merge(list& other) {
  if (this != &other) {
    auto it1 = begin();
    auto it2 = other.begin();
    while ((it1 != end()) && (it2 != other.end())) {
      if ((it1 == end()) || (*it2 < *it1)) {
        it1 = insert(it1, *it2);
        ++it1;
        ++it2;
      } else
        ++it1;
    }
    other.clear();
    other.head = other.tail = other.stub = new node();
  }
}

template <typename T>
void list<T>::splice(iterator pos, list& other) {
  if (this != &other) {
    for (auto it = other.begin(); it != other.end(); ++it, ++pos)
      pos = insert(pos, *it);
    other.clear();
    other.head = other.tail = other.stub = new node();
  }
}

template <typename T>
template <typename... Args>
typename list<T>::iterator list<T>::insert_many(const_iterator cpos,
                                                Args&&... args) {
  list local_list{args...};
  iterator pos;
  for (auto it = local_list.begin(); it != local_list.end(); ++it) {
    pos = iterator(cpos.get_iter());
    insert(pos, *it);
  }
  return pos;
}

template <typename T>
template <typename... Args>
void list<T>::insert_many_back(Args&&... args) {
  list local_list{args...};
  for (auto it = local_list.begin(); it != local_list.end(); ++it)
    push_back(*it);
}

template <typename T>
template <typename... Args>
void list<T>::insert_many_front(Args&&... args) {
  list local_list{args...};
  iterator pos = begin();
  for (auto it = local_list.begin(); it != local_list.end(); ++it)
    insert(pos, *it);
}

template <typename T>
void list<T>::print() {
  std::cout << "size: " << sz << '\n';
  node* tmp = head;
  while (tmp) {
    std::cout << tmp->value << ' ';
    tmp = tmp->next_node;
  }
  std::cout << "\n\n";
}
}  // namespace s21

#endif
