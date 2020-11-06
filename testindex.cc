#include <iostream>

#include "src/index.h"
#include "src/record.h"
#include "src/btree.h"

using namespace std;

int main() {
    const std::string INDEXFILE{"index.db"};
    Index<BTree, Record<80, 6>, 10> btreeIndex(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    btreeIndex.execute();
    btreeIndex.print_tree();
    return 0;
}