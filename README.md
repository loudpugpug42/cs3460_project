# CS3460 Project - GitHub Crawler

## Project Name and Selected Project
### GitHub Crawler
We selected Project Option 5, the GitHub Crawler project. It is a modern C++ command-line program that queries the GitHub REST API for public repositories, converts API responses into C++ `Repository` objects, prints repository metadata during collection, and saves a stable local dataset as `repositories.json`.

The crawler supports configurable GitHub search criteria from the command line. The current milestone focuses on online repository collection and local storage of repository metadata with further milestones to add onto it.

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

The libraries are declared in `vcpkg.json` and are installed automatically by vcpkg during the CMake configuration step. vcpkg manifest mode allows project dependencies to be declared in the repository and restored durirng configuration.

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

The default collection requests 100 public repositories using the default search criterion:

```
stars:>0
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

### Search Criteria

A GitHub search criterion can be supplied as a command-line argument. For example:
```
./build/github_crawler "stars:>1000"
```
would request repositories matching the GitHub search criterion of `stars:>1000`.
