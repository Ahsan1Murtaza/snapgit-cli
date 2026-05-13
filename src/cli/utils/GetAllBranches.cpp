// SPDX-License-Identifier: MIT
// Implementation for GetAllBranches.

#include "GetAllBranches.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>


using namespace std;

/**
 * @brief Returns all branches.
 */
void getAllBranches() {
    string currentBranch = "";

    ifstream headFile(".snapgit/HEAD");
    string headContent;
    getline(headFile, headContent);
    headFile.close();

    if (headContent.rfind("ref:", 0) == 0) {
        currentBranch = headContent.substr(16); 
    }

    cout << "Branches:\n";
    for (const auto &entry : filesystem::directory_iterator(".snapgit/refs/heads")) {
        string branchName = entry.path().filename().string();
        if (branchName == currentBranch)
            cout << "* " << branchName << "\n";
        else
            cout << "  " << branchName << "\n";
    }
}
