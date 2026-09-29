#pragma once

#include "repository.h"

#include <optional>
#include <string>

class GitHubClient {
public:
    std::optional<std::string> searchRepositories(
        const SearchOptions& options) const;
};
