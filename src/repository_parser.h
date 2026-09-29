#pragma once

#include "repository.h"

#include <string>
#include <vector>

class RepositoryParser {
public:
    std::vector<Repository> parse(const std::string& body) const;
};
