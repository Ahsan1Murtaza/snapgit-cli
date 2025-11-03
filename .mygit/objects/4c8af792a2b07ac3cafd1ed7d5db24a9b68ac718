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

    Object(string t) : type(t){}

    string getType() const { return type; }
    string getHash() const { return hash; }

    virtual void save() = 0;  // Pure virtual function to save the object
    
};

#endif // OBJECT_H

