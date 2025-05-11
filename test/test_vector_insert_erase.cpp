#include <iostream>
#include <algorithm>
#include "vector/mini_vector.h"

void print_vector(const mini_stl::vector<int>& vec, const std::string& msg) {
    std::cout << msg;
    for (auto x : vec)
        std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    mini_stl::vector<int> vec;

    // 初始 push_back
    for (int i = 0; i < 5; ++i) {
        vec.push_back(i * 10);  // 0 10 20 30 40
    }
    print_vector(vec, "初始: ");

    // 测试 insert
    vec.insert(vec.begin() + 2, 99);  // 在位置2插入99 → 0 10 99 20 30 40
    print_vector(vec, "insert(2, 99): ");

    // 测试 erase 单个
    vec.erase(vec.begin() + 3);  // 删除 20 → 0 10 99 30 40
    print_vector(vec, "erase(3): ");

    // 测试 erase 范围
    vec.erase(vec.begin() + 1, vec.begin() + 3);  // 删除 10, 99 → 0 30 40
    print_vector(vec, "erase(1, 3): ");

    // 测试 resize
    vec.resize(5, -1);  // 增大到5个元素，填充-1 → 0 30 40 -1 -1
    print_vector(vec, "resize(5, -1): ");

    vec.resize(3);  // 缩小 → 0 30 40
    print_vector(vec, "resize(3): ");

    // 测试 shrink_to_fit（观察容量变化）
    std::cout << "容量 shrink 之前: " << vec.capacity() << "\n";
    vec.shrink_to_fit();
    std::cout << "容量 shrink 之后: " << vec.capacity() << "\n";

    return 0;
}