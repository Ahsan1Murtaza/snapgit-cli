#include "../../Objects/Object.h"
#include <string>

#ifndef COMMIT_H
#define COMMIT_H

class Commit : public Object {
private: 
    std::string treeHash;
    std::string parentHash;
    std::string author;
    std::string email;
    std:: string message;
public:
    Commit(const string& treeHash, const string& parentHash, const string& message, const string& author, const string& email);

    void save() override;
};

#endif // COMMIT_H