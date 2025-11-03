#include "LogHandler.h"
#include "../../Helper/GetCurrentCommitHash/GetCurrentCommitHash.h"
#include "../../Helper/RepoCheck/RepoCheck.h"
#include <iostream>
#include <fstream>

using namespace std;

void LogHandler::showLog() {
    if (!RepoCheck::isRepoInitialized()) {
        cout << "fatal: not a snapgit repository\n";
        return;
    }

    string currentCommit = getCurrentCommitHash();
    if (currentCommit.empty()) {
        cout << "No commits yet.\n";
        return;
    }

    cout << "=== SnapGit Log ===\n";

    while (!currentCommit.empty()) {
        printCommitDetails(currentCommit);

        string commitPath = ".mygit/commits/" + currentCommit + ".commit";
        ifstream file(commitPath);
        if (!file) break;

        string line, parentHash = "";
        while (getline(file, line)) {
            if (line.rfind("parent ", 0) == 0)
                parentHash = line.substr(7);
        }

        file.close();
        currentCommit = parentHash;
    }
}

void LogHandler::printCommitDetails(const string& commitHash) {
    string path = ".mygit/commits/" + commitHash + ".commit";
    ifstream file(path);
    if (!file) return;

    cout << "\ncommit " << commitHash << "\n";
    string line;
    while (getline(file, line)) {
        if (line.rfind("author ", 0) == 0)
            cout << line << "\n";
        else if (line.rfind("message ", 0) == 0)
            cout << "    " << line.substr(8) << "\n";
    }
    file.close();
}