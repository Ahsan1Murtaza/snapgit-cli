// SPDX-License-Identifier: MIT
// Declarations for UpdateHead.

#ifndef UPDATE_HEAD_H
#define UPDATE_HEAD_H

#include <string>


/**
 * @brief Updates head.
 * @param newHash Commit hash that HEAD should point to.
 */
void updateHead(const std::string& newHash);

#endif // UPDATE_HEAD_H
