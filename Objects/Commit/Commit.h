#include "../../Objects/Object.h"
#include <string>
#include <vector>

#ifndef COMMIT_H
#define COMMIT_H

class Commit : public Object {
private: 
    std::string treeHash;
    std::vector<std::string> parentHashes;
    std::string author;
    std::string email;
    std:: string message;
public:
    Commit(const string& treeHash, const std::vector<std::string>& parentHashes, const string& message, const string& author, const string& email);

    void save() override;
};

#endif // COMMIT_H