// SPDX-License-Identifier: MIT
// Declarations for Tree.

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
    /**
     * @brief Constructs a Tree instance.
     */
    Tree();
     ~Tree();   // to clean up dynamically allocated memory

    /**
     * @brief Performs add blob.
     * @param filePath Path to the target file relative to the repository root.
     * @param blobHash Hash of the blob object.
     */
    void addBlob(const std::string& filePath, const std::string& blobHash);

    /**
     * @brief Returns or create sub tree.
     * @param folderName Folder name for the subtree node.
     * @return Pointer to the requested object; may be null when unavailable.
     */
    Tree* getOrCreateSubTree(const std::string& folderName);

    /**
     * @brief Performs build from index.
     * @param indexPath Path to the index file.
     */
    void buildFromIndex(const std::string& indexPath);


    /**
     * @brief Performs save.
     */
    void save() override;

    // Disable copy to avoid double-deletes
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;

    // void showTreeElements(int depth = 0) const; // For development purpose

};

#endif // TREE_H
