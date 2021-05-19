#include "pch.h"

#include "file_util/file_util.h"
#include "file_util/file_util.cpp"

TEST(file_util, EpochToDate) {
    EXPECT_EQ(file_util::EpochToDate("1621448661"), "2021-05-19 14:24:21");
}

TEST(file_util, FileNameFromPath) {
    EXPECT_EQ(file_util::FileNameFromPath("N:\\cpp\\file.txt"), "file.txt");
}

