#ifndef CONFIG_HANDLER_H
#define CONFIG_HANDLER_H

#include <string>

class ConfigHandler {
public:
    void handleConfig(const std::string& name, const std::string& email);
};

#endif
