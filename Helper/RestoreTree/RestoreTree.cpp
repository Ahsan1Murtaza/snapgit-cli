#include "RestoreTree.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>
using namespace std;
namespace fs = std::filesystem;

void restoreTree(const string& treeHash, const string& basePath) {
    string folder = ".mygit/objects/" + treeHash.substr(0, 2);
    string file = folder + "/" + treeHash.substr(2);
    
    ifstream in(file);
    if (!in.is_open()) {
        cerr << "Error: Tree object not found: " << treeHash << endl;
        return;
    }
    
    string line;
    while (getline(in, line)) {
        istringstream iss(line);
        string mode, type, hash, name;
        iss >> mode >> type >> hash >> name;
        
        if (name.empty()) continue;
        
        string fullPath = basePath.empty() ? name : basePath + "/" + name;
        
        if (type == "blob") {
            string blobFolder = ".mygit/objects/" + hash.substr(0, 2);
            string blobFile = blobFolder + "/" + hash.substr(2);
            
            ifstream blobIn(blobFile, ios::binary);
            if (!blobIn.is_open()) {
                cerr << "Error: Blob not found: " << hash << endl;
                continue;
            }
            
            fs::path filePath(fullPath);
            if (filePath.has_parent_path()) {
                fs::create_directories(filePath.parent_path());
            }
            
            ofstream out(fullPath, ios::binary);
            out << blobIn.rdbuf();
            blobIn.close();
            out.close();
            
            cout << "Restored: " << fullPath << endl;
        }
        else if (type == "tree") {
            fs::create_directories(fullPath);
            restoreTree(hash, fullPath);
        }
    }
    
    in.close();
}