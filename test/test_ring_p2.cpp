#include <iostream>
#include <gtest/gtest.h>

#include <cstddef>
#include <algorithm>
#include <climits>  // CHAR_BIT

#include "ring_p2.h"

template<typename T>
struct RingBufferParamFixture
: public ::testing::Test
{
protected:
    jaxos::data_struct::ring<int,4> ring;

    virtual void SetUp()
    {      
        fill_buffer();
    }

    virtual void TearDown()
    {
    }

    void fill_buffer()
    {
        std::cout << "---- Adding elements to the queue til it is full.\n";
        for (int i = 0; !ring.full(); ++i)
        {
            ring.push_front(i);
        }
    }
};

TYPED_TEST_SUITE_P(RingBufferParamFixture);

TYPED_TEST_P(RingBufferParamFixture, initialCondition)
{
    static constexpr int magnitude = 4;

    jaxos::data_struct::ring<TypeParam,magnitude> ring;

    ASSERT_EQ(1<<magnitude, ring.capacity());
    ASSERT_EQ(0, ring.count());
    ASSERT_FALSE(ring.full());
    ASSERT_TRUE(ring.empty());
}

TYPED_TEST_P(RingBufferParamFixture, fillBufferTest)
{
    ASSERT_EQ(this->ring.capacity(), this->ring.count());
    ASSERT_TRUE(this->ring.full());
    ASSERT_FALSE(this->ring.empty());

    std::cout << "------ Ensuring another addition is not possible.\n";
    ASSERT_FALSE(this->ring.push_front(99));
}

TYPED_TEST_P(RingBufferParamFixture, consumeBufferTest)
{
    std::cout << "\n---- Consuming until queue is empty.\n";
    while (!this->ring.empty())
    {
        auto item = this->ring.peek();
        this->ring.pop_back();
    }

    ASSERT_EQ(0, this->ring.count());
    ASSERT_FALSE(this->ring.full());
    ASSERT_TRUE(this->ring.empty());

    std::cout << "------ Ensuring no more items can be retrieved.\n";

    ASSERT_FALSE(this->ring.pop_back());

    EXPECT_THROW(this->ring.peek(), const char *);

    std::cout << "---- Ensure we can add again.\n";
    this->fill_buffer();

    ASSERT_EQ(this->ring.capacity(), this->ring.count());
    ASSERT_TRUE(this->ring.full());
    ASSERT_FALSE(this->ring.empty());
}

TYPED_TEST_P(RingBufferParamFixture, modifyTest)
{
    this->ring.peek() = 333;
    ASSERT_EQ(333, this->ring.peek());

    auto & last = this->ring.peek(this->ring.count()-1);
    last = 42;
    ASSERT_EQ(42, this->ring.peek(this->ring.count()-1));

    for (int i = 0; i != this->ring.count(); ++i)
    {
        auto &var = this->ring.peek(i);
        var = 666+i;
        ASSERT_EQ(666+i, this->ring.peek(i));
    }
}

TYPED_TEST_P(RingBufferParamFixture, negativeOffsetTest)
{
    for (; !this->ring.empty(); )
        this->ring.pop_back();

    this->fill_buffer();

    for (int i = 3; i >= -3; --i)
    {
        auto &var = this->ring.peek(i);

        std::cout << "peeked :" << var << "\n";

        //ASSERT_EQ(666+i, this->ring.peek(i));
    }
}

REGISTER_TYPED_TEST_SUITE_P(RingBufferParamFixture,
    initialCondition, fillBufferTest, consumeBufferTest, modifyTest,
    negativeOffsetTest);

using Types = testing::Types<int, long long, std::string>;
INSTANTIATE_TYPED_TEST_SUITE_P(TestPrefix, RingBufferParamFixture, Types);
