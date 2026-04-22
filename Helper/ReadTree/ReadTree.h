// SPDX-License-Identifier: MIT
// Declarations for ReadTree.

#ifndef READ_TREE_H
#define READ_TREE_H

#include <string>
#include <unordered_map>

/**
 * @brief readTreeFiles operation.
 * @param treeHash Parameter description.
 * @param basePath Parameter description.
 * @return Return value description.
 */
std::unordered_map<std::string, std::string> readTreeFiles(const std::string& treeHash, const std::string& basePath = "");

#endif // READ_TREE_H
