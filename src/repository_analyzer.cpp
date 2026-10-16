#include "repository_analyzer.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

bool matches_filters(
    const Repository& r,
    const AnalysisOptions& options) {

    if (r.stars < options.min_stars) {
        return false;
    }

    if (!options.language.empty() &&
        r.language != options.language) {
        return false;
    }

    return true;
}

bool compare_owner_name(
    const Repository& a,
    const Repository& b) {

    if (a.owner != b.owner) {
        return a.owner < b.owner;
    }

    return a.name < b.name;
}

bool compare_by_stars(
    const Repository& a,
    const Repository& b) {

    if (a.stars != b.stars) {
        return a.stars > b.stars;
    }

    return compare_owner_name(a, b);
}

bool compare_by_forks(
    const Repository& a,
    const Repository& b) {

    if (a.forks != b.forks) {
        return a.forks > b.forks;
    }

    return compare_owner_name(a, b);
}

bool compare_by_updated(
    const Repository& a,
    const Repository& b) {

    if (a.updated_at != b.updated_at) {
        return a.updated_at > b.updated_at;
    }

    return compare_owner_name(a, b);
}

} // namespace

std::vector<Repository> RepositoryAnalyzer::analyze(
    const std::vector<Repository>& input,
    const AnalysisOptions& options) const {

    if (options.sort_key != "stars" &&
        options.sort_key != "forks" &&
        options.sort_key != "updated") {
        throw std::invalid_argument(
            "Invalid sort key. Use stars, forks, or updated."
        );
    }

    // Step 1: Filter.
    std::vector<Repository> rows;
    rows.reserve(input.size());

    for (const Repository& r : input) {
        if (matches_filters(r, options)) {
            rows.push_back(r);
        }
    }

    // Step 2: Sort in descending order using named comparators.
    if (options.sort_key == "stars") {
        std::sort(
            rows.begin(), rows.end(), compare_by_stars
        );
    }
    else if (options.sort_key == "forks") {
        std::sort(
            rows.begin(), rows.end(), compare_by_forks
        );
    }
    else if (options.sort_key == "updated") {
        std::sort(
            rows.begin(), rows.end(), compare_by_updated
        );
    }

    // Step 3: Select Top N.
    if (rows.size() > options.top_n) {
        rows.resize(options.top_n);
    }

    return rows;
}
