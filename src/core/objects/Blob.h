// SPDX-License-Identifier: MIT
// Declarations for Blob.

#ifndef BLOB_H
#define BLOB_H

#include "Object.h"
class Blob : public Object {
private:
    string content;
    string filePath;

public:
    /**
     * @brief Constructs a Blob instance.
     * @param filePath Path to the target file relative to the repository root.
     */
    Blob(const string& filePath);
    /**
     * @brief Performs save.
     */
    void save() override;
};


#endif // BLOB_H
