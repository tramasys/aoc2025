#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

struct Day01 {
    static constexpr int MOD = 100;

    struct Instr {
        int dir;
        std::int64_t dist;
    };

    std::vector<Instr> prog;

    void parse(std::string_view input) {
        prog.clear();
        prog.reserve(512);

        std::size_t i = 0;
        const std::size_t n = input.size();

        while (i < n) {
            while (i < n && (input[i] == '\n' || input[i] == '\r')) {
                ++i;
            }

            if (i >= n) {
                break;
            }

            char c = input[i++];
            if (c != 'L' && c != 'R') {
                while (i < n && input[i] != '\n' && input[i] != '\r') {
                    ++i;
                }
                continue;
            }

            int dir = (c == 'R') ? +1 : -1;

            while (i < n && input[i] == ' ') {
                ++i;
            }

            std::int64_t d = 0;
            while (i < n && input[i] >= '0' && input[i] <= '9') {
                d = (d * 10) + (input[i] - '0');
                ++i;
            }

            prog.push_back(Instr{.dir = dir, .dist = d});

            while (i < n && input[i] != '\n' && input[i] != '\r') {
                ++i;
            }
        }
    }

    static int norm(int pos) {
        pos %= MOD;
        if (pos < 0) {
            pos += MOD;
        }
        return pos;
    }

    [[nodiscard]] long long part1() const {
        int pos = 50;
        long long count = 0;

        for (const auto& ins : prog) {
            if (ins.dir > 0) {
                pos = norm(pos + static_cast<int>(ins.dist % MOD));
            } else {
                pos = norm(pos - static_cast<int>(ins.dist % MOD));
            }

            if (pos == 0) {
                ++count;
            }
        }

        return count;
    }

    static std::int64_t zeros_right(int pos, std::int64_t d) {
        if (d <= 0) {
            return 0;
        }

        pos = norm(pos);
        std::int64_t k0 = (MOD - pos) % MOD;
        if (k0 == 0) {
            k0 = MOD;
        }

        if (d < k0) {
            return 0;
        }

        return 1 + ((d - k0) / MOD);
    }

    static std::int64_t zeros_left(int pos, std::int64_t d) {
        if (d <= 0) {
            return 0;
        }

        pos = norm(pos);
        std::int64_t k0 = pos % MOD;
        if (k0 == 0) {
            k0 = MOD;
        }

        if (d < k0) {
            return 0;
        }

        return 1 + ((d - k0) / MOD);
    }

    [[nodiscard]] long long part2() const {
        int pos = 50;
        long long total = 0;

        for (const auto& ins : prog) {
            if (ins.dir > 0) {
                total += zeros_right(pos, ins.dist);
                pos = norm(pos + static_cast<int>(ins.dist % MOD));
            } else {
                total += zeros_left(pos, ins.dist);
                pos = norm(pos - static_cast<int>(ins.dist % MOD));
            }
        }

        return total;
    }
};
