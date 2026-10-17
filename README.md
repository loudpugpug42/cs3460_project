# CS3460 Project - GitHub Crawler

## Project Name and Selected Project
### GitHub Crawler
We selected Project Option 5, the GitHub Crawler project. It is a modern C++ command-line program that collects public repository metadata through GitHub REST API, converts API responses into structured C++ objects, and saves the data locally. It focuses on repository metadata rather than analyzing repository source code or Git history.

The crawler supports two main workflows:
- Online Repository Collection: Query GitHub for public repositories, parse their metadata into `Repository` objects, display the collected information, and save the dataset to `repositories.json`.
- Local Repository Analysis: Load the saved dataset without making another GitHub request, filter and sort repositories, select the top N results, display a report, and export the selected results to JSON or CSV.

## Team Members

### Saxton Calvert - Team Leader
A#: A02357348

Email: A02357348@usu.edu

### Jake Peterson
A#: A02421770

Email: a02421770@usu.edu

### Connor Gould
A#: A02341215

Email: a02341215@usu.edu

## Setup

### Requirements
The following software must be installed before building the project:
- C++20 or later
- CLang / CLang++ (LLVM Clang 19.0.0 or later; development uses CLang++ 23.1.0)
- CMake 3.20 or later
- Ninja
- Git
- vcpkg
- Command-line build and execution

The following C++ libraries are used:
- cpp-httplib with OpenSSL support for HTTPS requests to the GitHub REST API
- nlohmann/json for parsing and serializing JSON

The libraries are declared in `vcpkg.json` and are installed automatically by vcpkg during the CMake configuration step run by the build script.

A separate system-wide OpenSSL installation is not required when the dependencies are installed through vcpkg because the required OpenSSL dependency is resolved through the cpp-httplib configuration.

### Clone the repository and enter the project directory:

```
git clone https://github.com/loudpugpug42/cs3460_project cs3460_project
cd cs3460_project
```

### Configure vcpkg
Install vcpkg on your system if it is not already installed. Set the `VCPKG_ROOT` environment variable to the location of your vcpkg installation. The build script uses:
```
$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```
as the CMake toolchain file.

### Make the build script executable:

```
chmod +x build.sh
```

### Build the project:

```
./build.sh
```

## Run

After building, run the program with:

```
./build/github_crawler
```

### Collection
To collect the default dataset of up to 100 public repositories, run:

```
./build/github_crawler collect
```

The default collection requests 100 public repositories using the default search criterion:

```
stars:>0
```

Custom GitHub repository search criteria can also be supplied. For instance, to restrict the search to repositories using C++ that have over 1000 stars, run:
```
./build/github_crawler collect "stars:>1000 language:c++"
```

The program prints a summary and information for every repository collected including:
- Owner/name
- Description
- Stars
- Forks
- Language
- Size
- Last update time
- URL
The collected repository data is saved as `repositories.json` which is a JSON array containing the project's own `Repository` fields rather than the complete raw GitHub API response.

### Analysis
The default execution mode performs local analysis using `repositories.json`. It does not collect another dataset from GitHub.

```
./build/github_crawler
```

By default, the program loads the saved dataset, selects up to 10 repositories, sorts them by descending star count, prints a report, and writes the selected results to `selected.json`. The number loaded, analysis criteria, and number selected are reported in the terminal. The following command line options are available.

| Option          | Description                              | Default                           |
|-----------------|------------------------------------------|-----------------------------------|
| --input FILE    | Path to the local repository data        | `repositories.json`               |
| --min-stars N   | Minimum star count required              | `0`                               |
| --language LANG | Exact programming-language filter        | Any language                      |
| --sort KEY      | Sort by `stars`, `forks`, or `updated`   | `stars`                           |
| --top N         | Maximum number of repositories to select | `10`                              |
| --format FORMAT | Export format: `json` or `csv`           | `json`                            |
| --output FILE   | Output file path                         | `selected.json` or `selected.csv` |
| --help          | Display command line usage information   | ---                               |

Filtering occurs after sorting, and top-N selection is applied after sorting. If fewer repositories match the requested criteria than the specified top-N count, all matching repositories are returned. To select the five repositories in C++ with at least 1000 stars sorted by fork count and export it to a csv, run:
```
./build/github_crawler --min-stars 1000 --language C++ --sort forks --format csv
```

## Output Files
The following are the default file names for output, stored in the same directory as the project. The output filenames can be changed using `--output FILE`

### repositories.json
Produced by the collect workflow, this file contains the local repository dataset. Each array element represents a `Repository` object with the following fields:
- owner
- name
- description
- stars
- forks
- language
- size
- updated_at
- url

### selected.json
The default output for analysis is a JSON array containing only the repositories selected by the current analysis run.

### selected.csv
When `--format csv` is specified, the selected repositories are exported as CSV. The JSON and CSV exporters receive the same selected repository collection and include the repository metadata fields listed above.
