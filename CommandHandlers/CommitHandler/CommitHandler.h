#pragma once
#include <string>

class CommitHandler {
public:
    void handleCommit(const std::string& message);

private:
    std::string createTree();
};
