#pragma once
#include <string_view>

struct Day04 {
    int dummy = 0;

    void parse(std::string_view input) {
        (void)input;
        dummy = 42;
    }

    int part1() const {
        return dummy;
    }

    long long part2() const {
        return 2LL * dummy;
    }
};
