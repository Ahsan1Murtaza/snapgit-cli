// SPDX-License-Identifier: MIT
// Implementation for BranchHandler.

#include "BranchHandler.h"

#include "../utils/RepoCheck.h"
#include "../utils/GetAllBranches.h"

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

/**
 * @brief Handles the  branch command workflow.
 * @param branchName Branch name to create or inspect.
 */
void BranchHandler::handleBranch(const string &branchName) {
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Run 'snapgit init' first.\n";
        return;
    }

    
    if (branchName.empty()) {
        getAllBranches();  // use helper
        return;
    }

    
    string branchPath = ".snapgit/refs/heads/" + branchName;

    if (filesystem::exists(branchPath)) {
        cerr << "Error: Branch '" << branchName << "' already exists.\n";
        return;
    }

   
    ifstream headFile(".snapgit/HEAD");
    string headContent;
    getline(headFile, headContent);
    headFile.close();

    string currentCommit;
    if (headContent.rfind("ref:", 0) == 0) {
       
        string branchRef = ".snapgit/" + headContent.substr(5);
        ifstream branchRefFile(branchRef);
        getline(branchRefFile, currentCommit);
        branchRefFile.close();
    } else {
        
        currentCommit = headContent;
    }

    
    ofstream newBranch(branchPath);
    newBranch << currentCommit;
    newBranch.close();

    cout << "Branch '" << branchName << "' created at commit " << currentCommit << endl;
}
