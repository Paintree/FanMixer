#include "CppUTest/TestHarness.h"

extern "C" {
    // includes
    #include <fan.h>
}

TEST_GROUP(FanLogicTest) {
    void setup() {}
    void teardown() {}
};

TEST(FanLogicTest, TestPwmOutput) {
    LONGS_EQUAL(100, fanCalculateOutputSpeed(0,0));
    LONGS_EQUAL(100, fanCalculateOutputSpeed(0,100));
    LONGS_EQUAL(100, fanCalculateOutputSpeed(100,0));
    for (int i = 1; i <= 100; i++) {
        LONGS_EQUAL(i, fanCalculateOutputSpeed(i,0));
        LONGS_EQUAL(i, fanCalculateOutputSpeed(0,i));
        if (i*2 <= 100)
            LONGS_EQUAL(2*i, fanCalculateOutputSpeed(i,i));
        else
            LONGS_EQUAL(100, fanCalculateOutputSpeed(i,i));
    }
}