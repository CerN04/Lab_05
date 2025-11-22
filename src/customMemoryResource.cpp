#include "customMemoryResource.hpp"

#include <memory>
#include <new>
#include <stdexcept>


CustomMemResource::CustomMemResource(size_t mem_size) : pool_size(mem_size) {
    begin_mem_pool = static_cast<char *>(::operator new(mem_size));
}

CustomMemResource::~CustomMemResource() { ::operator delete(begin_mem_pool); }

void *CustomMemResource::do_allocate(size_t bytes, size_t alignment) {
    if (bytes == 0) {
        throw std::invalid_argument("Invalid allocate");
    }
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
        throw std::invalid_argument("Invalid alignment");
    }
    if (bytes > pool_size) {
        throw std::bad_alloc();
    }
    if (current_position >= pool_size) {
        throw std::bad_alloc();
    }

    for (auto &block : used_blocks) {
        if (!block.used && block.size >= bytes) {
            void *ptr = block.ptr;
            size_t size = block.size;

            if (std::align(alignment, bytes, ptr, size)) {
                if (size >= bytes) {
                    if (size > bytes) {
                        void *remaining_ptr = static_cast<char *>(ptr) + bytes;
                        size_t remaining_size = size - bytes;
                        used_blocks.push_back({remaining_ptr, remaining_size, alignment, false});
                    }

                    block.ptr = ptr;
                    block.size = bytes;
                    block.alignment = alignment;
                    block.used = true;
                    return ptr;
                }
            }
        }
    }

    void *new_ptr = begin_mem_pool + current_position;
    size_t available = pool_size - current_position;
    if (std::align(alignment, bytes, new_ptr, available)) {
        if (available >= bytes) {
            used_blocks.push_back({new_ptr, bytes, alignment, true});
            current_position = static_cast<char *>(new_ptr) - begin_mem_pool + bytes;
            return new_ptr;
        }
    }

    throw std::bad_alloc();
}

void CustomMemResource::do_deallocate(void *ptr, size_t bytes, size_t alignment) {
    if (ptr == nullptr) {
        return;
    }

    if (ptr < begin_mem_pool || ptr >= begin_mem_pool + pool_size) {
        throw std::invalid_argument("Pointer outside memory pool");
    }

    for (auto &block : used_blocks) {
        if (block.ptr == ptr && block.size == bytes && block.alignment == alignment) {
            if (!block.used) {
                throw std::logic_error("Double deallocate");
            }
            block.used = false;
            return;
        }
    }
    throw std::invalid_argument("Invalid deallocation");
}

bool CustomMemResource::do_is_equal(const std::pmr::memory_resource &other) const noexcept { return this == &other; }