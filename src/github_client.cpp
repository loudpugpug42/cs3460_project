#include "github_client.h"

#include <httplib.h>

#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

#ifdef _WIN32
#include <stdlib.h>
#endif

namespace {

/*
 * @brief Retrieves the value of an environment variable.
 * @param name The name of the environment variable.
 * @return The value of the environment variable, or std::nullopt if it is not set.
 */
std::optional<std::string> get_environment_variable(
    const char* name) {

// Use platform-specific methods to retrieve the environment variable.
#ifdef _WIN32
    char* raw_value = nullptr;
    size_t length = 0;

    if (_dupenv_s(&raw_value, &length, name) != 0 ||
        raw_value == nullptr) {
        std::free(raw_value);
        return std::nullopt;
    }

    std::string value(raw_value);
    std::free(raw_value);

#else
    const char* raw_value = std::getenv(name);

    if (raw_value == nullptr) {
        return std::nullopt;
    }

    std::string value(raw_value);
#endif

    if (value.empty()) {
        return std::nullopt;
    }

    return value;
}

} // namespace

/*
 * @brief Searches for GitHub repositories based on the provided options.
 * @param options The search options, including query and count.
 * @return The JSON response from the GitHub API as a string, or std::nullopt on error.
 */
std::optional<std::string> GitHubClient::searchRepositories(
    const SearchOptions& options) const {

    if (options.count == 0 || options.count > 100) {
        std::cerr
            << "Error: repository count must be between 1 and 100.\n";
        return std::nullopt;
    }

    httplib::Client client("https://api.github.com");

    const std::string path =
        "/search/repositories?q=" +
        httplib::encode_uri_component(options.query) +
        "&per_page=" +
        std::to_string(options.count);

    httplib::Headers headers{
        {"Accept", "application/vnd.github+json"},
        {"X-GitHub-Api-Version", "2026-03-10"},
        {"User-Agent", "cs3460-github-crawler"}
    };

    // Authentication is optional.
    if (const auto token = get_environment_variable("GITHUB_TOKEN")) {
        headers.emplace(
            "Authorization",
            "Bearer " + *token
        );
    }

    auto result = client.Get(path, headers);

    if (!result) {
        std::cerr
            << "GitHub request failed: "
            << httplib::to_string(result.error())
            << '\n';
        return std::nullopt;
    }

    if (result->status != 200) {
        std::cerr
            << "GitHub API returned HTTP "
            << result->status
            << '\n';

        if (!result->body.empty()) {
            std::cerr << "API response: " << result->body << '\n';
        }

        return std::nullopt;
    }

    return result->body;
}
