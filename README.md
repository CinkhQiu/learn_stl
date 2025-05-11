# Learn_stl
## mini_vector

`mini_vector` 是一个简洁实现的 C++ 模板向量类，模拟 `std::vector` 的核心功能，包含自定义随机访问迭代器，并支持以下操作：

### 功能特性

- 支持 `push_back`, `insert`, `erase`, `resize`, `shrink_to_fit`
- 支持 `operator[]`, `front`, `back`, `begin`, `end`, `rbegin`, `rend`
- 支持拷贝构造、移动构造、拷贝赋值、移动赋值
- 自定义迭代器兼容 STL 算法，如 `std::sort`
- 基于 `std::allocator` 实现动态内存管理

### 目录结构

```
.
├── include/
│   └── vector/
│       ├── mini_iterator.h       // 自定义迭代器
│       └── mini_vector.h         // vector 实现
├── test/
│   ├── test_vector.cpp           // 基础功能测试
│   └── test_vector_insert_erase.cpp // 扩展功能测试
├── src/                          // （可选）实现源文件
└── bin/                          // 可执行文件输出目录
```

### 编译方法

需使用支持 C++20 的编译器（如 g++ 10+）：

```bash
g++ -std=c++20 -Iinclude test/test_vector.cpp -o ./bin/test_vector
g++ -std=c++20 -Iinclude test/test_vector_insert_erase.cpp -o ./bin/test_vector_insert_erase
```

### 示例输出（节选）

```
初始: 0 10 20 30 40
insert(2, 99): 0 10 99 20 30 40
erase(3): 0 10 99 30 40
resize(5, -1): 0 30 40 -1 -1
shrink 之后: 3
```

### 注意事项

- 所有接口需手动管理内存，需特别注意异常安全与构造/析构时机。
- 迭代器需保证 `.base()` 方法能返回底层指针，用于 `std::destroy_at()` 等操作。

### 后续可扩展

- `assign`, `swap`, `emplace`
- 容器比较运算符支持
- 更强的异常安全保证
- 单元测试框架集成（如 gtest）