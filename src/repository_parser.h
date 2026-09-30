#pragma once

#include "repository.h"

#include <string>
#include <vector>

/**
 * @brief A class for parsing GitHub repository data.
 */
class RepositoryParser {
public:
    std::vector<Repository> parse(const std::string& body) const;
};
