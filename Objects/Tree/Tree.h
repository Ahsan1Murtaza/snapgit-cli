#include "../../Objects/Object.h"
#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <map>
#include <string>


class Tree : public Object {
    private:
    std::map<std::string, std::string> blobs; // fileName -> blobHash
    std::map<std::string, Tree*> subTrees; // folderName -> Tree object

    std::string treeContent; // To store content of tree
public:
    Tree();
     ~Tree();   // to clean up dynamically allocated memory

    void addBlob(const std::string& filePath, const std::string& blobHash);

    Tree* getOrCreateSubTree(const std::string& folderName);

    void buildFromIndex(const std::string& indexPath);


    void save() override;

    // Disable copy to avoid double-deletes
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    // void showTreeElements(int depth = 0) const; // For development purpose

};

#endif // TREE_H