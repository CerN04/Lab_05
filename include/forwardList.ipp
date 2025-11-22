#include <memory>
#include <new>

template <typename T> ForwardList<T>::ListItem::ListItem(const T &value) : data(value), next(nullptr) {}

template <typename T> ForwardList<T>::ForwardIterator::ForwardIterator() : iter(nullptr) {}

template <typename T> ForwardList<T>::ForwardIterator::ForwardIterator(ListItem *node) : iter(node) {}

template <typename T>
typename ForwardList<T>::ForwardIterator::reference ForwardList<T>::ForwardIterator::operator*() const {
    if (iter == nullptr) {
        throw std::runtime_error("Dereferencing end iterator");
    }
    return iter->data;
}

template <typename T> typename ForwardList<T>::ForwardIterator::pointer ForwardList<T>::ForwardIterator::operator->() const {
    if (iter == nullptr) {
        throw std::runtime_error("Dereferencing end iterator");
    }
    return &iter->data;
}

template <typename T> typename ForwardList<T>::ForwardIterator &ForwardList<T>::ForwardIterator::operator++() {
    if (iter == nullptr) {
        throw std::runtime_error("Incrementing end iterator");
    }
    iter = iter->next;
    return *this;
}

template <typename T> bool ForwardList<T>::ForwardIterator::operator==(const ForwardIterator &other) const {
    return iter == other.iter;
}

template <typename T> bool ForwardList<T>::ForwardIterator::operator!=(const ForwardIterator &other) const {
    return iter != other.iter;
}

template <typename T> typename ForwardList<T>::ForwardIterator ForwardList<T>::begin() { return ForwardIterator(head); }

template <typename T> typename ForwardList<T>::ForwardIterator ForwardList<T>::cbegin() const {
    return ForwardIterator(head);
}

template <typename T> typename ForwardList<T>::ForwardIterator ForwardList<T>::end() { return ForwardIterator(nullptr); }

template <typename T> typename ForwardList<T>::ForwardIterator ForwardList<T>::cend() const {
    return ForwardIterator(nullptr);
}

template <typename T>
ForwardList<T>::ForwardList(std::pmr::memory_resource *resource) : allocator(resource), head(nullptr) {}

template <typename T> void ForwardList<T>::push_front(const T &value) {
    ListItem *new_node = allocator.allocate(1);
    new (new_node) ListItem(value);
    new_node->next = head;
    head = new_node;
}

template <typename T> void ForwardList<T>::pop_front() {
    if (head) {
        ListItem *temp = head;
        head = head->next;
        temp->~ListItem();
        allocator.deallocate(temp, 1);
    }
}

template <typename T> bool ForwardList<T>::empty() const { return head == nullptr; }

template <typename T> void ForwardList<T>::clear() {
    while (head) {
        pop_front();
    }
}

template <typename T> ForwardList<T>::~ForwardList() { clear(); }