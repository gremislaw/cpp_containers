#ifndef S21_QUEUE_H
#define S21_QUEUE_H

#include <stdbool.h>

#include <exception>
#include <iostream>

namespace s21 {
template <typename T>
class queue {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

 public:
  queue();
  queue(std::initializer_list<value_type> const& items);
  queue(const queue& q);
  queue(queue&& q);
  ~queue();
  queue& operator=(queue&& q);
  queue& operator=(const queue& q);

  const_reference front() const;
  const_reference back() const;

  bool empty() const;
  size_type size();
  void pop();
  void push(const_reference value);
  void swap(queue& q);

  template <typename... Args>
  void insert_many_back(Args&&... args);

  void clear();
  void print();

 private:
  struct node {
    node* next;
    value_type data;
    node() : next(nullptr), data(0) {}
    node(value_type dat) : next(nullptr), data(dat) {}
  };
  node* head;
  node* tail;
  size_type sz;
};

template <typename T>
queue<T>::queue() : head(nullptr), tail(nullptr), sz(0) {}

template <typename T>
queue<T>::queue(std::initializer_list<T> const& items) : queue() {
  if (items.size()) {
    for (auto el : items) push(el);
  }
}

template <typename T>
queue<T>::queue(const queue& q) : queue() {
  for (node* temp = q.head; temp != nullptr; temp = temp->next)
    push(temp->data);
}

template <typename T>
queue<T>::queue(queue&& q) {
  head = q.head;
  tail = q.tail;
  sz = q.sz;
  q.head = nullptr;
  q.tail = nullptr;
  q.sz = 0;
}

template <typename T>
queue<T>::~queue() {
  clear();
}

template <typename T>
queue<T>& queue<T>::operator=(queue&& q) {
  if (this != &q) {
    clear();
    queue(std::forward<T>(q));
  }
  return *this;
}

template <typename T>
queue<T>& queue<T>::operator=(const queue& q) {
  if (this != &q) {
    clear();
    for (node* temp = q.head; temp != nullptr; temp = temp->next)
      push(temp->data);
  }
  return *this;
}

template <typename T>
const T& queue<T>::front() const {
  if (empty()) throw std::logic_error("get front from empty Queue");
  return head->data;
}

template <typename T>
const T& queue<T>::back() const {
  if (empty()) throw std::logic_error("get back from empty Queue");
  return tail->data;
}

template <typename T>
bool queue<T>::empty() const {
  return (head == nullptr);
}

template <typename T>
size_t queue<T>::size() {
  return sz;
}

template <typename T>
void queue<T>::pop() {
  if (!empty()) {
    node* temp = head;
    head = head->next;
    delete temp;
    --sz;
  }
}

template <typename T>
void queue<T>::push(const_reference value) {
  if (empty()) {
    tail = new node(value);
    head = tail;
  } else {
    node* new_back = new node(value);
    tail->next = new_back;
    tail = new_back;
  }
  ++sz;
}

template <typename T>
void queue<T>::swap(queue& other) {
  std::swap(other.sz, this->sz);
  std::swap(other.head, this->head);
  std::swap(other.tail, this->tail);
}

template <typename T>
template <typename... Args>
void queue<T>::insert_many_back(Args&&... args) {
  vector<T> tmp{args...};
  for (size_t i = 0; i < tmp.size(); ++i) push(tmp[i]);
}

template <typename T>
void queue<T>::clear() {
  while (!empty()) pop();
}

template <typename T>
void queue<T>::print() {
  for (node* temp = head; temp != nullptr; temp = temp->next)
    std::cout << temp->data << ' ';
  std::cout << '\n';
}
}  // namespace s21

#endif