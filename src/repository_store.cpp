#include "repository_store.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using json = nlohmann::json;

void RepositoryStore::print(
    const std::vector<Repository>& repos) const {

    std::cout << "\n========================================\n";
    std::cout << "Repositories collected: "
              << repos.size()
              << '\n';
    std::cout << "========================================\n\n";

    for (std::size_t i = 0; i < repos.size(); ++i) {
        const Repository& r = repos[i];

        std::cout << "Repository " << (i + 1)
                  << " of " << repos.size() << '\n';

        std::cout << "Owner/Name: "
                  << r.owner << '/' << r.name << '\n';

        std::cout << "Description: "
                  << (r.description.empty()
                      ? "(none)"
                      : r.description)
                  << '\n';

        std::cout << "Stars: "
                  << r.stars
                  << '\n';

        std::cout << "Forks: "
                  << r.forks
                  << '\n';

        std::cout << "Language: "
                  << (r.language.empty()
                      ? "(none)"
                      : r.language)
                  << '\n';

        std::cout << "Size: "
                  << r.size
                  << " KB\n";

        std::cout << "Updated: "
                  << r.updated_at
                  << '\n';

        std::cout << "URL: "
                  << r.url
                  << '\n';

        std::cout << "----------------------------------------\n";
    }
}

void RepositoryStore::save(
    const std::vector<Repository>& repos,
    const std::string& filename) const {

    json out = json::array();

    for (const auto& r : repos) {
        out.push_back({
            {"owner", r.owner},
            {"name", r.name},
            {"description", r.description},
            {"stars", r.stars},
            {"forks", r.forks},
            {"language", r.language},
            {"size", r.size},
            {"updated_at", r.updated_at},
            {"url", r.url}
        });
    }

    std::ofstream file(filename);

    if (!file) {
        throw std::runtime_error(
            "Could not open output file: " + filename
        );
    }

    file << out.dump(2) << '\n';

    if (!file) {
        throw std::runtime_error(
            "Failed while writing output file: " + filename
        );
    }
}
