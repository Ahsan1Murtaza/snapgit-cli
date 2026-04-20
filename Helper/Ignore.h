#ifndef IGNORE_H
#define IGNORE_H

#include <string>
#include <vector>

std::vector<std::string> readIgnorePatterns();
bool isIgnoredPath(const std::string& relativePath, const std::vector<std::string>& patterns);

#endif // IGNORE_H
