#include "vector/mini_vector.h"
#include <algorithm>
#include <iostream>

void print_vector(const mini_stl::vector<int> &vec,
                  const std::string &msg) {
    std::cout << msg;
    for (auto x : vec)
        std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    mini_stl::vector<int> vec;

    // 测试 push_back + front/back
    vec.push_back(10);
    vec.push_back(30);
    vec.push_back(20);
    vec.push_back(40);
    print_vector(vec, "原始: ");

    // 测试 operator[]
    std::cout << "vec[2] = " << vec[2] << "\n";
    vec[2] = 99;
    print_vector(vec, "修改 vec[2] 后: ");

    // 测试 sort
    std::sort(vec.begin(), vec.end());
    print_vector(vec, "排序后: ");

    // 测试 range-for 与 const_iterator
    const mini_stl::vector<int> &cvec = vec;
    std::cout << "范围for遍历: ";
    for (int v : cvec)
        std::cout << v << " ";
    std::cout << "\n";

    // 测试拷贝构造
    mini_stl::vector<int> copy_vec(vec);
    print_vector(copy_vec, "拷贝构造 copy_vec: ");

    // 测试移动构造
    mini_stl::vector<int> move_vec(std::move(copy_vec));
    print_vector(move_vec, "移动构造 move_vec: ");

    // 测试赋值运算符
    mini_stl::vector<int> assign_vec;
    assign_vec = vec;
    print_vector(assign_vec, "拷贝赋值 assign_vec: ");

    // 测试移动赋值运算符
    mini_stl::vector<int> move_assign_vec;
    move_assign_vec = std::move(assign_vec);
    print_vector(move_assign_vec, "移动赋值 move_assign_vec: ");

    return 0;
}