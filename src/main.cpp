#include "github_client.h"
#include "repository_parser.h"
#include "repository_store.h"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        SearchOptions options;

        // Default search: 100 public repositories with at least one star.
        options.query = "stars:>0";
        options.count = 100;

        // Allow a custom search criterion from the command line.
        if (argc > 1) {
            options.query.clear();

            for (int i = 1; i < argc; ++i) {
                if (!options.query.empty()) {
                    options.query += ' ';
                }

                options.query += argv[i];
            }
        }

        std::cout << "GitHub Repository Crawler\n";
        std::cout << "Search criterion: "
                  << options.query << '\n';
        std::cout << "Requested repositories: "
                  << options.count << "\n\n";

        GitHubClient client;

        const auto response =
            client.searchRepositories(options);

        if (!response) {
            return 1;
        }

        RepositoryParser parser;

        const auto repositories =
            parser.parse(*response);

        RepositoryStore store;

        store.print(repositories);

        store.save(
            repositories,
            "repositories.json"
        );

        std::cout << "\n========================================\n";
        std::cout << "Collection complete.\n";
        std::cout << "Repositories collected: "
                  << repositories.size()
                  << '\n';
        std::cout << "Dataset saved to: repositories.json\n";
        std::cout << "========================================\n";

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: "
                  << e.what()
                  << '\n';

        return 1;
    }
}
