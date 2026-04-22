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
 * @brief readIgnorePatterns operation.
 * @return Return value description.
 */
vector<string> readIgnorePatterns() {
    vector<string> patterns;
    ifstream ignoreFile(".mygitignore");
    string line;

    while (getline(ignoreFile, line)) {
        /**
         * @brief line.erase operation.
         * @param ch Parameter description.
         */
        line.erase(line.begin(), find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !isspace(ch);
        }));
        /**
         * @brief line.erase operation.
         * @param ch Parameter description.
         */
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
 * @brief isIgnoredPath operation.
 * @param relativePath Parameter description.
 * @param patterns Parameter description.
 * @return Return value description.
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
