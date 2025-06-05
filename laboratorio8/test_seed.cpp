// test_seed.cpp
#include <gtest/gtest.h>
#include "random_utils.h"

TEST(SeedTest, GeneratesExpectedValues) {
    unsigned int seed = 12345;
    std::vector<uint32_t> expected = {
        3992670690, 3823185381, 1358822685, 561383553, 789925284
    };

    auto result = generateRandomSequence(seed, 5);

    EXPECT_EQ(result, expected);
}
