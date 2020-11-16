// Copyright [2020] BlindGarret<lroe2930@gmail.com>
#include "bgmemory/pointers/MutableSharedPtr.hxx"
#include "bgmemory/pointers/MutableSharedPtr.hxx"
#include "bgmemory/pointers/SharedPtrMutator.hxx"
#include "./TestHelpers.hxx"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(mutable_weak_ptr,
     Constructor_CopiedFromSharedPtr_DoesntDeleteSharedPtrAfterFallingOutOfScope)
{
    CountableTestDeleter<int>::reset();

    bg::MutableSharedPtr<int> p(new int(3), new CountableTestDeleter<int>());
    {
        bg::MutableWeakPtr<int> w = p;
    }

    ASSERT_EQ(0, CountableTestDeleter<int>::getDeleteCount());
}

TEST(mutable_weak_ptr,
     Constructor_AssignedFromSharedPtr_DeletesWhenWeakPtrIsntOutOfScope)
{
    CountableTestDeleter<int>::reset();

    bg::MutableWeakPtr<int> w;
    {
        bg::MutableSharedPtr<int> p(new int(3), new CountableTestDeleter<int>());
        w = p;
    }

    ASSERT_EQ(1, CountableTestDeleter<int>::getDeleteCount());
}

//************************
// Expired
//************************

TEST(mutable_weak_ptr,
     Expired_CalledWithActiveSharedPtr_ReturnsExpected)
{
    bool expected = false;

    bg::MutableSharedPtr<int> p(new int(3));
    bg::MutableWeakPtr<int> w = p;

    ASSERT_EQ(expected, w.expired());
}

TEST(mutable_weak_ptr,
     Expired_CalledWithNonActiveSharedPtr_ReturnsExpected)
{
    bool expected = true;

    bg::MutableWeakPtr<int> w;
    {
        bg::MutableSharedPtr<int> p(new int(3));
        w = p;
    }

    ASSERT_EQ(expected, w.expired());
}

//************************
// Lock
//************************

TEST(mutable_weak_ptr,
     Lock_CalledWithUnexpiredPointer_ReturnsActiveSharedPointerWithExpectedPayload)
{
    int expected = 3;

    bg::MutableSharedPtr<int> p(new int(expected));
    bg::MutableWeakPtr<int> w = p;
    auto p2 = w.lock();

    ASSERT_EQ(expected, *p2.get());
}

TEST(mutable_weak_ptr,
     Lock_CalledWithExpiredPointer_ReturnsInactiveSharedPointer)
{
    int *expected = nullptr;

    bg::MutableWeakPtr<int> w;
    {
        bg::MutableSharedPtr<int> p(new int(3));
        w = p;
    }
    auto p2 = w.lock();

    ASSERT_EQ(expected, p2.get());
}

//************************
// Reset
//************************

TEST(mutable_weak_ptr,
     Reset_Called_SetsSharedPtrToExpired)
{
    int *expected = nullptr;

    bg::MutableSharedPtr<int> p(new int(3));
    bg::MutableWeakPtr<int> w = p;
    w.reset();
    auto p2 = w.lock();

    ASSERT_EQ(expected, p2.get());
}

//************************
// UseCount
//************************

TEST(mutable_weak_ptr,
     UseCount_CalledWithMultipleSharedAndWeakPointersAttached_ReturnsExpected)
{
    int expected = 5;

    // 3 shared pointers
    bg::MutableSharedPtr<int> p(new int(3));
    auto p2 = p;
    auto p3 = p;
    // 4 weak pointers, and 2 additional shared pointers via lock.
    bg::MutableWeakPtr<int> w = p;
    auto w2 = w;
    auto w3 = w;
    auto w4 = w;
    auto p4 = w.lock();
    auto p5 = w4.lock();

    ASSERT_EQ(expected, w2.useCount());
}

//************************
// Swap
//************************

TEST(mutable_weak_ptr,
     Swap_CalledWithActiveSharedPointers_SwapsValuesAsExpected)
{
    int expectedValueOne = 5;
    int expectedValueTwo = 42;

    bg::MutableSharedPtr<int> p(new int(expectedValueOne));
    bg::MutableSharedPtr<int> p2(new int(expectedValueTwo));
    bg::MutableWeakPtr<int> w = p;
    bg::MutableWeakPtr<int> w2 = p2;
    w.swap(w2);

    ASSERT_EQ(expectedValueOne, *w2.lock().get());
    ASSERT_EQ(expectedValueTwo, *w.lock().get());
}
