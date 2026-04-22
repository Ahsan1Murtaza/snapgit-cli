// SPDX-License-Identifier: MIT
// Declarations for GetAllCommits.

#ifndef GET_ALL_COMMITS_H
#define GET_ALL_COMMITS_H

#include <string>
#include <vector>
using namespace std;

struct CommitInfo {
    string hash;
    string tree;
    vector<string> parents; // Supports multi parents
    string author;
    string email;
    long long timestamp = 0;
    string message;
};

/**
 * @brief Returns all commits.
 * @param headCommitHash Commit hash currently referenced by HEAD.
 * @return Container populated with parsed repository data.
 */
vector<CommitInfo> getAllCommits(const string &headCommitHash);

#endif
