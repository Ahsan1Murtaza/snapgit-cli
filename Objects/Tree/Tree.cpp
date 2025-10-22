#include "Tree.h"
#include "../../Helper/SortIndex/SortIndex.h"
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

Tree::Tree() : Object("tree") {
    // Constructor implementation
}

Tree::~Tree() {
    for (auto& pair : subTrees) {
        delete pair.second; // clean up memory for each subtree
    }
}

void Tree::addBlob(const string& filePath, const string& blobHash) {
    blobs[filePath] = blobHash;
}

Tree* Tree::getOrCreateSubTree(const string& folderName) {
    if (subTrees.find(folderName) == subTrees.end()) {
        subTrees[folderName] = new Tree();
    }
    return subTrees[folderName];
}

void Tree::buildFromIndex(const std::string& indexPath) {

    sortIndexFile(indexPath); // Ensure index file is sorted

    ifstream indexFile(indexPath);
    if (!indexFile) {
        cout << "Error: Unable to open index file for building tree.\n";
        return;
    }
    string line;
    while (getline(indexFile, line)) {
        size_t spacePos = line.find(' ');
        if (spacePos == string::npos) continue;

        string filePath = line.substr(0, spacePos);
        string blobHash = line.substr(spacePos + 1);

        size_t pos;
        
        Tree* currentTree = this;
        while ((pos = filePath.find_first_of("/\\")) != string::npos) {
            string folderName = filePath.substr(0, pos);
            currentTree = currentTree->getOrCreateSubTree(folderName);
            filePath = filePath.substr(pos + 1);
        }
        currentTree->addBlob(filePath, blobHash);
    }
}
void Tree::showTreeElements(int depth) const {
    std::string indent(depth * 2, ' '); // Indentation for tree levels

    // Print blobs (files)
    for (const auto& blob : blobs) {
        std::cout << indent << "File: " << blob.first 
                  << " -> " << blob.second << "\n";
    }

    // Print subtrees (folders)
    for (const auto& subtree : subTrees) {
        std::cout << indent << "Folder: " << subtree.first << "\n";
        subtree.second->showTreeElements(depth + 1); // recursive call
    }
}


void Tree::save() {
    return; // leave for now
}