// SPDX-License-Identifier: MIT
// Declarations for GetCurrentCommitHash.

#ifndef GET_CURRENT_COMMIT_HASH_H
#define GET_CURRENT_COMMIT_HASH_H

#include<string>

/**
 * @brief Returns current commit hash.
 * @return Current commit hash, or an empty string when unavailable.
 */
std::string getCurrentCommitHash();

#endif // GET_CURRENT_COMMIT_HASH_H
