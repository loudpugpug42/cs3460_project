#pragma once

#include "repository.h"

#include <vector>

class RepositoryAnalyzer {
public:
    std::vector<Repository> analyze(
        const std::vector<Repository>& input,
        const AnalysisOptions& options
    ) const;
};
