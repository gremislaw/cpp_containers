#ifndef S21_VECTOR_HPP
#define S21_VECTOR_HPP

#include <stdbool.h>

#include <iostream>
#include <limits>
#include <new>

namespace s21 {
template <typename T>
class vector {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = T*;
  using const_iterator = const T*;
  using size_type = size_t;

  vector();
  vector(size_type n);
  vector(std::initializer_list<value_type> const& items);
  vector(const vector& v);
  vector(vector&& v);
  ~vector();
  vector& operator=(vector&& v);

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
  void reserve(size_type size);
  size_type capacity() const;
  void shrink_to_fit();

  void clear();
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);
  void push_back(const_reference value);
  void pop_back();
  void swap(vector& other);

  void print();

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args);

  template <typename... Args>
  void insert_many_back(Args&&... args);

 private:
  size_type temp_size;
  size_type cap;
  value_type* mas;
};

template <typename T>
vector<T>::vector() : temp_size(0), cap(0), mas(0) {}

template <typename T>
vector<T>::vector(size_t n) : temp_size(n), cap(n), mas(new T[n]) {
  if (mas == nullptr) throw std::bad_alloc();
  for (size_t i = 0; i < n; ++i) mas[i] = T();
}

template <typename T>
vector<T>::vector(std::initializer_list<T> const& items)
    : vector(items.size()) {
  for (size_t i = 0; i < temp_size; ++i) mas[i] = *(items.begin() + i);
}

template <typename T>
vector<T>::vector(const vector<T>& v) : vector(v.capacity()) {
  temp_size = v.temp_size;
  for (size_t i = 0; i < temp_size; ++i) mas[i] = v.mas[i];
}

template <typename T>
vector<T>::vector(vector<T>&& v) : vector() {
  temp_size = v.temp_size;
  v.temp_size = 0;
  cap = v.cap;
  v.cap = 0;
  mas = v.mas;
  v.mas = nullptr;
}

template <typename T>
vector<T>::~vector() {
  delete[] mas;
  temp_size = 0;
  cap = 0;
  mas = nullptr;
}

template <typename T>
vector<T>& vector<T>::operator=(vector&& v) {
  if (this != &v) this->swap(v);
  return *this;
}

template <typename T>
T& vector<T>::at(size_t pos) {
  if (pos >= temp_size) throw std::out_of_range("Index out range");
  return *(mas + pos);
}

template <typename T>
T& vector<T>::operator[](size_t pos) {
  return *(mas + pos);
}

template <typename T>
const T& vector<T>::front() {
  return *mas;
}

template <typename T>
const T& vector<T>::back() {
  return *(mas + temp_size - 1);
}

template <typename T>
T* vector<T>::data() const {
  return mas;
}

template <typename T>
T* vector<T>::begin() const {
  return mas;
}

template <typename T>
T* vector<T>::end() const {
  return mas + temp_size;
}

template <typename T>
bool vector<T>::empty() const {
  return (temp_size == 0);
}

template <typename T>
size_t vector<T>::size() const {
  return temp_size;
}

template <typename T>
size_t vector<T>::max_size() const {
  return std::numeric_limits<std::size_t>::max() / (2 * sizeof(T));
}

template <typename T>
void vector<T>::reserve(size_t size) {
  if (size > max_size())
    throw std::length_error("Can't allocate memory of this size");
  if (size > cap) {
    vector<T> tmp(size);
    for (size_t i = 0; i < temp_size; ++i) tmp.mas[i] = mas[i];
    tmp.temp_size = temp_size;
    *this = std::move(tmp);
  }
  return;
}

template <typename T>
size_t vector<T>::capacity() const {
  return cap;
}

template <typename T>
void vector<T>::shrink_to_fit() {
  if ((cap > temp_size) && (temp_size > 0)) {
    vector<T> tmp(temp_size);
    for (size_t i = 0; i < temp_size; ++i) tmp.mas[i] = mas[i];
    *this = std::move(tmp);
  }
  return;
}

template <typename T>
void vector<T>::clear() {
  delete[] mas;
  mas = nullptr;
  temp_size = 0;
  cap = 0;
  return;
}

template <typename T>
T* vector<T>::insert(T* pos, const T& value) {
  size_t pos_t = pos - begin();
  ++temp_size;
  if (temp_size + 1 > cap) {
    vector<T> tmp(2 * ((cap > 0) ? cap : 1));
    tmp.temp_size = temp_size;
    for (size_t i = 0; i < temp_size - 1; ++i) tmp.mas[i] = mas[i];
    *this = std::move(tmp);
  }
  for (size_t i = temp_size - 1; i > pos_t; --i) mas[i] = mas[i - 1];
  mas[pos_t] = value;
  return (mas + pos_t);
}

template <typename T>
void vector<T>::erase(T* pos) {
  size_t pos_t = pos - begin();
  --temp_size;
  for (size_t i = pos_t; i < temp_size; ++i) mas[i] = mas[i + 1];
  return;
}

template <typename T>
void vector<T>::push_back(const T& value) {
  ++temp_size;
  if (temp_size + 1 > cap) {
    vector<T> tmp(2 * ((cap > 0) ? cap : 1));
    tmp.temp_size = temp_size;
    for (size_t i = 0; i < temp_size - 1; ++i) tmp.mas[i] = mas[i];
    *this = std::move(tmp);
  }
  mas[temp_size - 1] = value;
  return;
}

template <typename T>
void vector<T>::pop_back() {
  --temp_size;
  return;
}

template <typename T>
void vector<T>::swap(vector& other) {
  size_t tsize = temp_size;
  temp_size = other.temp_size;
  other.temp_size = tsize;

  size_t tcap = cap;
  cap = other.cap;
  other.cap = tcap;

  T* tmas = mas;
  mas = other.mas;
  other.mas = tmas;
  return;
}

template <typename T>
void vector<T>::print() {
  for (size_t i = 0; i < temp_size; ++i) std::cout << mas[i] << ' ';
  std::cout << "\nsize: " << temp_size << "\ncapacity: " << cap << "\n\n";
}

template <typename T>
template <typename... Args>
T* vector<T>::insert_many(const T* pos, Args&&... args) {
  vector<T> tmp{args...};
  T* tmp_pos = (T*)pos;
  for (size_t i = 0; i < tmp.size(); ++i) tmp_pos = insert(tmp_pos, tmp[i]) + 1;
  return tmp_pos;
}

template <typename T>
template <typename... Args>
void vector<T>::insert_many_back(Args&&... args) {
  insert_many(this->end(), args...);
  return;
}
}  // namespace s21

#endif