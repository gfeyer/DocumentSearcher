#include "pch.h"

#include "file_util/file_util.h"
#include "file_util/file_util.cpp"

TEST(file_util, epoch_to_date) {
    EXPECT_EQ(file_util::epoch_to_date("1621448661"),"2021-05-19 14:24:21");
}

