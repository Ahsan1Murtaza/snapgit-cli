// SPDX-License-Identifier: MIT
// Declarations for Object.

#ifndef OBJECT_H
#define OBJECT_H

#include <string>
using namespace std;

/*
This class is a base for Blob, Tree, Commit
It has type and hash of object
*/
class Object {
public:
    string type; // blob, tree, commit
    string hash;

    /**
     * @brief Object operation.
     * @param t Parameter description.
     */
    Object(string t) : type(t){}

    /**
     * @brief getType operation.
     * @return Return value description.
     */
    string getType() const { return type; }
    /**
     * @brief getHash operation.
     * @return Return value description.
     */
    string getHash() const { return hash; }

    /**
     * @brief save operation.
     */
    virtual void save() = 0;  // Pure virtual function to save the object
    
};

#endif // OBJECT_H

