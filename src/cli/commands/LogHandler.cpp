// SPDX-License-Identifier: MIT
// Implementation for LogHandler.

#include "LogHandler.h"

#include "../utils/GetAllCommits.h"
#include "../utils/RepoCheck.h"
#include "../utils/GetHeadRef.h"     
#include "../utils/GetCurrentCommitHash.h" 

#include <iostream>
#include <algorithm>

using namespace std;

/**
 * @brief Handles the  log command workflow.
 */
void LogHandler::handleLog() {
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Run 'init' first.\n";
        return;
    }

    string headCommit = getCurrentCommitHash();
    if (headCommit.empty()) {
        cout << "No commits yet on this branch.\n";
        return;
    }

    auto commits = getAllCommits(headCommit);
    // Sort by timestamp descending (most recent first)
    sort(commits.begin(), commits.end(), [](const CommitInfo& a, const CommitInfo& b) {
        return a.timestamp > b.timestamp;
    });
    for (auto &c : commits) {
        cout << "commit " << c.hash << "\n";
        cout << "Author: " << c.author;
        if (!c.email.empty()) cout << " <" << c.email << ">";
        cout << "\n";
        if (c.timestamp > 0) {
            time_t t = static_cast<time_t>(c.timestamp);
            char buf[64];
            strftime(buf, sizeof(buf), "%a %b %d %H:%M:%S %Y", localtime(&t));
            cout << "Date:   " << buf << "\n";
        }
        cout << "\n    " << c.message << "\n\n";
    }
}
