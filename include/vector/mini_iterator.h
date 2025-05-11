#pragma once
#include <cstddef>
#include <iterator>

namespace mini_stl {

template <typename T> class vector_iterator {
  public:
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

  private:
    pointer ptr_;

  public:
    vector_iterator() : ptr_(nullptr) {}
    explicit vector_iterator(pointer ptr) : ptr_(ptr) {}

    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }

    vector_iterator &operator++() {
        ++ptr_;
        return *this;
    }
    vector_iterator operator++(int) {
        vector_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    vector_iterator &operator--() {
        --ptr_;
        return *this;
    }
    vector_iterator operator--(int) {
        vector_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    vector_iterator &operator+=(difference_type n) {
        ptr_ += n;
        return *this;
    }
    vector_iterator &operator-=(difference_type n) {
        ptr_ -= n;
        return *this;
    }

    vector_iterator operator+(difference_type n) const {
        return vector_iterator(ptr_ + n);
    }
    vector_iterator operator-(difference_type n) const {
        return vector_iterator(ptr_ - n);
    }
    difference_type operator-(const vector_iterator &rhs) const {
        return ptr_ - rhs.ptr_;
    }

    reference operator[](difference_type n) const { return *(ptr_ + n); }

    bool operator==(const vector_iterator &rhs) const {
        return ptr_ == rhs.ptr_;
    }
    bool operator!=(const vector_iterator &rhs) const {
        return ptr_ != rhs.ptr_;
    }
    bool operator<(const vector_iterator &rhs) const { return ptr_ < rhs.ptr_; }
    bool operator>(const vector_iterator &rhs) const { return ptr_ > rhs.ptr_; }
    bool operator<=(const vector_iterator &rhs) const {
        return ptr_ <= rhs.ptr_;
    }
    bool operator>=(const vector_iterator &rhs) const {
        return ptr_ >= rhs.ptr_;
    }

    pointer base() const { return ptr_; } // for reverse_iterator
};

template <typename T> class vector_const_iterator {
  public:
    using value_type = T;
    using pointer = const T *;
    using reference = const T &;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

  private:
    pointer ptr_;

  public:
    vector_const_iterator() : ptr_(nullptr) {}
    explicit vector_const_iterator(pointer ptr) : ptr_(ptr) {}
    vector_const_iterator(const vector_iterator<T> &it) : ptr_(it.base()) {}

    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }

    vector_const_iterator &operator++() {
        ++ptr_;
        return *this;
    }
    vector_const_iterator operator++(int) {
        vector_const_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    vector_const_iterator &operator--() {
        --ptr_;
        return *this;
    }
    vector_const_iterator operator--(int) {
        vector_const_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    vector_const_iterator &operator+=(difference_type n) {
        ptr_ += n;
        return *this;
    }
    vector_const_iterator &operator-=(difference_type n) {
        ptr_ -= n;
        return *this;
    }

    vector_const_iterator operator+(difference_type n) const {
        return vector_const_iterator(ptr_ + n);
    }
    vector_const_iterator operator-(difference_type n) const {
        return vector_const_iterator(ptr_ - n);
    }
    difference_type operator-(const vector_const_iterator &rhs) const {
        return ptr_ - rhs.ptr_;
    }

    reference operator[](difference_type n) const { return *(ptr_ + n); }

    bool operator==(const vector_const_iterator &rhs) const {
        return ptr_ == rhs.ptr_;
    }
    bool operator!=(const vector_const_iterator &rhs) const {
        return ptr_ != rhs.ptr_;
    }
    bool operator<(const vector_const_iterator &rhs) const {
        return ptr_ < rhs.ptr_;
    }
    bool operator>(const vector_const_iterator &rhs) const {
        return ptr_ > rhs.ptr_;
    }
    bool operator<=(const vector_const_iterator &rhs) const {
        return ptr_ <= rhs.ptr_;
    }
    bool operator>=(const vector_const_iterator &rhs) const {
        return ptr_ >= rhs.ptr_;
    }

    pointer base() const { return ptr_; }
};

} // namespace mini_stl