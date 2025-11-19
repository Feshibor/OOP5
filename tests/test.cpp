#include <gtest/gtest.h>
#include "../include/functions.h"

struct TestStruct { int a; double b; };

TEST(MemoryResourceTest, AllocateDeallocate) {
    ReuseMemoryResource resource;
    void* ptr = resource.allocate(16);
    EXPECT_NE(ptr, nullptr);
    resource.deallocate(ptr, 16);
}

TEST(ContainerTest, PushBackInt) {
    ReuseMemoryResource resource;
    MyContainer<int> c(&resource);
    c.push_back(42);
    c.push_back(99);
    EXPECT_EQ(c.size(), 2);
    auto it = c.begin();
    EXPECT_EQ(*it, 42);
    ++it;
    EXPECT_EQ(*it, 99);
}

TEST(ContainerTest, PushBackStruct) {
    ReuseMemoryResource resource;
    MyContainer<TestStruct> c(&resource);
    c.push_back({1, 1.1});
    c.push_back({2, 2.2});
    EXPECT_EQ(c.size(), 2);
    auto it = c.begin();
    EXPECT_EQ(it->a, 1);
    EXPECT_EQ(it->b, 1.1);
    ++it;
    EXPECT_EQ(it->a, 2);
    EXPECT_EQ(it->b, 2.2);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
