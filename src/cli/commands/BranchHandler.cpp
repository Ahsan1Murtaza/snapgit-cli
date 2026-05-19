// SPDX-License-Identifier: MIT
// Implementation for BranchHandler.

#include "BranchHandler.h"

#include "../utils/RepoCheck.h"
#include "../utils/GetAllBranches.h"
#include "../utils/GetHeadRef.h"
#include "../utils/GetCurrentCommitHash.h"

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

static string currentBranchName() {
    string headRef = getHeadRef();
    const string prefix = "refs/heads/";
    if (headRef.rfind(prefix, 0) == 0) {
        return headRef.substr(prefix.size());
    }
    return "";
}

/**
 * @brief Handles the branch command (list, create, or delete).
 * @param argc Argument count from main (includes program name).
 * @param argv Argument vector from main.
 */
void BranchHandler::handleBranch(int argc, char* argv[]) {
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Run 'snapgit init' first.\n";
        return;
    }

    if (argc == 2) {
        getAllBranches();
        return;
    }

    if (argc == 4 && (string(argv[2]) == "-d" || string(argv[2]) == "--delete")) {
        string branchName = argv[3];
        string branchPath = ".snapgit/refs/heads/" + branchName;

        if (!fs::exists(branchPath)) {
            cerr << "Error: Branch '" << branchName << "' does not exist.\n";
            return;
        }

        string onBranch = currentBranchName();
        if (!onBranch.empty() && onBranch == branchName) {
            cerr << "Error: Cannot delete the branch you are currently on ('" << branchName << "').\n";
            cerr << "Checkout another branch first.\n";
            return;
        }

        error_code ec;
        if (!fs::remove(branchPath, ec)) {
            cerr << "Error: Failed to delete branch '" << branchName << "': " << ec.message() << "\n";
            return;
        }

        cout << "Deleted branch '" << branchName << "'.\n";
        return;
    }

    if (argc == 3) {
        string branchName = argv[2];
        string branchPath = ".snapgit/refs/heads/" + branchName;

        if (fs::exists(branchPath)) {
            cerr << "Error: Branch '" << branchName << "' already exists.\n";
            return;
        }

        string currentCommit = getCurrentCommitHash();

        ofstream newBranch(branchPath);
        newBranch << currentCommit;
        newBranch.close();

        cout << "Branch '" << branchName << "' created at commit " << currentCommit << endl;
        return;
    }

    cout << "Usage:\n";
    cout << "  snapgit branch             # List branches\n";
    cout << "  snapgit branch <name>      # Create a new branch\n";
    cout << "  snapgit branch -d <name>   # Delete a branch\n";
}
