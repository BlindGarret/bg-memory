// Copyright [2019] BlindGarret<lroe2930@gmail.com>
#include "bgmemory/pointers/MutableUniquePtr.hxx"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

class WatchableDeleter {
  static int deleteCount;

 public:
  constexpr WatchableDeleter() noexcept {}

  static int getCount() { return deleteCount; }

  static void reset() { deleteCount = 0; }

  void operator()(int* pointer) {
    WatchableDeleter::deleteCount++;
    if (pointer != nullptr) {
      delete pointer;
    }
  }
};

int WatchableDeleter::deleteCount;

TEST(mutable_unique_ptr,
     DefaultConstructor_CalledWithThenDestructed_CallsDeleter) {
  WatchableDeleter::reset();

  {  bg::MutableUniquePtr<int, WatchableDeleter> p; }

  ASSERT_EQ(1, WatchableDeleter::getCount());
}
