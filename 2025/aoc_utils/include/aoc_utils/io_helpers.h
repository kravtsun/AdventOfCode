// This software was partially written using Suggestions from GitHub Copilot.
#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <filesystem>

namespace aoc_utils {

/// Reads all lines from a file and returns them as a vector of strings.
static inline auto read_lines(const std::filesystem::path &filepath) {
    std::ifstream fin{filepath};
    if (!fin.is_open()) {
        throw std::runtime_error("Failed to open the file: " + filepath.string());
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) {
        lines.push_back(line);
    }
    return lines;
}

static inline int char2digit(char c) {
    return c - '0';
}

static inline auto get_input_filepath(const std::string &filename, int itask) {
    std::ostringstream os;
    os << std::setw(2) << std::setfill('0') << itask;
    return std::filesystem::path(CMAKE_SOURCE_DIR) / ("solution" + os.str()) / filename;
}

static const std::string EXAMPLE_INPUT_FILE = "example_input.txt";
static const std::string INPUT_FILE = "input.txt";

static inline auto get_example_input_filepath(int itask) {
    return get_input_filepath(EXAMPLE_INPUT_FILE, itask);
}

static inline auto get_input_filepath(int itask) {
    return get_input_filepath(EXAMPLE_INPUT_FILE, itask);
}

} // namespace aoc_utils
