// MergeHandler.h
#ifndef MERGE_HANDLER_H
#define MERGE_HANDLER_H
#include <string>
class MergeHandler {
public:
    MergeHandler() = default;
    void handleMerge(const std::string& otherBranch);
};
#endif
