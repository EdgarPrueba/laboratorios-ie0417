// random_utils.h
#pragma once
#include <vector>
#include <random>

std::vector<uint32_t> generateRandomSequence(unsigned int seed, int count) {
    std::mt19937 generator(seed);
    std::vector<uint32_t> result;
    for (int i = 0; i < count; ++i) {
        result.push_back(generator());
    }
    return result;
}
