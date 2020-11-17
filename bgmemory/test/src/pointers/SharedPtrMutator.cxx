// Copyright [2020] BlindGarret<lroe2930@gmail.com>
#include "bgmemory/pointers/MutableSharedPtr.hxx"
#include "bgmemory/pointers/MutableSharedPtr.hxx"
#include "bgmemory/pointers/SharedPtrMutator.hxx"
#include "./TestHelpers.hxx"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>

//---------------------
//  Mutate
//---------------------

TEST(shared_ptr_mutator,
     Mutate_CalledWithNullSharedPtr_SetsExpected)
{
    int *expected = nullptr;

    bg::MutableSharedPtr<int> p(new int());
    bg::SharedPtrMutator<int> m(p);
    m.mutate(nullptr);

    ASSERT_EQ(expected, p.get());
}

TEST(shared_ptr_mutator,
     Mutate_CalledWithSharedPtr_SetsExpected)
{
    int *expected = new int();

    bg::MutableSharedPtr<int> p(new int());
    bg::SharedPtrMutator<int> m(p);
    m.mutate(expected);

    ASSERT_EQ(expected, p.get());
}

TEST(shared_ptr_mutator,
     Mutate_CalledWithSharedPtrTrackableObject_DeletesOriginalObjectCorrectly)
{
    TrackedDeletableTestObject::reset();
    const int expected = 1; // 2 objects are created. then one is deleted.

    auto secondObject = new TrackedDeletableTestObject();
    bg::MutableSharedPtr<TrackedDeletableTestObject> p(new TrackedDeletableTestObject());
    ASSERT_EQ(expected + 1, TrackedDeletableTestObject::getLiveObjectCount());
    bg::SharedPtrMutator<TrackedDeletableTestObject> m(p);
    m.mutate(secondObject);

    ASSERT_EQ(expected, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(shared_ptr_mutator,
     Mutate_SharedPointerCopyConstructorMutated_ChangesAllInstances)
{
    TrackedDeletableTestObject::reset();
    const int initialValue = 1;
    const int changedValue = 2;

    bg::MutableSharedPtr<int> firstPointer(new int(initialValue));
    auto secondPointer = firstPointer;
    bg::SharedPtrMutator<int> m(firstPointer);
    m.mutate(new int(changedValue));

    ASSERT_EQ(changedValue, *firstPointer);
    ASSERT_EQ(changedValue, *secondPointer);
}

TEST(shared_ptr_mutator,
     Mutate_CalledWithNullWeakPtr_SetsExpected)
{
    int *expected = nullptr;

    bg::MutableSharedPtr<int> p(new int());
    bg::MutableWeakPtr<int> w(p);
    bg::SharedPtrMutator<int> m(w);
    m.mutate(nullptr);

    ASSERT_EQ(expected, p.get());
}

TEST(shared_ptr_mutator,
     Mutate_CalledWithWeakPtr_SetsExpected)
{
    int *expected = new int();

    bg::MutableSharedPtr<int> p(new int());
    bg::MutableWeakPtr<int> w(p);
    bg::SharedPtrMutator<int> m(p);
    m.mutate(expected);

    ASSERT_EQ(expected, p.get());
}

TEST(shared_ptr_mutator,
     Mutate_CalledWithWeakPtrTrackableObject_DeletesOriginalObjectCorrectly)
{
    TrackedDeletableTestObject::reset();
    const int expected = 1; // 2 objects are created. then one is deleted.

    auto secondObject = new TrackedDeletableTestObject();
    bg::MutableSharedPtr<TrackedDeletableTestObject> p(new TrackedDeletableTestObject());
    bg::MutableWeakPtr<TrackedDeletableTestObject> w(p);
    ASSERT_EQ(expected + 1, TrackedDeletableTestObject::getLiveObjectCount());
    bg::SharedPtrMutator<TrackedDeletableTestObject> m(w);
    m.mutate(secondObject);

    ASSERT_EQ(expected, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(shared_ptr_mutator,
     Mutate_WeakPointerCopyConstructorMutated_ChangesAllInstances)
{
    TrackedDeletableTestObject::reset();
    const int initialValue = 1;
    const int changedValue = 2;

    bg::MutableSharedPtr<int> firstPointer(new int(initialValue));
    bg::MutableWeakPtr<int> w(firstPointer);
    auto secondPointer = firstPointer;
    bg::SharedPtrMutator<int> m(w);
    m.mutate(new int(changedValue));

    ASSERT_EQ(changedValue, *firstPointer);
    ASSERT_EQ(changedValue, *secondPointer);
}