// SPDX-License-Identifier: MIT
// Declarations for Ignore.

#ifndef IGNORE_H
#define IGNORE_H

#include <string>
#include <vector>

/**
 * @brief Reads ignore patterns from repository storage.
 * @return Requested string value.
 */
std::vector<std::string> readIgnorePatterns();
/**
 * @brief Checks whether ignored path.
 * @param relativePath Path relative to the repository root.
 * @param patterns Ignore patterns loaded from .mygitignore.
 * @return True when the check succeeds; otherwise false.
 */
bool isIgnoredPath(const std::string& relativePath, const std::vector<std::string>& patterns);

#endif // IGNORE_H
