#include "data_loader.h"
#include "github_client.h"
#include "repository_analyzer.h"
#include "repository_parser.h"
#include "repository_store.h"

#include <cstdint>
#include <exception>
#include <filesystem>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
    /**
     * @brief Prints the help message.
     * @param executable The name of the executable.
     */
    void print_help(const char* executable) {
        std::cout
            << "CS3460 GitHub Crawler\n\n"
            << "Milestone 2 analysis:\n"
            << "  " << executable
            << " [options]\n\n"
            << "Options:\n"
            << "  --input FILE       Dataset path (default: repositories.json)\n"
            << "  --min-stars N      Minimum star count (default: 0)\n"
            << "  --language LANG    Exact language filter (default: any)\n"
            << "  --sort KEY         stars, forks, or updated (default: stars)\n"
            << "  --top N            Maximum rows to select (default: 10)\n"
            << "  --format FORMAT    json or csv (default: json)\n"
            << "  --output FILE      Output path (default: selected.json/csv)\n"
            << "  --help             Show this help message\n\n"
            << "Milestone 1 collection:\n"
            << "  " << executable << " collect [SEARCH_CRITERION...]\n\n"
            << "Examples:\n"
            << "  " << executable
            << " --min-stars 1000 --sort stars --top 5\n"
            << "  " << executable
            << " --language C++ --sort forks --top 10 --format csv\n"
            << "  " << executable
            << " collect \"stars:>1000\"\n";
    }

    /**
     * @brief Requires a value for a given option.
     * @param index The current index in the argument vector.
     * @param argc The number of arguments.
     * @param argv The argument vector.
     * @param option The option for which to require a value.
     * @return The required value.
     */
    std::string require_value(
        int& index,
        int argc,
        char* argv[],
        const std::string& option) {

        if (index + 1 >= argc) {
            throw std::invalid_argument(
                "Missing value after " + option
            );
        }

        ++index;
        return argv[index];
    }

    /**
     * @brief Parses a string into a uint64_t.
     * @param text The string to parse.
     * @param option The option for which to parse the value.
     * @return The parsed value.
     */
    std::uint64_t parse_uint64(
        const std::string& text,
        const std::string& option) {

        if (text.empty() || text.front() == '-') {
            throw std::invalid_argument(
                "Invalid non-negative integer for " + option + ": " + text
            );
        }

        std::size_t parsed_characters = 0;
        const unsigned long long value =
            std::stoull(text, &parsed_characters);

        if (parsed_characters != text.size()) {
            throw std::invalid_argument(
                "Invalid integer for " + option + ": " + text
            );
        }

        if (value > std::numeric_limits<std::uint64_t>::max()) {
            throw std::invalid_argument(
                "Integer is too large for " + option
            );
        }

        return static_cast<std::uint64_t>(value);
    }

    /**
     * @brief Parses a string into a size_t.
     * @param text The string to parse.
     * @param option The option for which to parse the value.
     * @return The parsed value.
     */
    std::size_t parse_size(
        const std::string& text,
        const std::string& option) {

        const std::uint64_t value = parse_uint64(text, option);

        if (value > std::numeric_limits<std::size_t>::max()) {
            throw std::invalid_argument(
                "Value is too large for " + option
            );
        }

        return static_cast<std::size_t>(value);
    }

    /**
     * @brief Runs the repository collection phase.
     * @param argc The number of arguments.
     * @param argv The argument vector.
     * @return The exit code.
     */
    int run_collection(int argc, char* argv[]) {
        SearchOptions options;
        options.query = "stars:>0";
        options.count = 100;

        // Everything after "collect" forms the optional GitHub query.
        if (argc > 2) {
            options.query.clear();

            for (int i = 2; i < argc; ++i) {
                if (!options.query.empty()) {
                    options.query += ' ';
                }

                options.query += argv[i];
            }
        }

        std::cout << "Milestone 1: GitHub collection\n";
        std::cout << "Search criterion: "
                << options.query << '\n';
        std::cout << "Requested repositories: "
                << options.count << "\n\n";

        GitHubClient client;
        const auto response = client.searchRepositories(options);

        if (!response) {
            return 1;
        }

        RepositoryParser parser;
        const auto repositories = parser.parse(*response);

        RepositoryStore store;
        store.print(repositories);
        store.save(repositories, "repositories.json");

        std::cout << "\nCollection complete.\n";
        std::cout << "Repositories collected: "
                << repositories.size() << '\n';
        std::cout << "Dataset saved to: repositories.json\n";

        return 0;
    }

    /**
     * @brief Runs the repository analysis phase.
     * @param argc The number of arguments.
     * @param argv The argument vector.
     * @return The exit code.
     */
    int run_analysis(int argc, char* argv[]) {
        AnalysisOptions options;

        std::filesystem::path input_path = "repositories.json";
        std::filesystem::path output_path;
        std::string format = "json";

        // Parse analysis options from command-line arguments.
        for (int i = 1; i < argc; ++i) {
            const std::string arg = argv[i];

            if (arg == "--help") {
                print_help(argv[0]);
                return 0;
            }
            else if (arg == "--input") {
                input_path = require_value(i, argc, argv, arg);
            }
            else if (arg == "--min-stars") {
                options.min_stars = parse_uint64(
                    require_value(i, argc, argv, arg), arg
                );
            }
            else if (arg == "--language") {
                options.language = require_value(i, argc, argv, arg);
            }
            else if (arg == "--sort") {
                options.sort_key = require_value(i, argc, argv, arg);
            }
            else if (arg == "--top") {
                options.top_n = parse_size(
                    require_value(i, argc, argv, arg), arg
                );
            }
            else if (arg == "--format") {
                format = require_value(i, argc, argv, arg);
            }
            else if (arg == "--output") {
                output_path = require_value(i, argc, argv, arg);
            }
            else {
                throw std::invalid_argument(
                    "Unknown option: " + arg +
                    ". Use --help to see available options."
                );
            }
        }

        if (format != "json" && format != "csv") {
            throw std::invalid_argument(
                "Invalid output format. Use json or csv."
            );
        }

        if (output_path.empty()) {
            output_path = (format == "json")
                ? "selected.json"
                : "selected.csv";
        }

        // Load the existing local dataset.
        const std::vector<Repository> repositories =
            load_repositories(input_path);

        std::cout << "CS3460 GitHub Crawler - Milestone 2\n";
        std::cout << "Dataset: " << input_path.string() << '\n';
        std::cout << "Repositories loaded: "
                << repositories.size() << '\n';
        std::cout << "Filter: stars >= "
                << options.min_stars << '\n';
        std::cout << "Language: "
                << (options.language.empty()
                    ? "any"
                    : options.language)
                << '\n';
        std::cout << "Sort: " << options.sort_key
                << " (descending)\n";
        std::cout << "Top N: " << options.top_n << "\n\n";

        // Filter, sort, select Top N.
        RepositoryAnalyzer analyzer;
        const std::vector<Repository> selected =
            analyzer.analyze(repositories, options);

        // Report and export the exact same selected vector.
        RepositoryStore store;
        store.printTable(selected);

        if (format == "json") {
            store.save(selected, output_path.string());
        }
        else {
            store.saveCsv(selected, output_path.string());
        }

        std::cout << "\nSelected repositories: "
                << selected.size() << '\n';
        std::cout << "Export format: " << format << '\n';
        std::cout << "Exported to: "
                << output_path.string() << '\n';

        return 0;
    }
}

/**
 * @brief The main function.
 * @param argc The number of arguments.
 * @param argv The argument vector.
 * @return The exit code.
 */
int main(int argc, char* argv[]) {
    try {
        if (argc > 1 && std::string(argv[1]) == "collect") {
            return run_collection(argc, argv);
        }

        if (argc == 1) {
            // Show help for an explicit request only. A normal run
            // executes the default analysis.
        }

        return run_analysis(argc, argv);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
