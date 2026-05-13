// SPDX-License-Identifier: MIT
// Implementation for ReadIndex.

#include "ReadIndex.h"

#include <fstream>

/**
 * @brief Reads index from repository storage.
 * @return Requested string value.
 */
std::unordered_map<std::string, std::string> readIndex() {
    std::unordered_map<std::string, std::string> result;
    std::ifstream in(".mygit/index");
    std::string path, hash;
    while (in >> path >> hash) result[path] = hash;
    return result;
}
