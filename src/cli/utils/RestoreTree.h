// SPDX-License-Identifier: MIT
// Declarations for RestoreTree.

#ifndef RESTORETREE_H
#define RESTORETREE_H

#include <string>


using namespace std;

/**
 * @brief Performs restore tree.
 * @param treeHash Hash of the tree object to read.
 * @param basePath Base path used while walking tree entries.
 */
void restoreTree(const string& treeHash, const string& basePath = "");
void writeIndexFile();

#endif
