#include <iostream>

#include "index.h"
#include "record.h"
#include "btree.h"

using namespace std;

int main() {
    const std::string INDEXFILE{"index.db"};
    Index<BTree, Record<80, 5>, 10> btreeIndex(INDEXFILE, "../data/processed_German.txt", "../data/processed_Italian.txt");
    // btreeIndex.execute();
    std::ofstream testout("testout.txt");
    btreeIndex.print(testout);
    return 0;
}