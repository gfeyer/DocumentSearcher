#include "gtest/gtest.h"

int main(int argc, char** argv)
{
    //--gtest_filter=*Special*  in Properties->CommandArguments

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}