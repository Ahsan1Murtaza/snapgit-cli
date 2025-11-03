#pragma once
#include <string>

class LogHandler {
public:
    void showLog();

private:
    void printCommitDetails(const std::string& commitHash);
};
