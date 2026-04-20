#ifndef READ_TREE_H
#define READ_TREE_H

#include <string>
#include <unordered_map>

/**
 * Given a tree object hash, recursively return a map: path -> blobHash.
 * Expects tree object format lines: "<mode> <type> <hash> <path>"
 */
std::unordered_map<std::string, std::string> readTreeFiles(const std::string& treeHash, const std::string& basePath = "");

#endif // READ_TREE_H
