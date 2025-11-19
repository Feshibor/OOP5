#include "functions.h"
#include <iostream>

struct TestStruct {
    int x;
    double y;
};

int main() {
    ReuseMemoryResource resource;

    MyContainer<int> intContainer(&resource);
    for (int i = 0; i < 10; ++i) intContainer.push_back(i);

    std::cout << "Int container: ";
    for (auto v : intContainer) std::cout << v << " ";
    std::cout << "\n";

    MyContainer<TestStruct> structContainer(&resource);
    structContainer.push_back({1, 1.1});
    structContainer.push_back({2, 2.2});

    std::cout << "Struct container:\n";
    for (auto& s : structContainer) {
        std::cout << "x=" << s.x << " y=" << s.y << "\n";
    }

    return 0;
}
