#include "Hash.h"
#include <iomanip> // for std::setw and std::setfill
#include <sstream> 
#include <openssl/sha.h> // OpenSSL library for SHA-1
#include <iostream>

/*
Function to compute SHA-1 hash of input data
Parameters:
- data: Input string data to be hashed
Returns:
- A hexadecimal string representation of the SHA-1 hash

Total length of SHA-1 hash is 40 characters (160 bits)
20 char is converted to 40 char hex string
*/
std::string sha1(const std::string& data) {
    unsigned char hash[SHA_DIGEST_LENGTH]; // SHA_DIGEST_LENGTH is 20 bytes

    /*
    Compute SHA-1 hash of the input data
    Parameters:
    - data.c_str(): Pointer to the input data
    - data.size(): Size of the input data
    - hash: Output buffer to store the computed hash
    */
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash); 

    // hash is now filled with the SHA-1 hash bytes (20 bytes / 160 bits)

    std::ostringstream ss; // Convert hash bytes to hex string

    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i){ 
        // Convert each byte to a two-digit hexadecimal representation
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
     
    }

    return ss.str();
}
