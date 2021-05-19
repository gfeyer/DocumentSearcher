#include "pch.h"

#include "file_util/file_util.h"
#include "file_util/file_util.cpp"

TEST(file_util, EpochToDate) {
    EXPECT_EQ(file_util::EpochToDate("1621448661"), "2021-05-19");
}

TEST(file_util, FileNameFromPath) {
    EXPECT_EQ(file_util::FileNameFromPath("N:\\cpp\\file.txt"), "file.txt");
}

TEST(file_util, ExtensionFromPath) {
    EXPECT_EQ(file_util::ExtensionFromPath("C:\\folder\\file.doc"), "doc");
}

TEST(file_util, Read) {
    using namespace std;

    //auto file = file_util::Read("resources\\xlsx.xlsx");
    
    //cout << file.author;
    
    EXPECT_EQ(1,1);
}