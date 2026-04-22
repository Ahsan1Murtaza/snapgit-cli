// SPDX-License-Identifier: MIT
// Implementation for LogHandler.

#include "LogHandler.h"
#include "../../Helper/GetAllCommits/GetAllCommits.h"
#include "../../Helper/RepoCheck/RepoCheck.h"
#include "../../Helper/GetHeadRef/GetHeadRef.h"     
#include "../../Helper/GetCurrentCommitHash/GetCurrentCommitHash.h" 
#include <iostream>

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
    for (auto &c : commits) {
        cout << "commit " << c.hash << "\n";
        cout << "Author: " << c.author;
        if (!c.email.empty()) cout << " <" << c.email << ">";
        cout << "\n";
        cout << "\n    " << c.message << "\n\n";
    }
}
