#include "functions.h"
#include <iostream>
#include <cstring>

// ======================
// ReuseMemoryResource
// ======================
void* ReuseMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    for (auto it = freeBlocks.begin(); it != freeBlocks.end(); ++it) {
        if (allocatedBlocks[*it] >= bytes) {
            void* ptr = *it;
            freeBlocks.erase(it);
            return ptr;
        }
    }
    void* ptr = ::operator new(bytes, std::align_val_t(alignment));
    allocatedBlocks[ptr] = bytes;
    return ptr;
}

void ReuseMemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    if (allocatedBlocks.find(p) != allocatedBlocks.end()) {
        freeBlocks.push_back(p);
    }
}

ReuseMemoryResource::~ReuseMemoryResource() {
    for (auto& [ptr, size] : allocatedBlocks) {
        ::operator delete(ptr);
    }
    allocatedBlocks.clear();
    freeBlocks.clear();
}

// ======================
// MyContainer
// ======================
template<typename T>
MyContainer<T>::MyContainer(std::pmr::memory_resource* mr)
    : alloc(mr), data(nullptr), sz(0), capacity(0) {}

template<typename T>
MyContainer<T>::~MyContainer() {
    if (data) {
        for (size_t i = 0; i < sz; ++i) {
            data[i].~T();
        }
        alloc.deallocate(data, capacity);
    }
}

template<typename T>
void MyContainer<T>::push_back(const T& value) {
    if (sz == capacity) {
        size_t newCapacity = capacity == 0 ? 1 : capacity * 2;
        T* newData = alloc.allocate(newCapacity);
        for (size_t i = 0; i < sz; ++i) {
            new (newData + i) T(std::move(data[i]));
            data[i].~T();
        }
        if (data) alloc.deallocate(data, capacity);
        data = newData;
        capacity = newCapacity;
    }
    new (data + sz) T(value);
    ++sz;
}

// Explicit instantiation for common types
template class MyContainer<int>;
struct TestStruct { int a; double b; };
template class MyContainer<TestStruct>;
