#include "RestoreHandler.h"
#include "../../Helper/ReadIndex/ReadIndex.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <sstream>

using namespace std;
namespace fs = std::filesystem;



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
    string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    size_t pos = content.find('\0');
    if (pos != string::npos) content = content.substr(pos + 1);

    fs::create_directories(fs::path(filePath).parent_path());
    ofstream out(filePath, ios::binary);
    out.write(content.data(), content.size());

    cout << "Restored " << filePath << " from index\n";
}