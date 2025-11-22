#pragma once

#include <iterator>
#include <memory>
#include <memory_resource>


template <typename T> class ForwardList {
private:
    struct ListItem {
        T data;
        ListItem *next;

        ListItem(const T &val);
    };

    ListItem *head;
    std::pmr::polymorphic_allocator<ListItem> allocator;

public:
    class ForwardIterator {
    private:
        ListItem *iter;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        ForwardIterator();
        ForwardIterator(ListItem *node);

        reference operator*() const;
        pointer operator->() const;

        ForwardIterator &operator++();

        bool operator==(const ForwardIterator &other) const;
        bool operator!=(const ForwardIterator &other) const;
    };

    ForwardList(std::pmr::memory_resource *resource);
    ~ForwardList();

    ForwardIterator begin();
    ForwardIterator cbegin() const;
    ForwardIterator end();
    ForwardIterator cend() const;

    void push_front(const T &val);
    void pop_front();
    bool empty() const;
    void clear();
};

#include "forwardList.ipp"