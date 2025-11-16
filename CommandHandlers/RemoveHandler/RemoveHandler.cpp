#include "RemoveHandler.h"
#include "../../Helper/ReadIndex/ReadIndex.h"

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void RemoveHandler::handleRemove(const string& filePath, bool cached) {
    auto index = readIndex();

    if (index.find(filePath) == index.end()) {
        cerr << "Error: " << filePath << " not found in index\n";
        return;
    }

    // Remove from index
    index.erase(filePath);

    // Write updated index back
    ofstream out(".mygit/index", ios::trunc);
    if (!out) {
        cerr << "Error: Could not open index file for writing\n";
        return;
    }

    for (auto& [path, hash] : index) {
        out << path << " " << hash << "\n";
    }
    out.close();

    if (!cached) {
        // Remove from working directory
        if (fs::exists(filePath)) {
            fs::remove(filePath);
            cout << "Removed " << filePath << " from working directory and staging area\n";
        } else {
            cout << "Removed " << filePath << " from staging area (already deleted in working directory)\n";
        }
    } else {
        cout << "Removed " << filePath << " from staging area (index only)\n";
    }
}
