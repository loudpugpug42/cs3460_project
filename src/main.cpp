#include <iostream>

int main() {
std::cout << "Hello, World!" << '\n';
return 0;
}

// 1
struct Repository {
std::string owner;
std::string name;
std::string description;
std::uint64_t stars{};
std::uint64_t forks{};
std::string language;
std::uint64_t size{};
std::string updated_at;
std::string url;
};
struct SearchOptions {
std::string query; // e.g. "stars:>1000"
std::size_t count{100};
};

// 2
httplib::Client client("https://api.github.com");
std::string path =
"/search/repositories?q=" +
httplib::encode_uri_component(options.query) +
"&per_page=" + std::to_string(options.count);
httplib::Headers headers{
{"Accept", "application/vnd.github+json"},
{"X-GitHub-Api-Version", "2026-03-10"},
{"User-Agent", "cs3460-github-crawler"}
};
if (const char* token = std::getenv("GITHUB_TOKEN")) {
headers.emplace("Authorization", std::string("Bearer ") + token);
}
auto result = client.Get(path, headers);
if (!result || result->status != 200) {
// print a clear request/API error
return {};
}
std::string body = result->body;

// 3
#include <nlohmann/json.hpp>
using json = nlohmann::json;
json root = json::parse(body);
std::vector<Repository> repos;
for (const auto& item : root.at("items")) {
Repository r;
r.owner = item.at("owner").at("login").get<std::string>();
r.name = item.at("name").get<std::string>();
r.description = item.at("description").is_null()
? "" : item.at("description").get<std::string>();
r.stars = item.at("stargazers_count").get<std::uint64_t>();
r.forks = item.at("forks_count").get<std::uint64_t>();
r.language = item.at("language").is_null()
? "" : item.at("language").get<std::string>();
r.size = item.at("size").get<std::uint64_t>();
r.updated_at = item.at("updated_at").get<std::string>();
r.url = item.at("html_url").get<std::string>();
repos.push_back(std::move(r));
}

// 4
json out = json::array();
for (const auto& r : repos) {
out.push_back({
{"owner", r.owner}, {"name", r.name},
{"description", r.description}, {"stars", r.stars},
{"forks", r.forks}, {"language", r.language},
{"size", r.size}, {"updated_at", r.updated_at},
{"url", r.url}
});
}