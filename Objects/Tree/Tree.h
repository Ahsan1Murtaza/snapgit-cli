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
     * @brief Tree operation.
     */
    Tree();
     /**
      * @brief ~Tree operation.
      */
     ~Tree();   // to clean up dynamically allocated memory

    /**
     * @brief addBlob operation.
     * @param filePath Parameter description.
     * @param blobHash Parameter description.
     */
    void addBlob(const std::string& filePath, const std::string& blobHash);

    /**
     * @brief getOrCreateSubTree operation.
     * @param folderName Parameter description.
     * @return Return value description.
     */
    Tree* getOrCreateSubTree(const std::string& folderName);

    /**
     * @brief buildFromIndex operation.
     * @param indexPath Parameter description.
     */
    void buildFromIndex(const std::string& indexPath);


    /**
     * @brief save operation.
     */
    void save() override;

    // Disable copy to avoid double-deletes
    /**
     * @brief Tree operation.
     */
    Tree(const Tree&) = delete;
    /**
     * @brief operator= operation.
     * @return Return value description.
     */
    Tree& operator=(const Tree&) = delete;

    // void showTreeElements(int depth = 0) const; // For development purpose

};

#endif // TREE_H
