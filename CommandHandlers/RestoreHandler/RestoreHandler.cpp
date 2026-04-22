// SPDX-License-Identifier: MIT
// Implementation for RestoreHandler.

#include "RestoreHandler.h"
#include "../../Helper/ReadIndex/ReadIndex.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;



/**
 * @brief Handles the  restore command workflow.
 * @param filePath Path to the target file relative to the repository root.
 */
void RestoreHandler::handleRestore(const string& filePath) {
    auto index = readIndex();

    if (index.find(filePath) == index.end()) {
        cerr << "Error: " << filePath << " not found in index\n";
        return;
    }

    string hash = index[filePath];
    string objFolder = ".mygit/objects/" + hash.substr(0, 2);
    string objFile = objFolder + "/" + hash.substr(2);

    if (!fs::exists(objFile)) {
        cerr << "Error: Object " << hash << " not found\n";
        return;
    }

    ifstream in(objFile, ios::binary);
    if (!in) {
        cerr << "Error: Could not open object file " << objFile << "\n";
            return;
    }

    string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    size_t pos = content.find('\0');
    if (pos != string::npos) content = content.substr(pos + 1);

    // Ensure parent directories exist (skip if root-level file)
        fs::path filePathObj(filePath);
        fs::path parentDir = filePathObj.parent_path();
        if (!parentDir.empty()) {
            fs::create_directories(parentDir);
        }

        // Write content to working directory
        ofstream out(filePathObj, ios::binary);
        if (!out) {
            cerr << "Error: Could not write to file " << filePath << "\n";
            return;
        }
        out.write(content.data(), content.size());
        out.close();

        cout << "Restored " << filePath << " from index\n";
}
