#include "gtest/gtest.h"

#include "file_util/file_util.h"
#include "file_util/file_util.cpp"

TEST(file_util, epoch_to_date) {

    EXPECT_EQ(file_util::epoch_to_date("1612242000"), "2021-02-02 00:00:00");
}