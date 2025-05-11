#pragma once

#include <cstddef>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <utility>
#include "mini_iterator.h"

namespace mini_stl {
template <typename T, typename Allocator = std::allocator<T>> class vector {
  public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using iterator = mini_stl::vector_iterator<T>;
    using const_iterator = mini_stl::vector_const_iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using allocator_type = Allocator;

    // 默认构造函数（使用默认 allocator）
    vector();

    // 构造函数：指定大小并填充值（所有元素为 value）
    vector(size_t count, const T &value, const Allocator &alloc);

    // 构造函数：指定大小，默认构造元素
    explicit vector(size_t count, const Allocator &alloc = Allocator());

    // 构造函数：从迭代器范围构造
    template <typename InputIt>
    vector(InputIt first, InputIt last, const Allocator &alloc = Allocator());

    // 拷贝构造函数（深拷贝）
    vector(const vector &other);

    // 移动构造函数（所有权转移）
    vector(vector &&other) noexcept;

    // 析构函数：销毁元素并释放内存
    ~vector();

    // 添加元素：拷贝版本
    void push_back(const T &value);

    // 添加元素：移动版本
    void push_back(T &&value);

    // 就地构造元素（完美转发参数）
    template <typename... Args> void emplace_back(Args &&...args);

    // 清空容器，销毁所有元素
    void clear() noexcept;

    // 保证容器至少有 n 个元素的空间，不改变当前 size_
    void reserve(size_t new_cap);

    // 下标访问（不检查越界）
    T &operator[](size_t index) noexcept;
    const T &operator[](size_t index) const;

    // 安全访问：带边界检查
    T &at(size_t index);
    const T &at(size_t index) const;

    // 拷贝赋值
    vector &operator=(const vector &other);

    // 移动赋值
    vector &operator=(vector &&other) noexcept;

    // 初始化列表赋值（语法糖）
    vector &operator=(std::initializer_list<T> ilist);

    // 获取当前容器中元素数量
    size_t size() const;

    // 重新设置容器大小
    void resize(size_t new_size);

    // resize：使用填充值
    void resize(size_t new_size, const T &value);

    // shrink_to_fit：释放多余内存
    void shrink_to_fit();

    // insert: 在 pos 位置插入拷贝元素
    iterator insert(iterator pos, const T &value);

    // insert: 在 pos 位置插入移动元素
    iterator insert(iterator pos, T &&value);

    // erase: 删除单个元素
    iterator erase(iterator pos);

    // erase: 删除范围 [first, last)
    iterator erase(iterator first, iterator last);

    // 获取容器大小
    size_t capacity() const { return capacity_; }

    // 迭代器相关接口

    // 返回指向第一个元素的正向迭代器（begin -> ... -> end）
    iterator begin() noexcept { return iterator(data_); }
    const_iterator begin() const noexcept { return const_iterator(data_); }

    // 返回指向最后一个元素之后的位置（end 不能解引用）
    iterator end() noexcept { return iterator(data_ + size_); }
    const_iterator end() const noexcept {
        return const_iterator(data_ + size_);
    }

    // 反向迭代器：从末尾向前遍历
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    // 返回第一个元素
    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }

    // 返回最后一个元素
    reference back() { return *(end() - 1); }
    const_reference back() const { return *(end() - 1); }

  private:
    T *data_;              // 数据区指针
    size_t size_;          // 当前元素个数
    size_t capacity_;      // 当前容量
    allocator_type alloc_; // 分配器

    // 容量不足时扩容：翻倍策略
    void __maybe_resize();
};

// 默认构造函数
template <typename T, typename Allocator>
vector<T, Allocator>::vector()
    : data_(nullptr), size_(0), capacity_(0), alloc_() {}

// 构造函数：指定大小并填充值
template <typename T, typename Allocator>
vector<T, Allocator>::vector(size_t count, const T &value,
                             const Allocator &alloc)
    : size_(count), capacity_(count), alloc_(alloc) {
    data_ = alloc_.allocate(capacity_);
    size_t i = 0;
    try {
        for (; i < size_; ++i)
            std::construct_at(data_ + i, value);
    } catch (...) {
        for (size_t j = 0; j < i; ++j)
            std::destroy_at(data_ + j);
        alloc_.deallocate(data_, capacity_);
        throw;
    }
}

// 构造函数：默认值构造 count 个元素
template <typename T, typename Allocator>
vector<T, Allocator>::vector(size_t count, const Allocator &alloc)
    : size_(count), capacity_(count), alloc_(alloc) {
    data_ = alloc_.allocate(capacity_);
    size_t i = 0;
    try {
        for (; i < size_; ++i)
            std::construct_at(data_ + i);
    } catch (...) {
        for (size_t j = 0; j < i; ++j)
            std::destroy_at(data_ + j);
        alloc_.deallocate(data_, capacity_);
        throw;
    }
}

// 构造函数：从迭代器范围构造
template <typename T, typename Allocator>
template <typename InputIt>
vector<T, Allocator>::vector(InputIt first, InputIt last,
                             const Allocator &alloc)
    : alloc_(alloc) {
    size_ = capacity_ = std::distance(first, last);
    data_ = alloc_.allocate(capacity_);
    size_t i = 0;
    try {
        for (InputIt it = first; it != last; ++it, ++i)
            std::construct_at(data_ + i, *it);
    } catch (...) {
        for (size_t j = 0; j < i; ++j)
            std::destroy_at(data_ + j);
        alloc_.deallocate(data_, capacity_);
        throw;
    }
}

// 拷贝构造函数
template <typename T, typename Allocator>
vector<T, Allocator>::vector(const vector &other)
    : size_(other.size_), capacity_(other.capacity_), alloc_(other.alloc_) {
    data_ = alloc_.allocate(capacity_);
    size_t i = 0;
    try {
        for (; i < size_; ++i)
            std::construct_at(data_ + i, other.data_[i]);
    } catch (...) {
        for (size_t j = 0; j < i; ++j)
            std::destroy_at(data_ + j);
        alloc_.deallocate(data_, capacity_);
        throw;
    }
}

// 移动构造函数
template <typename T, typename Allocator>
vector<T, Allocator>::vector(vector &&other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_),
      alloc_(std::move(other.alloc_)) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

// 析构函数
template <typename T, typename Allocator> vector<T, Allocator>::~vector() {
    clear();
    if (data_)
        alloc_.deallocate(data_, capacity_);
}

// 清空容器
template <typename T, typename Allocator>
void vector<T, Allocator>::clear() noexcept {
    for (size_t i = 0; i < size_; ++i)
        std::destroy_at(data_ + i);
    size_ = 0;
}

// 元素访问
template <typename T, typename Allocator>
T &vector<T, Allocator>::operator[](size_t index) noexcept {
    return data_[index];
}

template <typename T, typename Allocator>
const T &vector<T, Allocator>::operator[](size_t index) const {
    return data_[index];
}

template <typename T, typename Allocator>
T &vector<T, Allocator>::at(size_t index) {
    if (index >= size_)
        throw std::out_of_range("vector::at: index out of range");
    return data_[index];
}

template <typename T, typename Allocator>
const T &vector<T, Allocator>::at(size_t index) const {
    if (index >= size_)
        throw std::out_of_range("vector::at: index out of range");
    return data_[index];
}

// 当前元素数量
template <typename T, typename Allocator>
size_t vector<T, Allocator>::size() const {
    return size_;
}

// reserve 实现
template <typename T, typename Allocator>
void vector<T, Allocator>::reserve(size_t new_cap) {
    if (new_cap <= capacity_)
        return;

    T *new_data = alloc_.allocate(new_cap);
    size_t i = 0;

    try {
        for (; i < size_; ++i)
            std::construct_at(new_data + i, std::move_if_noexcept(data_[i]));
    } catch (...) {
        for (size_t j = 0; j < i; ++j)
            std::destroy_at(new_data + j);
        alloc_.deallocate(new_data, new_cap);
        throw;
    }

    // 不使用 clear()，只手动 destroy，保留 size_
    for (size_t j = 0; j < size_; ++j)
        std::destroy_at(data_ + j);
    if (data_)
        alloc_.deallocate(data_, capacity_);

    data_ = new_data;
    capacity_ = new_cap;
}

// push_back 实现
template <typename T, typename Allocator>
void vector<T, Allocator>::push_back(const T &value) {
    if (size_ == capacity_)
        __maybe_resize();
    std::construct_at(data_ + size_, value);
    ++size_;
}

template <typename T, typename Allocator>
void vector<T, Allocator>::push_back(T &&value) {
    if (size_ == capacity_)
        __maybe_resize();
    std::construct_at(data_ + size_, std::move_if_noexcept(value));
    ++size_;
}

// emplace_back 实现
template <typename T, typename Allocator>
template <typename... Args>
void vector<T, Allocator>::emplace_back(Args &&...args) {
    if (size_ == capacity_)
        __maybe_resize();
    std::construct_at(data_ + size_, std::forward<Args>(args)...);
    ++size_;
}

// 拷贝赋值
template <typename T, typename Allocator>
vector<T, Allocator> &vector<T, Allocator>::operator=(const vector &other) {
    if (this != &other) {
        vector tmp(other);
        *this = std::move(tmp);
    }
    return *this;
}

// 移动赋值
template <typename T, typename Allocator>
vector<T, Allocator> &vector<T, Allocator>::operator=(vector &&other) noexcept {
    if (this != &other) {
        clear();
        if (data_)
            alloc_.deallocate(data_, capacity_);
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        alloc_ = std::move(other.alloc_);
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

// initializer_list 赋值
template <typename T, typename Allocator>
vector<T, Allocator> &
vector<T, Allocator>::operator=(std::initializer_list<T> ilist) {
    clear();
    if (ilist.size() > capacity_) {
        if (data_)
            alloc_.deallocate(data_, capacity_);
        data_ = alloc_.allocate(ilist.size());
        capacity_ = ilist.size();
    }
    size_t i = 0;
    for (const auto &item : ilist) {
        std::construct_at(data_ + i, item);
        ++i;
    }
    size_ = ilist.size();
    return *this;
}

// 扩容函数：默认 2 倍扩容
template <typename T, typename Allocator>
void vector<T, Allocator>::__maybe_resize() {
    reserve(capacity_ == 0 ? 1 : capacity_ * 2);
}

// resize 实现：缩小销毁，扩大默认构造
template <typename T, typename Allocator>
void vector<T, Allocator>::resize(size_t new_size) {
    if (new_size < size_) {
        for (size_t i = new_size; i < size_; ++i)
            std::destroy_at(data_ + i);
    } else if (new_size > size_) {
        reserve(new_size);
        for (size_t i = size_; i < new_size; ++i)
            std::construct_at(data_ + i);
    }
    size_ = new_size;
}

// resize 实现：使用填充值
template <typename T, typename Allocator>
void vector<T, Allocator>::resize(size_t new_size, const T &value) {
    if (new_size < size_) {
        for (size_t i = new_size; i < size_; ++i)
            std::destroy_at(data_ + i);
    } else if (new_size > size_) {
        reserve(new_size);
        for (size_t i = size_; i < new_size; ++i)
            std::construct_at(data_ + i, value);
    }
    size_ = new_size;
}

// shrink_to_fit：释放多余内存
template <typename T, typename Allocator>
void vector<T, Allocator>::shrink_to_fit() {
    if (size_ == capacity_)
        return;

    T *new_data = alloc_.allocate(size_);
    size_t i = 0;
    try {
        for (; i < size_; ++i)
            std::construct_at(new_data + i, std::move_if_noexcept(data_[i]));
    } catch (...) {
        for (size_t j = 0; j < i; ++j)
            std::destroy_at(new_data + j);
        alloc_.deallocate(new_data, size_);
        throw;
    }

    // 不要 clear()，保留 size_
    for (size_t j = 0; j < size_; ++j)
        std::destroy_at(data_ + j);

    alloc_.deallocate(data_, capacity_);

    data_ = new_data;
    capacity_ = size_;  // 更新容量
}

// insert: 在 pos 位置插入拷贝元素
template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(iterator pos, const T &value) {
    size_t idx = pos - begin();
    if (size_ == capacity_)
        __maybe_resize();
    pos = begin() + idx;
    for (iterator it = end(); it != pos; --it)
        *it = std::move(*(it - 1));
    *pos = value;
    ++size_;
    return pos;
}

// insert: 在 pos 位置插入移动元素
template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(iterator pos, T &&value) {
    size_t idx = pos - begin();
    if (size_ == capacity_)
        __maybe_resize();
    pos = begin() + idx;
    for (iterator it = end(); it != pos; --it)
        *it = std::move(*(it - 1));
    *pos = std::move(value);
    ++size_;
    return pos;
}

// erase: 删除单个元素
template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::erase(iterator pos) {
    for (iterator it = pos; it != end() - 1; ++it)
        *it = std::move(*(it + 1));
    --size_;
    std::destroy_at((end()).base());  // 修复：end() 是 iterator，需要 .base()
    return pos;
}

// erase: 删除范围 [first, last)
template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::erase(iterator first, iterator last) {
    if (first == last)
        return first;

    iterator new_end = std::move(last, end(), first);

    for (iterator it = new_end; it != end(); ++it)
        std::destroy_at(it.base());  // 修复：必须转为 T*
        
    size_ -= (last - first);
    return first;
}
} // namespace mini_stl