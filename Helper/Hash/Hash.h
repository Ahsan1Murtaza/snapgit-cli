// SPDX-License-Identifier: MIT
// Declarations for Hash.

#ifndef HASH_H
#define HASH_H

#include <string>

/**
 * @brief Performs sha1.
 * @param data Input content to hash.
 * @return Requested string value.
 */
std::string sha1(const std::string& data);

#endif // HASH_H
