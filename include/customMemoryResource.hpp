#pragma once

#include <cstddef>
#include <vector>
#include <cstddef>


class CustomMemResource : public std::pmr::memory_resource {
private:
    struct MemBlock {
        void *ptr;
        size_t size;
        size_t alignment;
        bool used;
    };

    std::vector<MemBlock> used_blocks;
    char *begin_mem_pool;
    size_t pool_size;
    size_t current_position = 0;

public:
    CustomMemResource(size_t mem_size);
    void *do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void *ptr, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override;
    ~CustomMemResource();
};