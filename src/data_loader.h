#pragma once

#include "repository.h"

#include <filesystem>
#include <vector>

std::vector<Repository> load_repositories(
    const std::filesystem::path& path
);
