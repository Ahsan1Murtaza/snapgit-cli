// SPDX-License-Identifier: MIT
// Declarations for MarkReachable.

#ifndef MARK_REACHABLE_H
#define MARK_REACHABLE_H

#include <string>
#include <unordered_set>

/**
 * @brief Collects all object hashes reachable from refs, HEAD, and index.
 * @return Set of reachable object hashes (commits, trees, blobs).
 */
std::unordered_set<std::string> markReachableObjects();

#endif
