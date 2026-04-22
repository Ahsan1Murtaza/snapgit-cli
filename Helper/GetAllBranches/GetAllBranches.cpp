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

    ifstream headFile(".mygit/HEAD");
    string headContent;
    getline(headFile, headContent);
    headFile.close();

    if (headContent.rfind("ref:", 0) == 0) {
        currentBranch = headContent.substr(16); 
    }

    cout << "Branches:\n";
    for (const auto &entry : filesystem::directory_iterator(".mygit/refs/heads")) {
        string branchName = entry.path().filename().string();
        if (branchName == currentBranch)
            cout << "* " << branchName << "\n";
        else
            cout << "  " << branchName << "\n";
    }
}
