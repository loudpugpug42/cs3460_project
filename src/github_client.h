#pragma once

#include "repository.h"

#include <optional>
#include <string>

/**
 * @brief A class for interacting with the GitHub API.
 */
class GitHubClient {
public:
    std::optional<std::string> searchRepositories(
        const SearchOptions& options) const;
};
