#pragma once

#include "repository.h"

#include <string>
#include <vector>

/**
 * @brief Provides functionality to print and save repository data.
 */
class RepositoryStore {
public:
    // Collection output
    void print(const std::vector<Repository>& repos) const;

    void save(
        const std::vector<Repository>& repos,
        const std::string& filename
    ) const;

    // Analysis output
    void printTable(
        const std::vector<Repository>& repos
    ) const;

    void saveCsv(
        const std::vector<Repository>& repos,
        const std::string& filename
    ) const;
};
