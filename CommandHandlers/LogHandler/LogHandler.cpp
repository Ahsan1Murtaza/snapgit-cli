#include "LogHandler.h"
#include "../../Helper/GetAllCommits/GetAllCommits.h"
#include "../../Helper/RepoCheck/RepoCheck.h"
#include "../../Helper/GetHeadRef/GetHeadRef.h"     
#include "../../Helper/GetCurrentCommitHash/GetCurrentCommitHash.h" 
#include <iostream>

using namespace std;

// ANSI colors
static const char* COLOR_RESET = "\033[0m";
static const char* COLOR_COMMIT = "\033[33m"; // yellow
static const char* COLOR_LABEL = "\033[36m";  // cyan

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
        cout << COLOR_COMMIT << "commit " << c.hash << COLOR_RESET << "\n";
        cout << COLOR_LABEL << "Author: " << COLOR_RESET << c.author;
        if (!c.email.empty()) cout << " <" << c.email << ">";
        cout << "\n";
        cout << "\n    " << c.message << "\n\n";
    }
}
