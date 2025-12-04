#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace aoc {

    [[nodiscard]] inline std::string read_file(std::string_view path) {
        std::ifstream in(std::string(path), std::ios::binary);
        if (!in) {
            throw std::runtime_error("cannot open: " + std::string(path));
        }

        in.seekg(0, std::ios::end);
        const auto size = in.tellg();
        if (size < 0) {
            throw std::runtime_error("tellg failed on: " + std::string(path));
        }

        std::string data(static_cast<std::size_t>(size), '\0');
        in.seekg(0, std::ios::beg);
        in.read(data.data(), static_cast<std::streamsize>(data.size()));
        return data;
    }

    template <typename T> inline void do_not_optimize_away(const T& value) {
#if defined(__clang__) || defined(__GNUG__)
        asm volatile("" : : "g"(value) : "memory");
#else
        (void) value;
#endif
    }

    template <typename T>
    concept Streamable = requires(std::ostream& os, const T& v) {
        { os << v } -> std::same_as<std::ostream&>;
    };

    template <typename S>
    concept AoCSolver =
        requires(S s, std::string_view sv) {
            { s.parse(sv) } -> std::same_as<void>;
            { s.part1() };
            { s.part2() };
        } && Streamable<decltype(std::declval<S>().part1())> &&
        Streamable<decltype(std::declval<S>().part2())>;

    template <AoCSolver Solver>
    void run_solver(std::string_view input, std::string_view input_path, int repeat) {
        using clock = std::chrono::steady_clock;

        Solver solver;
        solver.parse(input);

        do_not_optimize_away(solver.part1());
        do_not_optimize_away(solver.part2());

        const auto t0 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            auto r1 = solver.part1();
            auto r2 = solver.part2();
            do_not_optimize_away(r1);
            do_not_optimize_away(r2);
        }
        const auto t1 = clock::now();

        const auto ans1 = solver.part1();
        const auto ans2 = solver.part2();

        const std::chrono::duration<double, std::milli> dt = t1 - t0;
        const double avg_ms = dt.count() / static_cast<double>(repeat);

        std::cout << "Input: " << input_path << '\n';
        std::cout << "Part 1: " << ans1 << '\n';
        std::cout << "Part 2: " << ans2 << '\n';
        std::cout << "Avg over " << repeat << " runs: " << avg_ms << " ms/run\n";
    }

    using Invoker = void (*)(std::string_view input, std::string_view path, int repeat);

    template <AoCSolver Solver> constexpr Invoker make_invoker() {
        return +[](std::string_view input, std::string_view path, int repeat) {
            run_solver<Solver>(input, path, repeat);
        };
    }

    struct DayEntry {
        int day;
        Invoker invoke;
    };

} // namespace aoc
