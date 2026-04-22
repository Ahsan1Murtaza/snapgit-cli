// SPDX-License-Identifier: MIT
// Declarations for ConfigHandler.

#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include <string>

class ConfigHandler {
public:
    /**
     * @brief handleConfig operation.
     * @param name Parameter description.
     * @param email Parameter description.
     */
    void handleConfig(const std::string& name, const std::string& email);
};

#endif
