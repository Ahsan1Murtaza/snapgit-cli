#ifndef RESTORE_HANDLER_H
#define RESTORE_HANDLER_H

#include <string>

class RestoreHandler {
public:
    RestoreHandler() {};
    void handleRestore(const std::string& filePath);
};

#endif // RESTORE_HANDLER_H