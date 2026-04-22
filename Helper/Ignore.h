// SPDX-License-Identifier: MIT
// Declarations for Ignore.

#ifndef IGNORE_H
#define IGNORE_H

#include <string>
#include <vector>

/**
 * @brief readIgnorePatterns operation.
 * @return Return value description.
 */
std::vector<std::string> readIgnorePatterns();
/**
 * @brief isIgnoredPath operation.
 * @param relativePath Parameter description.
 * @param patterns Parameter description.
 * @return Return value description.
 */
bool isIgnoredPath(const std::string& relativePath, const std::vector<std::string>& patterns);

#endif // IGNORE_H
