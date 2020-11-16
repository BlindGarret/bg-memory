// Copyright [2020] BlindGarret<lroe2930@gmail.com>
#include "bgmemory/pointers/MutableSharedPtr.hxx"
#include "bgmemory/pointers/SharedPtrMutator.hxx"
#include "./TestHelpers.hxx"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(mutable_shared_ptr,
     NullPtrConstructor_Called_DoesntConstructAdditionalDeleters)
{
  CountableTestDeleter<int>::reset();

  bg::MutableSharedPtr<int> p(nullptr, new CountableTestDeleter<int>());

  ASSERT_EQ(1, CountableTestDeleter<int>::getConstructCount());
}

TEST(mutable_shared_ptr,
     NullPtrConstructor_CalledWithThenDestructed_CallsDeleter)
{
  CountableTestDeleter<int>::reset();

  {
    bg::MutableSharedPtr<int> p(nullptr, new CountableTestDeleter<int>());
  }

  ASSERT_EQ(1, CountableTestDeleter<int>::getDeleteCount());
}

//---------------------
//  Pointer constructor
//---------------------

TEST(mutable_shared_ptr,
     PointerConstructor_Called_DoesNotConstructAdditionalObjects)
{
  TrackedDeletableTestObject::reset();

  bg::MutableSharedPtr<TrackedDeletableTestObject> p(new TrackedDeletableTestObject());

  ASSERT_EQ(1, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_shared_ptr,
     PointerConstructor_CalledThenDestructed_DestructsTheObject)
{
  TrackedDeletableTestObject::reset();

  {
    bg::MutableSharedPtr<TrackedDeletableTestObject> p(new TrackedDeletableTestObject());
  }

  ASSERT_EQ(0, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_shared_ptr,
     PointerConstructor_Called_SetsCorrectPointerValue)
{
  auto raw = new int();

  bg::MutableSharedPtr<int> p(raw);

  ASSERT_EQ(raw, p.get());
}

TEST(mutable_shared_ptr,
     PointerConstructor_Called_SetsCorrectValue)
{
  const int expected = 42;
  auto raw = new int(expected);

  bg::MutableSharedPtr<int> p(raw);

  ASSERT_EQ(expected, *p);
}

TEST(mutable_shared_ptr,
     PointerConstructor_Called_SetsBoolToExpected)
{
  const bool expected = true;

  bg::MutableSharedPtr<int> p(new int());

  ASSERT_TRUE(p);
}

TEST(mutable_shared_ptr,
     PointerConstructor_Called_AllowsForMethodAccess)
{
  const int expected = 42;
  bg::MutableSharedPtr<SimpleTestObject> p(new SimpleTestObject(expected));

  ASSERT_EQ(expected, p->GetValue());
}

//---------------------
//  Pointer RValue Deleter constructor
//---------------------

TEST(mutable_shared_ptr,
     PointerRValueDeleterConstructor_Called_DoesNotConstructAdditionalObjects)
{
  TrackedDeletableTestObject::reset();

  bg::MutableSharedPtr<TrackedDeletableTestObject> p(new TrackedDeletableTestObject(), new IdTestDeleter<TrackedDeletableTestObject>());

  ASSERT_EQ(1, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_shared_ptr,
     PointerRValueDeleterConstructor_CalledThenDestructed_DestructsTheObject)
{
  TrackedDeletableTestObject::reset();

  {
    bg::MutableSharedPtr<TrackedDeletableTestObject> p(new TrackedDeletableTestObject(), new IdTestDeleter<TrackedDeletableTestObject>());
  }

  ASSERT_EQ(0, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_shared_ptr,
     PointerRValueDeleterConstructor_Called_SetsCorrectPointerValue)
{
  auto raw = new int();

  bg::MutableSharedPtr<int> p(raw, new IdTestDeleter<int>());

  ASSERT_EQ(raw, p.get());
}

TEST(mutable_shared_ptr,
     PointerRValueDeleterConstructor_Called_SetsCorrectValue)
{
  const int expected = 42;
  auto raw = new int(expected);

  bg::MutableSharedPtr<int> p(raw, new IdTestDeleter<int>());

  ASSERT_EQ(expected, *p);
}

TEST(mutable_shared_ptr,
     PointerRValueDeleterConstructor_Called_SetsBoolToExpected)
{
  const bool expected = true;

  bg::MutableSharedPtr<int> p(new int(), new IdTestDeleter<int>());

  ASSERT_TRUE(p);
}

TEST(mutable_shared_ptr,
     PointerRValueDeleterConstructor_Called_AllowsForMethodAccess)
{
  const int expected = 42;
  bg::MutableSharedPtr<SimpleTestObject> p(new SimpleTestObject(expected), new IdTestDeleter<SimpleTestObject>());

  ASSERT_EQ(expected, p->GetValue());
}

TEST(mutable_shared_ptr,
     PointerRValueDeleterConstructor_Called_CanGatherExpectedDeleter)
{
  IdTestDeleter<int>::reset();
  const int expected = 0;

  bg::MutableSharedPtr<int> p(new int(), new IdTestDeleter<int>());

  ASSERT_EQ(expected, ((const IdTestDeleter<int> &)p.getDeleter()).getId());
}

TEST(mutable_shared_ptr,
     PointerRValueDeleterConstructor_Called_CanGatherExpectedConstDeleter)
{
  IdTestDeleter<int>::reset();
  const int expected = 0;

  const bg::MutableSharedPtr<int> p(new int(), new IdTestDeleter<int>());

  ASSERT_EQ(expected, ((const IdTestDeleter<int> &)p.getDeleter()).getId());
}

//---------------------
//  Reset
//---------------------

TEST(mutable_shared_ptr,
     Reset_CalledWithNullPtr_SetsExpected)
{
  int *expected = nullptr;

  bg::MutableSharedPtr<int> p(new int());
  p.reset(nullptr);

  ASSERT_EQ(expected, p.get());
}

TEST(mutable_shared_ptr,
     Reset_CalledWithPtr_SetsExpected)
{
  int *expected = new int();

  bg::MutableSharedPtr<int> p(new int());
  p.reset(expected);

  ASSERT_EQ(expected, p.get());
}

TEST(mutable_shared_ptr,
     Reset_CalledWithTrackableObject_DeletesOriginalObjectCorrectly)
{
  TrackedDeletableTestObject::reset();
  const int expected = 1; // 2 objects are created. then one is deleted.

  auto secondObject = new TrackedDeletableTestObject();
  bg::MutableSharedPtr<TrackedDeletableTestObject> p(new TrackedDeletableTestObject());
  ASSERT_EQ(expected + 1, TrackedDeletableTestObject::getLiveObjectCount());
  p.reset(secondObject);

  ASSERT_EQ(expected, TrackedDeletableTestObject::getLiveObjectCount());
}

//---------------------
//  Pointer Copy constructor
//---------------------

TEST(mutable_shared_ptr,
     PointerCopyConstructor_Copied_IncrementsCountAsExpected)
{
  TrackedDeletableTestObject::reset();
  const int expected = 2;

  bg::MutableSharedPtr<int> firstPointer(new int(1));
  auto secondPointer = firstPointer;

  ASSERT_EQ(firstPointer.useCount(), expected);
}

TEST(mutable_shared_ptr,
     PointerCopyConstructor_Copied_SetsSecondPointerValueToFirst)
{
  TrackedDeletableTestObject::reset();

  bg::MutableSharedPtr<int> firstPointer(new int(1));
  auto secondPointer = firstPointer;

  ASSERT_EQ(*firstPointer, *secondPointer);
}

TEST(mutable_shared_ptr,
     PointerCopyConstructor_Copied_OnlyConstructsSingleDeleter)
{
  CountableTestDeleter<int>::reset();

  bg::MutableSharedPtr<int> p(nullptr, new CountableTestDeleter<int>());
  auto p2 = p;

  ASSERT_EQ(1, CountableTestDeleter<int>::getConstructCount());
}

TEST(mutable_shared_ptr,
     PointerCopyConstructor_Copied_OnlyDeletesObjectOnce)
{
  CountableTestDeleter<int>::reset();

  {
    bg::MutableSharedPtr<int> p(nullptr, new CountableTestDeleter<int>());
    auto p2 = p;
  }

  ASSERT_EQ(1, CountableTestDeleter<int>::getDeleteCount());
}

//---------------------
//  Swap
//---------------------

TEST(mutable_shared_ptr,
     Swap_CalledWithSecondPointer_PointersSwapAsExpected)
{
  TrackedDeletableTestObject::reset();
  const int first = 42;
  const int second = 24;

  bg::MutableSharedPtr<int> firstPointer(new int(first));
  bg::MutableSharedPtr<int> secondPointer(new int(second));
  firstPointer.swap(secondPointer);

  ASSERT_EQ(second, *firstPointer);
  ASSERT_EQ(first, *secondPointer);
}

TEST(mutable_shared_ptr,
     Swap_CalledWithFirstPointer_PointersSwapAsExpected)
{
  TrackedDeletableTestObject::reset();
  const int first = 42;
  const int second = 24;

  bg::MutableSharedPtr<int> firstPointer(new int(first));
  bg::MutableSharedPtr<int> secondPointer(new int(second));
  secondPointer.swap(firstPointer);

  ASSERT_EQ(second, *firstPointer);
  ASSERT_EQ(first, *secondPointer);
}

TEST(mutable_shared_ptr,
     PointerCopyConstructor_CopiedAndSwapped_DidNotMutateValueForCopy)
{
  TrackedDeletableTestObject::reset();
  const int initialValue = 1;
  const int changedValue = 2;

  bg::MutableSharedPtr<int> firstPointer(new int(initialValue));
  auto secondPointer = firstPointer;
  bg::MutableSharedPtr<int> newPointer(new int(changedValue));
  firstPointer.swap(newPointer);

  ASSERT_EQ(changedValue, *firstPointer);
  ASSERT_EQ(initialValue, *secondPointer);
  ASSERT_EQ(initialValue, *newPointer);
}