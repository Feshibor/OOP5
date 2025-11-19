#pragma once
#include <memory_resource>
#include <map>
#include <cstddef>
#include <iterator>
#include <algorithm>

// ======================
// Custom Memory Resource
// ======================
class ReuseMemoryResource : public std::pmr::memory_resource {
private:
    std::map<void*, size_t> allocatedBlocks;
    std::vector<void*> freeBlocks;

protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const memory_resource& other) const noexcept override {
        return this == &other;
    }

public:
    ~ReuseMemoryResource();
};

// ======================
// Custom Container
// ======================
template<typename T>
class MyContainer {
public:
    using value_type = T;
    using allocator_type = std::pmr::polymorphic_allocator<T>;

private:
    allocator_type alloc;
    T* data;
    size_t sz;
    size_t capacity;

public:
    explicit MyContainer(std::pmr::memory_resource* mr = std::pmr::get_default_resource());
    ~MyContainer();

    void push_back(const T& value);
    size_t size() const { return sz; }

    // Iterator
    class iterator {
        T* ptr;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit iterator(T* p) : ptr(p) {}
        reference operator*() const { return *ptr; }
        pointer operator->() const { return ptr; }
        iterator& operator++() { ++ptr; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++ptr; return tmp; }
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    };

    iterator begin() { return iterator(data); }
    iterator end() { return iterator(data + sz); }
};
