// SPDX-License-Identifier: MIT
// Declarations for RestoreTree.

#ifndef RESTORETREE_H
#define RESTORETREE_H

#include <string>
using namespace std;

/**
 * @brief restoreTree operation.
 * @param treeHash Parameter description.
 * @param basePath Parameter description.
 */
void restoreTree(const string& treeHash, const string& basePath = "");
/**
 * @brief writeIndexFile operation.
 */
void writeIndexFile();  // ✨ NEW: Declare new function

#endif
