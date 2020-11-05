// Copyright [2019] BlindGarret<lroe2930@gmail.com>
#include "bgmemory/pointers/MutableUniquePtr.hxx"
#include "./TestHelpers.hxx"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <iostream>

TEST(mutable_unique_ptr,
     DefaultConstructor_Called_ConstructsDeleter) {
  CountableTestDeleter::reset();

  bg::MutableUniquePtr<int, CountableTestDeleter> p;

  ASSERT_EQ(1, CountableTestDeleter::getConstructCount());
}

TEST(mutable_unique_ptr,
     DefaultConstructor_CalledWithThenDestructed_CallsDeleter) {
  CountableTestDeleter::reset();

  {  bg::MutableUniquePtr<int, CountableTestDeleter> p; }

  ASSERT_EQ(1, CountableTestDeleter::getDeleteCount());
}

TEST(mutable_unique_ptr,
     NullPtrConstructor_Called_ConstructsDeleter) {
  CountableTestDeleter::reset();

  bg::MutableUniquePtr<int, CountableTestDeleter> p(nullptr);

  ASSERT_EQ(1, CountableTestDeleter::getConstructCount());
}

TEST(mutable_unique_ptr,
     NullPtrConstructor_CalledWithThenDestructed_CallsDeleter) {
  CountableTestDeleter::reset();

  {  bg::MutableUniquePtr<int, CountableTestDeleter> p(nullptr); }

  ASSERT_EQ(1, CountableTestDeleter::getDeleteCount());
}

//---------------------
//  Pointer constructor
//---------------------

TEST(mutable_unique_ptr,
     PointerConstructor_Called_DoesNotConstructAdditionalObjects) {
  TrackedDeletableTestObject::reset();

  bg::MutableUniquePtr<TrackedDeletableTestObject> p(new TrackedDeletableTestObject());

  ASSERT_EQ(1, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_unique_ptr,
     PointerConstructor_CalledThenDestructed_DestructsTheObject) {
  TrackedDeletableTestObject::reset();

  { bg::MutableUniquePtr<TrackedDeletableTestObject> p(new TrackedDeletableTestObject()); }

  ASSERT_EQ(0, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_unique_ptr,
     PointerConstructor_Called_SetsCorrectPointerValue) {
  auto raw = new int();

  bg::MutableUniquePtr<int> p(raw);

  ASSERT_EQ(raw, p.get());
}

TEST(mutable_unique_ptr,
     PointerConstructor_Called_SetsCorrectValue) {
  const int expected = 42;
  auto raw = new int(expected);

  bg::MutableUniquePtr<int> p(raw);

  ASSERT_EQ(expected, *p);
}

TEST(mutable_unique_ptr,
     PointerConstructor_Called_SetsBoolToExpected) {
  const bool expected = true;

  bg::MutableUniquePtr<int> p(new int());

  ASSERT_TRUE(p);
}

TEST(mutable_unique_ptr,
     PointerConstructor_CalledAndReleased_SetsBoolToExpected) {
  const bool expected = false;

  bg::MutableUniquePtr<int> p(new int());
  auto danglingPointer = p.release();

  ASSERT_FALSE(p);
  
  //Clean up the leak
  delete danglingPointer;
}

TEST(mutable_unique_ptr,
     PointerConstructor_Called_ReleaseReturnsExpected) {
  auto raw = new int(3);

  bg::MutableUniquePtr<int> p(raw);
  auto released = p.release();

  ASSERT_EQ(raw, released);
  
  //Clean up the leak
  delete released;
}

TEST(mutable_unique_ptr,
     PointerConstructor_Called_AllowsForMethodAccess) {
  const int expected = 42;
  bg::MutableUniquePtr<SimpleTestObject> p(new SimpleTestObject(expected));

  ASSERT_EQ(expected, p->GetValue());
}

//---------------------
//  Pointer RValue Deleter constructor
//---------------------

TEST(mutable_unique_ptr,
     PointerRValueDeleterConstructor_Called_DoesNotConstructAdditionalObjects) {
  TrackedDeletableTestObject::reset();

  bg::MutableUniquePtr<TrackedDeletableTestObject, IdTestDeleter<TrackedDeletableTestObject>> p(new TrackedDeletableTestObject(), IdTestDeleter<TrackedDeletableTestObject>());

  ASSERT_EQ(1, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_unique_ptr,
     PointerRValueDeleterConstructor_CalledThenDestructed_DestructsTheObject) {
  TrackedDeletableTestObject::reset();

  { bg::MutableUniquePtr<TrackedDeletableTestObject, IdTestDeleter<TrackedDeletableTestObject>> p(new TrackedDeletableTestObject(), IdTestDeleter<TrackedDeletableTestObject>()); }

  ASSERT_EQ(0, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_unique_ptr,
     PointerRValueDeleterConstructor_Called_SetsCorrectPointerValue) {
  auto raw = new int();

  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(raw, IdTestDeleter<int>());

  ASSERT_EQ(raw, p.get());
}

TEST(mutable_unique_ptr,
     PointerRValueDeleterConstructor_Called_SetsCorrectValue) {
  const int expected = 42;
  auto raw = new int(expected);

  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(raw, IdTestDeleter<int>());

  ASSERT_EQ(expected, *p);
}

TEST(mutable_unique_ptr,
     PointerRValueDeleterConstructor_Called_SetsBoolToExpected) {
  const bool expected = true;

  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(new int(), IdTestDeleter<int>());

  ASSERT_TRUE(p);
}

TEST(mutable_unique_ptr,
     PointerRValueDeleterConstructor_CalledAndReleased_SetsBoolToExpected) {
  const bool expected = false;

  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(new int(), IdTestDeleter<int>());
  auto released = p.release();

  ASSERT_FALSE(p);

  //Clean up the leak
  delete released;
}

TEST(mutable_unique_ptr,
     PointerRValueDeleterConstructor_Called_ReleaseReturnsExpected) {
  auto raw = new int(3);

  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(raw, IdTestDeleter<int>());
  auto released = p.release();

  ASSERT_EQ(raw, released);
  
  //Clean up the leak
  delete released;
}

TEST(mutable_unique_ptr,
     PointerRValueDeleterConstructor_Called_AllowsForMethodAccess) {
  const int expected = 42;
  bg::MutableUniquePtr<SimpleTestObject, IdTestDeleter<SimpleTestObject>> p(new SimpleTestObject(expected), IdTestDeleter<SimpleTestObject>());

  ASSERT_EQ(expected, p->GetValue());
}

TEST(mutable_unique_ptr,
     PointerRValueDeleterConstructor_Called_CanGatherExpectedDeleter) {
  IdTestDeleter<int>::reset();
  const int expected = 0;

  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(new int(), IdTestDeleter<int>());

  ASSERT_EQ(expected, p.getDeleter().getId());
}

TEST(mutable_unique_ptr,
     PointerRValueDeleterConstructor_Called_CanGatherExpectedConstDeleter) {
  IdTestDeleter<int>::reset();
  const int expected = 0;

  const bg::MutableUniquePtr<int, IdTestDeleter<int>> p(new int(), IdTestDeleter<int>());

  ASSERT_EQ(expected, p.getDeleter().getId());
}

//---------------------
//  Pointer Const Deleter constructor
//---------------------

TEST(mutable_unique_ptr,
     PointerConstDeleterConstructor_Called_DoesNotConstructAdditionalObjects) {
  TrackedDeletableTestObject::reset();

  const IdTestDeleter<TrackedDeletableTestObject> d;
  bg::MutableUniquePtr<TrackedDeletableTestObject, IdTestDeleter<TrackedDeletableTestObject>> p(new TrackedDeletableTestObject(), d);

  ASSERT_EQ(1, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_unique_ptr,
     PointerConstDeleterConstructor_CalledThenDestructed_DestructsTheObject) {
  TrackedDeletableTestObject::reset();

  { 
  const IdTestDeleter<TrackedDeletableTestObject> d;
    bg::MutableUniquePtr<TrackedDeletableTestObject, IdTestDeleter<TrackedDeletableTestObject>> p(new TrackedDeletableTestObject(), d); 
    }

  ASSERT_EQ(0, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_unique_ptr,
     PointerConstDeleterConstructor_Called_SetsCorrectPointeConst) {
  auto raw = new int();

  const IdTestDeleter<int> d;
  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(raw, d);

  ASSERT_EQ(raw, p.get());
}

TEST(mutable_unique_ptr,
     PointerConstDeleterConstructor_Called_SetsCorrectValue) {
  const int expected = 42;
  auto raw = new int(expected);

  const IdTestDeleter<int> d;
  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(raw, d);

  ASSERT_EQ(expected, *p);
}

TEST(mutable_unique_ptr,
     PointerConstDeleterConstructor_Called_SetsBoolToExpected) {
  const bool expected = true;

  const IdTestDeleter<int> d;
  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(new int(), d);

  ASSERT_TRUE(p);
}

TEST(mutable_unique_ptr,
     PointerConstDeleterConstructor_CalledAndReleased_SetsBoolToExpected) {
  const bool expected = false;

  const IdTestDeleter<int> d;
  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(new int(), d);
  auto released = p.release();

  ASSERT_FALSE(p);
  
  //Clean up the leak
  delete released;
}

TEST(mutable_unique_ptr,
     PointerConstDeleterConstructor_Called_ReleaseReturnsExpected) {
  auto raw = new int(3);

  const IdTestDeleter<int> d;
  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(raw, d);
  auto released = p.release();

  ASSERT_EQ(raw, released);
  
  //Clean up the leak
  delete released;
}

TEST(mutable_unique_ptr,
     PointerConstDeleterConstructor_Called_AllowsForMethodAccess) {
  const int expected = 42;

  const IdTestDeleter<SimpleTestObject> d;
  bg::MutableUniquePtr<SimpleTestObject, IdTestDeleter<SimpleTestObject>> p(new SimpleTestObject(expected), d);

  ASSERT_EQ(expected, p->GetValue());
}

TEST(mutable_unique_ptr,
     PointerConstDeleterConstructor_Called_CanGatherExpectedDeleter) {
  IdTestDeleter<int>::reset();
  const int expected = 0;

  const IdTestDeleter<int> d;
  bg::MutableUniquePtr<int, IdTestDeleter<int>> p(new int(), d);

  ASSERT_EQ(expected, p.getDeleter().getId());
}

TEST(mutable_unique_ptr,
     PointerConstDeleterConstructor_Called_CanGatherExpectedConstDeleter) {
  IdTestDeleter<int>::reset();
  const int expected = 0;

  const IdTestDeleter<int> d;
  const bg::MutableUniquePtr<int, IdTestDeleter<int>> p(new int(), d);

  ASSERT_EQ(expected, p.getDeleter().getId());
}

TEST(mutable_unique_ptr,
    Reset_CalledWithNullPtr_SetsExpected) {
  int* expected = nullptr;

  bg::MutableUniquePtr<int> p(new int());
  p.reset(nullptr);

  ASSERT_EQ(expected, p.get());
}

TEST(mutable_unique_ptr,
    Reset_CalledWithPtr_SetsExpected) {
  int* expected = new int();

  bg::MutableUniquePtr<int> p(new int());
  p.reset(expected);

  ASSERT_EQ(expected, p.get());
}

TEST(mutable_unique_ptr,
    Reset_CalledWithTrackableObject_DeletesOriginalObjectCorrectly) {
  TrackedDeletableTestObject::reset();
  const int expected = 1; // 2 objects are created. then one is deleted.

  auto secondObject = new TrackedDeletableTestObject();
  bg::MutableUniquePtr<TrackedDeletableTestObject> p(new TrackedDeletableTestObject());
  ASSERT_EQ(expected + 1, TrackedDeletableTestObject::getLiveObjectCount());
  p.reset(secondObject);

  ASSERT_EQ(expected, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_unique_ptr,
    Release_Calle_SetsManagedToNullPtr) {
  int* expected = nullptr;

  bg::MutableUniquePtr<int> p(new int());
  auto released = p.release();

  ASSERT_EQ(expected, p.get());
  
  //Clean up the leak
  delete released;
}

TEST(mutable_unique_ptr,
    Mutate_CalledWithNullPtr_SetsExpected) {
  int* expected = nullptr;

  bg::MutableUniquePtr<int> p(new int());
  p.mutate(nullptr);

  ASSERT_EQ(expected, p.get());
}

TEST(mutable_unique_ptr,
    Mutate_CalledWithPtr_SetsExpected) {
  int* expected = new int();

  bg::MutableUniquePtr<int> p(new int());
  p.mutate(expected);

  ASSERT_EQ(expected, p.get());
}

TEST(mutable_unique_ptr,
    Mutate_CalledWithTrackableObject_DeletesOriginalObjectCorrectly) {
  TrackedDeletableTestObject::reset();
  const int expected = 1; // 2 objects are created. then one is deleted.

  auto secondObject = new TrackedDeletableTestObject();
  bg::MutableUniquePtr<TrackedDeletableTestObject> p(new TrackedDeletableTestObject());
  ASSERT_EQ(expected + 1, TrackedDeletableTestObject::getLiveObjectCount());
  p.mutate(secondObject);

  ASSERT_EQ(expected, TrackedDeletableTestObject::getLiveObjectCount());
}

TEST(mutable_unique_ptr,
    Swap_CalledWithSecondPointer_PointersSwapAsExpected) {
  TrackedDeletableTestObject::reset();
  const int first = 42;
  const int second = 24;

  bg::MutableUniquePtr<int> firstPointer(new int(first));
  bg::MutableUniquePtr<int> secondPointer(new int(second));
  firstPointer.swap(secondPointer);

  ASSERT_EQ(second, *firstPointer);
  ASSERT_EQ(first, *secondPointer);
}

TEST(mutable_unique_ptr,
    Swap_CalledWithFirstPointer_PointersSwapAsExpected) {
  TrackedDeletableTestObject::reset();
  const int first = 42;
  const int second = 24;

  bg::MutableUniquePtr<int> firstPointer(new int(first));
  bg::MutableUniquePtr<int> secondPointer(new int(second));
  secondPointer.swap(firstPointer);

  ASSERT_EQ(second, *firstPointer);
  ASSERT_EQ(first, *secondPointer);
}
