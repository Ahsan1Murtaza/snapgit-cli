// SPDX-License-Identifier: MIT
// Declarations for Blob.

#ifndef BLOB_H
#define BLOB_H

#include "../../Objects/Object.h"
class Blob : public Object {
private:
    string content;
    string filePath;

public:
    /**
     * @brief Blob operation.
     * @param filePath Parameter description.
     */
    Blob(const string& filePath);
    /**
     * @brief save operation.
     */
    void save() override;
};


#endif // BLOB_H
