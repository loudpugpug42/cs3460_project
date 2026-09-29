#include "github_client.h"

#include <httplib.h>

#include <cstdlib>
#include <iostream>
#include <string>

std::optional<std::string> GitHubClient::searchRepositories(
    const SearchOptions& options) const {

    if (options.count == 0 || options.count > 100) {
        std::cerr << "Error: repository count must be between 1 and 100.\n";
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

    if (const char* token = std::getenv("GITHUB_TOKEN")) {
        if (*token != '\0') {
            headers.emplace(
                "Authorization",
                std::string("Bearer ") + token
            );
        }
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
