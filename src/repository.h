#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

/**
 * @brief Represents a GitHub repository.
 */
struct Repository {
    std::string owner;
    std::string name;
    std::string description;
    std::uint64_t stars{};
    std::uint64_t forks{};
    std::string language;
    std::uint64_t size{};
    std::string updated_at;
    std::string url;
};

/**
 * @brief Represents options for searching repositories.
 */
struct SearchOptions {
    std::string query;
    std::size_t count{100};
};

/**
 * @brief Represents options for analyzing repositories.
 */
struct AnalysisOptions {
    std::uint64_t min_stars{};
    std::string language;
    std::string sort_key{"stars"};
    std::size_t top_n{10};
};
