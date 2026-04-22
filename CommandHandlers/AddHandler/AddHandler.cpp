// SPDX-License-Identifier: MIT
// Implementation for AddHandler.

#include "AddHandler.h"
#include "../../Objects/Blob/Blob.h"
#include "../../Helper/RepoCheck/RepoCheck.h"
#include "../../Helper/Ignore.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <sstream>
#include <vector>

using namespace std;
namespace fs = filesystem;

AddHandler::AddHandler() {}

/*
This function handles Add Command
    Creates Blob object
    calls save
    add file to staging area (Index File)
*/
/**
 * @brief Handles the  add command workflow.
 * @param filePath Path to the target file relative to the repository root.
 */
void AddHandler::handleAdd(const string& filePath) {
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized.\n";
        return;
    }

    // Read current index
    unordered_map<string, string> indexMap;
    ifstream indexIn(".mygit/index");
    string line;
    while (getline(indexIn, line)) {
        istringstream iss(line);
        string path, hash;
        iss >> path >> hash;
        indexMap[path] = hash;
    }
    indexIn.close();

    vector<string> ignorePatterns = readIgnorePatterns();
    const fs::path repoRoot = fs::current_path();
    fs::path inputPath(filePath);

    if (!fs::exists(inputPath)) {
        cerr << "Error: Path '" << filePath << "' does not exist.\n";
        return;
    }

    vector<fs::path> filesToStage;

    if (filePath == ".") {
        fs::recursive_directory_iterator it(repoRoot), end;
        while (it != end) {
            const fs::directory_entry& entry = *it;
            fs::path relPath = fs::relative(entry.path(), repoRoot);
            string rel = relPath.generic_string();

            if (entry.is_directory()) {
                if (rel == ".mygit" || rel.rfind(".mygit/", 0) == 0 || isIgnoredPath(rel + "/", ignorePatterns)) {
                    it.disable_recursion_pending();
                }
                ++it;
                continue;
            }

            if (!entry.is_regular_file()) {
                ++it;
                continue;
            }

            if (rel == ".mygitignore") {
                filesToStage.push_back(entry.path());
                ++it;
                continue;
            }

            if (!isIgnoredPath(rel, ignorePatterns)) {
                filesToStage.push_back(entry.path());
            }
            ++it;
        }
    } else if (fs::is_directory(inputPath)) {
        fs::path canonicalInput = fs::weakly_canonical(inputPath);
        fs::recursive_directory_iterator it(canonicalInput), end;
        while (it != end) {
            const fs::directory_entry& entry = *it;
            fs::path relPath = fs::relative(entry.path(), repoRoot);
            string rel = relPath.generic_string();

            if (entry.is_directory()) {
                if (rel == ".mygit" || rel.rfind(".mygit/", 0) == 0 || isIgnoredPath(rel + "/", ignorePatterns)) {
                    it.disable_recursion_pending();
                }
                ++it;
                continue;
            }

            if (entry.is_regular_file() && rel.rfind(".mygit/", 0) != 0 && !isIgnoredPath(rel, ignorePatterns)) {
                filesToStage.push_back(entry.path());
            }
            ++it;
        }
    } else {
        filesToStage.push_back(fs::weakly_canonical(inputPath));
    }

    for (const auto& absFilePath : filesToStage) {
        fs::path relPath = fs::relative(absFilePath, repoRoot);
        string rel = relPath.generic_string();
        for (char& c : rel) if (c == '\\') c = '/';

        if (rel.rfind(".mygit/", 0) == 0 || isIgnoredPath(rel, ignorePatterns)) {
            continue;
        }

        Blob blob(absFilePath.string());
        const string newHash = blob.getHash();

        auto it = indexMap.find(rel);
        if (it != indexMap.end() && it->second == newHash) {
            continue;
        }

        blob.save();
        indexMap[rel] = newHash;
        cout << "Added " << rel << " in staging area\n";
    }

    // Write back index
    ofstream indexOut(".mygit/index", ios::trunc);
    for (auto& [path, hash] : indexMap) {
        indexOut << path << " " << hash << "\n";
    }
    indexOut.close();
}
