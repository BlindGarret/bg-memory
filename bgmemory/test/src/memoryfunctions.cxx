#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "bgmemory/memoryfunctions.hxx"

TEST(allocateAlligned, testing) {
    bg::allocateAlligned(0, 0);
    ASSERT_EQ(1, 1);
}