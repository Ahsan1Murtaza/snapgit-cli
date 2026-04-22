// SPDX-License-Identifier: MIT
// Implementation for Ignore.

#include "Ignore.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <fnmatch.h>

using namespace std;
namespace fs = std::filesystem;

/**
 * @brief Reads ignore patterns from repository storage.
 * @return Requested string value.
 */
vector<string> readIgnorePatterns() {
    vector<string> patterns;
    ifstream ignoreFile(".mygitignore");
    string line;

    while (getline(ignoreFile, line)) {
        line.erase(line.begin(), find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !isspace(ch);
        }));
        line.erase(find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !isspace(ch);
        }).base(), line.end());

        if (line.empty() || line[0] == '#') {
            continue;
        }

        for (char& c : line) {
            if (c == '\\') c = '/';
        }
        patterns.push_back(line);
    }

    return patterns;
}

/**
 * @brief Checks whether ignored path.
 * @param relativePath Path relative to the repository root.
 * @param patterns Ignore patterns loaded from .mygitignore.
 * @return True when the check succeeds; otherwise false.
 */
bool isIgnoredPath(const string& relativePath, const vector<string>& patterns) {
    fs::path relPath(relativePath);
    string fileName = relPath.filename().generic_string();

    for (const string& pattern : patterns) {
        if (pattern.find('*') != string::npos || pattern.find('?') != string::npos) {
            if (fnmatch(pattern.c_str(), relativePath.c_str(), 0) == 0) {
                return true;
            }
            continue;
        }

        if (!pattern.empty() && pattern.back() == '/') {
            string dirPattern = pattern.substr(0, pattern.size() - 1);
            if (relativePath == dirPattern || relativePath.rfind(dirPattern + "/", 0) == 0) {
                return true;
            }
            continue;
        }

        if (relativePath == pattern || fileName == pattern) {
            return true;
        }

        if (relativePath.rfind(pattern + "/", 0) == 0) {
            return true;
        }
    }

    return false;
}
