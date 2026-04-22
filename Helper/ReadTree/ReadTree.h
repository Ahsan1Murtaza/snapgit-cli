// SPDX-License-Identifier: MIT
// Declarations for ReadTree.

#ifndef READ_TREE_H
#define READ_TREE_H

#include <string>
#include <unordered_map>

/**
 * @brief Reads tree files from repository storage.
 * @param treeHash Hash of the tree object to read.
 * @param basePath Base path used while walking tree entries.
 * @return Requested string value.
 */
std::unordered_map<std::string, std::string> readTreeFiles(const std::string& treeHash, const std::string& basePath = "");

#endif // READ_TREE_H
