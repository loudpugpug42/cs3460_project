#pragma once

#include "repository.h"

#include <string>
#include <vector>

/**
 * @brief A class for storing and managing a collection of GitHub repositories.
 */
class RepositoryStore {
public:
    void print(const std::vector<Repository>& repos) const;

    void save(
        const std::vector<Repository>& repos,
        const std::string& filename) const;
};
