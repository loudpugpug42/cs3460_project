#include "repository_store.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using json = nlohmann::json;

/**
 * @brief Prints information about a list of repositories.
 * @param repos The list of repositories to print.
 */
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

/**
 * @brief Saves a list of repositories to a JSON file.
 * @param repos The list of repositories to save.
 * @param filename The name of the file to save to.
 */
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

namespace {
    /**
    * @brief Escapes a string for CSV output.
    * @param value The string to escape.
    * @return The escaped string.
    */
    std::string escape_csv(const std::string& value) {
        if (value.find_first_of(",\"\r\n") == std::string::npos) {
            return value;
        }

        std::string escaped = "\"";

        for (char ch : value) {
            if (ch == '"') {
                escaped += "\"\"";
            }
            else {
                escaped += ch;
            }
        }

        escaped += '"';
        return escaped;
    }

    /**
    * @brief Writes a row to a CSV file.
    * @param file The file to write to.
    * @param fields The fields to write.
    */
    void write_csv_row(
        std::ofstream& file,
        const std::vector<std::string>& fields) {

        for (std::size_t i = 0; i < fields.size(); ++i) {
            if (i > 0) {
                file << ',';
            }

            file << escape_csv(fields[i]);
        }

        file << '\n';
    }
}

/**
 * @brief Prints a table of repositories.
 * @param repos The list of repositories to print.
 */
void RepositoryStore::printTable(
    const std::vector<Repository>& repos) const {

    if (repos.empty()) {
        std::cout << "No repositories matched the analysis criteria.\n";
        return;
    }

    std::cout
        << std::left
        << std::setw(45) << "OWNER/NAME"
        << std::right
        << std::setw(10) << "STARS"
        << std::setw(10) << "FORKS"
        << "  "
        << std::left
        << std::setw(18) << "LANGUAGE"
        << "UPDATED\n";

    std::cout << std::string(105, '-') << '\n';

    for (const Repository& r : repos) {
        const std::string owner_name = r.owner + "/" + r.name;

        std::cout
            << std::left
            << std::setw(45) << owner_name
            << std::right
            << std::setw(10) << r.stars
            << std::setw(10) << r.forks
            << "  "
            << std::left
            << std::setw(18)
            << (r.language.empty() ? "(none)" : r.language)
            << r.updated_at
            << '\n';
    }
}

/**
 * @brief Saves a list of repositories to a CSV file.
 * @param repos The list of repositories to save.
 * @param filename The name of the file to save to.
 */
void RepositoryStore::saveCsv(
    const std::vector<Repository>& repos,
    const std::string& filename) const {

    std::ofstream file(filename);

    if (!file) {
        throw std::runtime_error(
            "Could not open CSV output file: " + filename
        );
    }

    // Export every Repository field, including description and URL.
    write_csv_row(file, {
        "owner",
        "name",
        "description",
        "stars",
        "forks",
        "language",
        "size",
        "updated_at",
        "url"
    });

    for (const Repository& r : repos) {
        write_csv_row(file, {
            r.owner,
            r.name,
            r.description,
            std::to_string(r.stars),
            std::to_string(r.forks),
            r.language,
            std::to_string(r.size),
            r.updated_at,
            r.url
        });
    }

    if (!file) {
        throw std::runtime_error(
            "Failed while writing CSV output file: " + filename
        );
    }
}
