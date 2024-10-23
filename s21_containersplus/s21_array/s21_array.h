#ifndef S21_ARRAY_HPP
#define S21_ARRAY_HPP

#include <stdbool.h>

#include <exception>
#include <iostream>
#include <limits>

namespace s21 {
template <typename T, std::size_t N>
class array {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = size_t;

  array();
  array(std::initializer_list<value_type> const& items);
  array(const array& a);
  array(array&& a);
  ~array();
  array& operator=(array&& a);

  reference at(size_type pos);
  reference operator[](size_type pos);
  const_reference front();
  const_reference back();
  iterator data() const;

  iterator begin() const;
  iterator end() const;

  bool empty() const;
  size_type size() const;
  size_type max_size() const;
  void swap(array& other);
  void fill(const_reference value);

  void print();

 private:
  T* mas;
};

template <typename T, std::size_t N>
array<T, N>::array() {
  mas = new T[N];
  for (size_t i = 0; i < N; ++i) mas[i] = 0;
}

template <typename T, std::size_t N>
array<T, N>::array(std::initializer_list<T> const& items) {
  mas = new T[items.size()];
  for (size_t i = 0; i < items.size(); ++i) mas[i] = *(items.begin() + i);
}

template <typename T, std::size_t N>
array<T, N>::array(const array& a) {
  if (this->size() == a.size()) {
    mas = new T[N];
    for (size_t i = 0; i < N; ++i) mas[i] = a.mas[i];
  } else
    throw std::out_of_range("Sizes of arrays are different");
}

template <typename T, std::size_t N>
array<T, N>::array(array&& a) {
  if (this->size() == a.size()) {
    mas = a.mas;
    a.mas = nullptr;
  } else
    throw std::out_of_range("Sizes of arrays are different");
}

template <typename T, std::size_t N>
array<T, N>::~array() {
  if (mas) delete[] mas;
}

template <typename T, std::size_t N>
array<T, N>& array<T, N>::operator=(array&& a) {
  if (this->size() == a.size()) {
    delete[] mas;
    mas = a.mas;
    a.mas = nullptr;
  } else
    throw std::out_of_range("Sizes of arrays are different");
}

template <typename T, std::size_t N>
T& array<T, N>::at(size_t pos) {
  if (pos >= N) throw std::out_of_range("Index out range");
  return *(mas + pos);
}

template <typename T, std::size_t N>
T& array<T, N>::operator[](size_t pos) {
  return *(mas + pos);
}

template <typename T, std::size_t N>
const T& array<T, N>::front() {
  return *mas;
}

template <typename T, std::size_t N>
const T& array<T, N>::back() {
  return *(mas + N - 1);
}

template <typename T, std::size_t N>
T* array<T, N>::data() const {
  return mas;
}

template <typename T, std::size_t N>
T* array<T, N>::begin() const {
  return mas;
}

template <typename T, std::size_t N>
T* array<T, N>::end() const {
  return mas + N;
}

template <typename T, std::size_t N>
bool array<T, N>::empty() const {
  return (N == 0);
}

template <typename T, std::size_t N>
size_t array<T, N>::size() const {
  return N;
}

template <typename T, std::size_t N>
size_t array<T, N>::max_size() const {
  return std::numeric_limits<std::size_t>::max() / (2 * sizeof(T));
}

template <typename T, std::size_t N>
void array<T, N>::swap(array& other) {
  if (this->size() == other.size()) {
    T* tmas = mas;
    mas = other.mas;
    other.mas = tmas;
  } else
    throw std::out_of_range("Sizes of arrays are different");
  return;
}

template <typename T, std::size_t N>
void array<T, N>::fill(const T& value) {
  for (size_t i = 0; i < N; ++i) mas[i] = value;
}

template <typename T, std::size_t N>
void array<T, N>::print() {
  for (size_t i = 0; i < N; ++i) std::cout << mas[i] << ' ';
  std::cout << '\n';
}

}  // namespace s21

#endif