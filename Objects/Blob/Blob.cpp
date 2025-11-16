#include "Blob.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <filesystem>

#include "../../Helper/Hash/Hash.h"

using namespace std;

/*
Constructor to create a Blob object from a file path.
Blob data Format: "blob <size>\0<content>"
Call sha1 on the blob data to get the hash.
*/
Blob::Blob(const string& filePath) : Object("blob"), filePath(filePath) {
    // Read file content
    ifstream file(filePath, ios::binary);
    if (file) {
        stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
        file.close();
    }
    else {
        cerr << "Error: Unable to open file " << filePath << endl;
        return;
    }

    // Blob format: "blob <size>\0<content>"
    string blobData = "blob " + to_string(content.size()) + "\0" + content;


    hash = sha1(blobData); // 40-digit SHA-1 hash hex string (eg: "e69de29bb2d1d6434b8b29ae775ad8c2e48c5391")

}

void Blob::save() {
    // Save the blob data to a file
    string dir = ".mygit/objects/" + hash.substr(0, 2);
    string file = dir + "/" + hash.substr(2);

    filesystem::create_directories(dir); // Create directory if it doesn't exist

    ofstream out(file, ios::binary);
    if (out) {
        out << content;
        out.close();
    }
}
