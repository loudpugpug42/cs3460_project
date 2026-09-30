#include "repository_parser.h"

#include <nlohmann/json.hpp>

#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using json = nlohmann::json;

/**
 * @brief Parses a JSON string containing GitHub repository data.
 * @param body The JSON string to parse.
 * @return A vector of Repository objects parsed from the JSON.
 * @throws std::runtime_error if the JSON is invalid or does not contain the expected structure.
 */
std::vector<Repository> RepositoryParser::parse(
    const std::string& body) const {

    try {
        const json root = json::parse(body);

        if (!root.contains("items") || !root.at("items").is_array()) {
            throw std::runtime_error(
                "GitHub response does not contain an items array."
            );
        }

        std::vector<Repository> repos;

        for (const auto& item : root.at("items")) {
            Repository r;

            r.owner =
                item.at("owner")
                    .at("login")
                    .get<std::string>();

            r.name =
                item.at("name")
                    .get<std::string>();

            r.description =
                item.at("description").is_null()
                    ? ""
                    : item.at("description").get<std::string>();

            r.stars =
                item.at("stargazers_count")
                    .get<std::uint64_t>();

            r.forks =
                item.at("forks_count")
                    .get<std::uint64_t>();

            r.language =
                item.at("language").is_null()
                    ? ""
                    : item.at("language").get<std::string>();

            r.size =
                item.at("size")
                    .get<std::uint64_t>();

            r.updated_at =
                item.at("updated_at")
                    .get<std::string>();

            r.url =
                item.at("html_url")
                    .get<std::string>();

            repos.push_back(std::move(r));
        }

        return repos;
    }
    catch (const json::exception& e) {
        throw std::runtime_error(
            std::string("Failed to parse GitHub JSON: ") + e.what()
        );
    }
}
