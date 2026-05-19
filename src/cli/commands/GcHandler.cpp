// SPDX-License-Identifier: MIT
// Implementation for GcHandler.

#include "GcHandler.h"

#include "../utils/MarkReachable.h"
#include "../utils/RepoCheck.h"

#include <iostream>
#include <filesystem>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

static bool isHexHash(const string& s) {
    if (s.size() != 40) return false;
    for (char c : s) {
        if (!isxdigit((unsigned char)c)) return false;
    }
    return true;
}

static vector<pair<string, fs::path>> listAllObjectFiles() {
    vector<pair<string, fs::path>> objects;
    const fs::path objectsDir = ".snapgit/objects";
    if (!fs::exists(objectsDir)) return objects;

    for (const auto& entry : fs::directory_iterator(objectsDir)) {
        const string name = entry.path().filename().string();

        if (entry.is_directory() && name.size() == 2) {
            for (const auto& fileEntry : fs::directory_iterator(entry.path())) {
                if (!fileEntry.is_regular_file()) continue;
                string hash = name + fileEntry.path().filename().string();
                if (isHexHash(hash)) {
                    objects.emplace_back(hash, fileEntry.path());
                }
            }
        } else if (entry.is_regular_file() && isHexHash(name)) {
            objects.emplace_back(name, entry.path());
        }
    }

    return objects;
}

/**
 * @brief Handles the gc command workflow.
 * @param dryRun When true, only prints what would be deleted.
 */
void GcHandler::handleGc(bool dryRun) {
    if (!isRepoInitialized()) {
        cerr << "Error: Repository not initialized. Run 'snapgit init' first.\n";
        return;
    }

    const auto reachable = markReachableObjects();
    const auto onDisk = listAllObjectFiles();

    vector<pair<string, fs::path>> toRemove;
    toRemove.reserve(onDisk.size());

    for (const auto& [hash, path] : onDisk) {
        if (reachable.find(hash) == reachable.end()) {
            toRemove.emplace_back(hash, path);
        }
    }

    cout << "Garbage collection " << (dryRun ? "(dry run)" : "") << ":\n";
    cout << "  Reachable objects: " << reachable.size() << "\n";
    cout << "  Objects on disk:   " << onDisk.size() << "\n";
    cout << "  To remove:         " << toRemove.size() << "\n";

    if (toRemove.empty()) {
        cout << "Nothing to clean up.\n";
        return;
    }

    cout << (dryRun ? "Would remove:\n" : "Removing:\n");
    for (const auto& [hash, path] : toRemove) {
        cout << "  " << hash << " (" << path.string() << ")\n";
        if (!dryRun) {
            error_code ec;
            fs::remove(path, ec);
            if (ec) {
                cerr << "  Warning: could not remove " << path << ": " << ec.message() << "\n";
            }
        }
    }

    if (!dryRun) {
        for (const auto& entry : fs::directory_iterator(".snapgit/objects")) {
            if (!entry.is_directory()) continue;
            const string name = entry.path().filename().string();
            if (name.size() != 2) continue;
            if (fs::is_empty(entry.path())) {
                fs::remove(entry.path());
            }
        }
        cout << "Done. Removed " << toRemove.size() << " unreachable object(s).\n";
    } else {
        cout << "Dry run complete. Re-run without --dry-run to delete.\n";
    }
}
