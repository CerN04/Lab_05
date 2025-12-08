#include <gtest/gtest.h>
#include <stdexcept>

#include "../include/customMemoryResource.hpp"
#include "../include/forwardList.hpp"

// ==================== Тесты для CustomMemoryResource ====================

TEST(CustomMemoryResourceTest, BasicAllocation) {
    CustomMemResource resource(1000);

    void *ptr1 = resource.allocate(100, 8);
    ASSERT_NE(ptr1, nullptr);

    void *ptr2 = resource.allocate(50, 4);
    ASSERT_NE(ptr2, nullptr);
    ASSERT_NE(ptr2, ptr1);

    resource.deallocate(ptr1, 100, 8);
    resource.deallocate(ptr2, 50, 4);
}

TEST(CustomMemoryResourceTest, MemoryReuse) {
    CustomMemResource resource(1000);

    void *ptr1 = resource.allocate(100, 8);
    ASSERT_NE(ptr1, nullptr);
    resource.deallocate(ptr1, 100, 8);

    // Переиспользование освобожденной памяти
    void *ptr2 = resource.allocate(100, 8);
    ASSERT_NE(ptr2, nullptr);
    resource.deallocate(ptr2, 100, 8);
}

TEST(CustomMemoryResourceTest, OversizedAllocation) {
    CustomMemResource resource(100);

    EXPECT_THROW(
        {
            void *ptr = resource.allocate(200, 8);
            resource.deallocate(ptr, 200, 8);
        },
        std::bad_alloc);
}

TEST(CustomMemoryResourceTest, MultipleAllocations) {
    CustomMemResource resource(1000);

    void *ptrs[5];
    for (int i = 0; i < 5; ++i) {
        ptrs[i] = resource.allocate(50, 8);
        ASSERT_NE(ptrs[i], nullptr);
    }

    for (int i = 0; i < 5; ++i) {
        resource.deallocate(ptrs[i], 50, 8);
    }
}

// ==================== Тесты для ForwardList с int ====================

TEST(ForwardListIntTest, EmptyList) {
    CustomMemResource resource(1000);
    ForwardList<int> list(&resource);

    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.begin(), list.end());
}

TEST(ForwardListIntTest, PushFront) {
    CustomMemResource resource(1000);
    ForwardList<int> list(&resource);

    list.push_front(10);
    EXPECT_FALSE(list.empty());

    list.push_front(20);
    list.push_front(30);

    int count = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST(ForwardListIntTest, Iteration) {
    CustomMemResource resource(1000);
    ForwardList<int> list(&resource);

    list.push_front(10);
    list.push_front(20);
    list.push_front(30);

    int sum = 0;
    int count = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum += *it;
        count++;
    }

    EXPECT_EQ(count, 3);
    EXPECT_EQ(sum, 60); // 30 + 20 + 10
}

TEST(ForwardListIntTest, PopFront) {
    CustomMemResource resource(1000);
    ForwardList<int> list(&resource);

    list.push_front(10);
    list.push_front(20);
    list.push_front(30);

    list.pop_front();
    EXPECT_FALSE(list.empty());

    int count = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        count++;
    }
    EXPECT_EQ(count, 2);
}

TEST(ForwardListIntTest, Clear) {
    CustomMemResource resource(1000);
    ForwardList<int> list(&resource);

    list.push_front(10);
    list.push_front(20);
    list.push_front(30);

    list.clear();
    EXPECT_TRUE(list.empty());
    EXPECT_EQ(list.begin(), list.end());
}

// ==================== Тесты для ForwardList с пользовательской структурой ====================

struct TestStruct {
    int value;
    char symbol;

    bool operator==(const TestStruct &other) const { return value == other.value && symbol == other.symbol; }
};

TEST(ForwardListStructTest, PushFrontStruct) {
    CustomMemResource resource(1000);
    ForwardList<TestStruct> list(&resource);

    TestStruct s1{42, 'A'};
    TestStruct s2{100, 'B'};

    list.push_front(s1);
    list.push_front(s2);

    EXPECT_FALSE(list.empty());
}

TEST(ForwardListStructTest, IteratorAccess) {
    CustomMemResource resource(1000);
    ForwardList<TestStruct> list(&resource);

    TestStruct s1{42, 'A'};
    TestStruct s2{100, 'B'};

    list.push_front(s1);
    list.push_front(s2);

    auto it = list.begin();
    EXPECT_EQ(it->value, 100);
    EXPECT_EQ(it->symbol, 'B');

    ++it;
    EXPECT_EQ(it->value, 42);
    EXPECT_EQ(it->symbol, 'A');
}

TEST(ForwardListStructTest, DereferenceOperator) {
    CustomMemResource resource(1000);
    ForwardList<TestStruct> list(&resource);

    TestStruct s1{42, 'A'};
    TestStruct s2{100, 'B'};

    list.push_front(s1);
    list.push_front(s2);

    EXPECT_EQ((*list.begin()), s2);
}

// ==================== Тесты для итераторов ====================

TEST(ForwardListIteratorTest, EmptyListIterators) {
    CustomMemResource resource(1000);
    ForwardList<int> list(&resource);

    EXPECT_EQ(list.begin(), list.end());
}

TEST(ForwardListIteratorTest, IteratorComparison) {
    CustomMemResource resource(1000);
    ForwardList<int> list(&resource);

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    auto it1 = list.begin();
    auto it2 = list.begin();
    EXPECT_EQ(it1, it2);

    ++it1;
    EXPECT_NE(it1, it2);
}

TEST(ForwardListIteratorTest, IteratorIncrement) {
    CustomMemResource resource(1000);
    ForwardList<int> list(&resource);

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    auto it = list.begin();
    EXPECT_EQ(*it, 3);

    ++it;
    EXPECT_EQ(*it, 2);

    ++it;
    EXPECT_EQ(*it, 1);

    ++it;
    EXPECT_EQ(it, list.end());
}

// ==================== Граничные случаи ====================

TEST(ForwardListEdgeCasesTest, PopFrontOnEmpty) {
    CustomMemResource resource(1000);
    ForwardList<int> list(&resource);

    // Не должно падать при удалении из пустого списка
    list.pop_front();
    EXPECT_TRUE(list.empty());
}

TEST(ForwardListEdgeCasesTest, SingleElement) {
    CustomMemResource resource(1000);
    ForwardList<int> list(&resource);

    list.push_front(42);
    EXPECT_FALSE(list.empty());

    list.pop_front();
    EXPECT_TRUE(list.empty());
}

TEST(ForwardListEdgeCasesTest, MultipleElements) {
    CustomMemResource resource(1000);
    ForwardList<int> list(&resource);

    for (int i = 0; i < 10; ++i) {
        list.push_front(i);
    }

    int count = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        count++;
    }
    EXPECT_EQ(count, 10);

    list.clear();
    EXPECT_TRUE(list.empty());
}
