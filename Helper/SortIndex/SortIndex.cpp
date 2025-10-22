#include "SortIndex.h"

#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream> 

using namespace std;

/* Function to sort the index file lexicographically
Reads the index file, sorts the entries, and writes them back.
*/
void sortIndexFile(const string& indexPath) {
    ifstream indexFile(indexPath);

    if (!indexFile) {
        cout << "Error: Unable to open index file for sorting.\n";
        return;
    }

    vector<string> entries;
    string line;

    // Read all lines from the index file
    while (getline(indexFile, line)) {
        entries.push_back(line);
    }
    indexFile.close();

    // Sort the entries lexicographically
    sort(entries.begin(), entries.end());

    // Write the sorted entries back to the index file
    ofstream outFile(indexPath, ios::trunc); // Open in truncate mode (make file empty)
    if (!outFile) {
        cout << "Error: Unable to open index file for writing sorted entries.\n";
        return;
    }
    for (const string& entry : entries) {
        outFile << entry << "\n";
    }
    outFile.close();

}