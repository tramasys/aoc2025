#include "day01/solver.hpp"
#include "day02/solver.hpp"
#include "day03/solver.hpp"
#include "day04/solver.hpp"
#include "harness.hpp"

using namespace aoc;

constexpr int DEFAULT_REPEAT = 1000;

constexpr std::array day_table{
    DayEntry{.day = 1, .invoke = make_invoker<Day01>()},
    DayEntry{.day = 2, .invoke = make_invoker<Day02>()},
    DayEntry{.day = 3, .invoke = make_invoker<Day03>()},
    DayEntry{.day = 4, .invoke = make_invoker<Day04>()},
};

struct Args {
    int day = 1;
    int repeat = DEFAULT_REPEAT;
};

[[nodiscard]] Args parse_args(int argc, char** argv) {
    Args args{};

    if (argc > 1) {
        args.day = std::stoi(argv[1]);
    }

    if (argc > 2) {
        args.repeat = std::stoi(argv[2]);
    }

    return args;
}

[[nodiscard]] std::string make_input_path(const Args& args) {
    std::array<char, 16> buf{};
    const int len = std::snprintf(buf.data(), buf.size(), "day%02d", args.day);

    std::string path = "src/";
    path.append(buf.data(), static_cast<std::size_t>(len));
    path += "/input.txt";
    return path;
}

int main(int argc, char** argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    try {
        const auto args = parse_args(argc, argv);
        const auto path = make_input_path(args);
        const auto input = read_file(path);

        const auto* const iter = std::ranges::find_if(
            day_table, [day = args.day](const DayEntry& entry) { return entry.day == day; });

        if (iter == day_table.end()) {
            std::cerr << "No solver registered for day " << args.day << '\n';
            return 1;
        }

        iter->invoke(input, path, args.repeat);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
