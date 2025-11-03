#include "StatusHandler.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include "../../Helper/RepoCheck/RepoCheck.h"

using namespace std;
namespace fs = std::filesystem;

StatusHandler::StatusHandler() {}

void StatusHandler::handleStatus() {
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Please run 'init' command first.\n";
        return;
    }

    string indexPath = ".mygit/index";
    if (!fs::exists(indexPath)) {
        cout << "No files staged for commit.\n";
        return;
    }

    cout << "On branch main\n\n";
    cout << "Changes to be committed:\n";

    ifstream indexFile(indexPath);
    string line;
    while (getline(indexFile, line)) {
        cout << "    " << line << "\n";
    }

    indexFile.close();
}
