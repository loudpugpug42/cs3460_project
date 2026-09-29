#pragma once

#include "repository.h"

#include <string>
#include <vector>

class RepositoryStore {
public:
    void print(const std::vector<Repository>& repos) const;

    void save(
        const std::vector<Repository>& repos,
        const std::string& filename) const;
};
