// SPDX-License-Identifier: MIT
// Implementation for Tree.

#include "Tree.h"
#include "../../Helper/SortIndex/SortIndex.h"
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <filesystem>

#include "../../Helper/Hash/Hash.h"

using namespace std;
namespace fs = std::filesystem;

/**
 * @brief Tree operation.
 */
Tree::Tree() : Object("tree") {
    // Constructor implementation
}

/**
 * @brief ~Tree operation.
 */
Tree::~Tree() {
    for (auto& pair : subTrees) {
        delete pair.second; // clean up memory for each subtree
    }
}

/*
Add file in a tree
Stored in map (filePath, hash)
*/
/**
 * @brief addBlob operation.
 * @param filePath Parameter description.
 * @param blobHash Parameter description.
 */
void Tree::addBlob(const string& filePath, const string& blobHash) {
    blobs[filePath] = blobHash;
}

/*
This function creates a Tree or returns it if already created
*/
/**
 * @brief getOrCreateSubTree operation.
 * @param folderName Parameter description.
 * @return Return value description.
 */
Tree* Tree::getOrCreateSubTree(const string& folderName) {
    if (subTrees.find(folderName) == subTrees.end()) {
        subTrees[folderName] = new Tree();
    }
    return subTrees[folderName];
}


/*
Scans the Index file and create a in-memory tree
*/
/**
 * @brief buildFromIndex operation.
 * @param indexPath Parameter description.
 */
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

/*
Just For Development Purpose
*/
// void Tree::showTreeElements(int depth) const {
//     std::string indent(depth * 2, ' '); // Indentation for tree levels

//     // Print blobs (files)
//     for (const auto& blob : blobs) {
//         std::cout << indent << "File: " << blob.first 
//                   << " -> " << blob.second << "\n";
//     }

//     // Print subtrees (folders)
//     for (const auto& subtree : subTrees) {
//         std::cout << indent << "Folder: " << subtree.first << "\n";
//         subtree.second->showTreeElements(depth + 1); // recursive call
//     }
// }


/*
Save the in-memory tree into Objects folder recursively
*/
/**
 * @brief save operation.
 */
void Tree::save() {
    
    ostringstream contentStream;

    // Add Blobs First
    for (const auto& blob: blobs) {
        contentStream << "100644 blob " << blob.second << " " << blob.first << "\n"; 
    }

    // Add subTress (recursive Calls)
    for (const auto& subTreePair: subTrees) {
        Tree* subTree = subTreePair.second;
        subTree->save(); // Save subTree first

        contentStream << "040000 tree " << subTree->getHash() << " " << subTreePair.first << "\n";
    }

    treeContent = contentStream.str();

    hash = sha1(treeContent);

    // save to .mygit/objects/
    string folder = ".mygit/objects/" + hash.substr(0, 2);
    string file = folder + "/" + hash.substr(2);
    
    fs::create_directories(folder);

    ofstream out(file);
    out << treeContent;
    out.close();

    cout << "Saved tree object: " << hash << endl;

}
