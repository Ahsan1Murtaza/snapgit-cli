#pragma once
#include <string>

class RemoveHandler {
public:
    // cached = true -> remove from index only
    void handleRemove(const std::string& filePath, bool cached = false);
};
