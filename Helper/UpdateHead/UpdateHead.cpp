#include "UpdateHead.h"

#include <fstream>

void updateHead(const std::string& newHash) {
    std::ofstream out(".mygit/refs/heads/main", std::ios::trunc);

    out << newHash;
    out.close();
}