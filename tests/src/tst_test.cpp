#include "CppUTest/TestHarness.h"

extern "C" {
    // includes
}

TEST_GROUP(Tests) {
    void setup() {}
    void teardown() {}
};

TEST(Tests, Testing) {
    LONGS_EQUAL(0x00, 0x00);
}