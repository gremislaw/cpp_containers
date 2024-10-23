#ifndef S21_STACK_HPP
#define S21_STACK_HPP

#include <stdbool.h>

#include <iostream>

namespace s21 {
template <typename T>
class stack {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  stack();
  stack(std::initializer_list<value_type> const& item);
  stack(const stack& s);
  stack(stack&& s);
  ~stack();
  stack& operator=(stack&& s);

  const_reference top() const;
  bool empty() const;
  size_type size();
  void push(const_reference val);
  void pop();
  void swap(stack& other);

  template <typename... Args>
  void insert_many_back(Args&&... args);

  void print();

 private:
  struct node {
    T data;
    node* next;
    node* prev;
    node(T dat) : data(dat) {}
  };
  node* head;
};

template <typename T>
stack<T>::stack() : head(nullptr) {}

template <typename T>
stack<T>::stack(std::initializer_list<T> const& item) : stack() {
  for (T el : item) {
    node* prev_head = head;
    head = new node(el);
    head->next = prev_head;
  }
}

template <typename T>
stack<T>::stack(const stack& other) : stack() {
  if (!other.empty()) {
    head = new node(other.head->data);
    node* it1 = head;
    node* it2 = other.head;
    for (; it2->next != nullptr; it1 = it1->next, it2 = it2->next)
      it1->next = new node(it2->next->data);
    it1->next = nullptr;
  }
}

template <typename T>
stack<T>::stack(stack&& other) : head(other.head) {
  other.head = nullptr;
}

template <typename T>
stack<T>::~stack() {
  while (!empty()) pop();
}

template <typename T>
stack<T>& stack<T>::operator=(stack&& other) {
  if (this != &other) {
    ~stack();
    head = other.head;
    other.head = nullptr;
  }
  return *this;
}

template <typename T>
const T& stack<T>::top() const {
  if (empty()) {
    throw std::logic_error("get top from empty stack");
  }
  return head->data;
}

template <typename T>
bool stack<T>::empty() const {
  return (head == nullptr);
}

template <typename T>
size_t stack<T>::size() {
  size_t res = 0;
  if (!empty()) {
    res = 1;
    node* temp = head;
    while (temp->next) {
      ++res;
      temp = temp->next;
    }
  }
  return res;
}

template <typename T>
void stack<T>::push(const T& val) {
  node* new_head = new node(val);
  new_head->next = head;
  head = new_head;
}

template <typename T>
void stack<T>::pop() {
  if (!empty()) {
    node* temp = head;
    head = temp->next;
    delete temp;
  }
}

template <typename T>
void stack<T>::swap(stack& other) {
  node* temp = head;
  head = other.head;
  other.head = temp;
}

template <typename T>
template <typename... Args>
void stack<T>::insert_many_back(Args&&... args) {
  vector<T> tmp{args...};
  for (size_t i = 0; i < tmp.size(); ++i) push(tmp[i]);
}

template <typename T>
void stack<T>::print() {
  node* tmp = head;
  if (!empty()) {
    while (tmp) {
      std::cout << tmp->data << ' ';
      tmp = tmp->next;
    }
    std::cout << '\n';
  }
}
}  // namespace s21

#endif