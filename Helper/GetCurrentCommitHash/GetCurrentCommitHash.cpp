// SPDX-License-Identifier: MIT
// Implementation for GetCurrentCommitHash.

#include "GetCurrentCommitHash.h"
#include "../GetHeadRef/GetHeadRef.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;

/**
 * @brief trim operation.
 * @param s Parameter description.
 * @return Return value description.
 */
static string trim(const string& s) {
    /**
     * @brief find_if_not operation.
     * @param ch Parameter description.
     * @return Return value description.
     */
    auto start = find_if_not(s.begin(), s.end(), [](unsigned char ch) { return isspace(ch); });
    if (start == s.end()) return "";
    /**
     * @brief find_if_not operation.
     * @param ch Parameter description.
     * @return Return value description.
     */
    auto end = find_if_not(s.rbegin(), s.rend(), [](unsigned char ch) { return isspace(ch); }).base();
    return string(start, end);
}

/*
This function returns currentCommitHash
if not present then return empty string
*/
/**
 * @brief getCurrentCommitHash operation.
 * @return Return value description.
 */
string getCurrentCommitHash() {
    string currentBranchFile = getHeadRef();

    // Normal branch mode: HEAD points to refs/heads/<branch>
    if (!currentBranchFile.empty()) {
        string path = ".mygit/" + currentBranchFile;
        ifstream in(path);
        if (!in.is_open()) {
            return "";
        }
        string hash;
        getline(in, hash);
        return trim(hash);
    }

    // Detached HEAD mode: HEAD stores commit hash directly
    ifstream headFile(".mygit/HEAD");
    if (!headFile.is_open()) {
        return "";
    }

    string headLine;
    getline(headFile, headLine);
    headLine = trim(headLine);

    const string prefix = "ref: ";
    if (headLine.rfind(prefix, 0) == 0) {
        // HEAD references a branch but getHeadRef() returned empty somehow.
        // Treat as unreadable state instead of returning a ref path here.
        return "";
    }

    return headLine;
}
