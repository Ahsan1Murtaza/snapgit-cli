// SPDX-License-Identifier: MIT
// Declarations for ReadIndex.

#pragma once
#include <unordered_map>
#include <string>

/**
 * @brief Reads index from repository storage.
 * @return Requested string value.
 */
std::unordered_map<std::string, std::string> readIndex();
