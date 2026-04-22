// SPDX-License-Identifier: MIT
// Declarations for GetUserInfo.

#ifndef CONFIG_HELPER_H
#define CONFIG_HELPER_H

#include <string>
#include <utility>

/**
 * @brief Returns user info from config.
 * @return Requested string value.
 */
std::pair<std::string, std::string> getUserInfoFromConfig();

#endif
