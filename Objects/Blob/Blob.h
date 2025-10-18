#ifndef BLOB_H
#define BLOB_H

#include "../../Objects/Object.h"
class Blob : public Object {
private:
    string content;
    string filePath;

public:
    Blob(const string& filePath);
    void save() override;
};


#endif // BLOB_H