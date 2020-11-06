#include <iostream>

#include "src/index.h"
#include "src/record.h"
#include "src/btree.h"
#include "src/bstar.h"
#include "src/bplustree.h"

using namespace std;

int main() {
    const std::string INDEXFILE{"index.db"};
    // Index<BTree, Record<80, 6>, 10> btreeIndex(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    // btreeIndex.execute();
    // btreeIndex.print_tree();
    // Index<bstar, Record<100, 6>, 10> bstarIndex(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    // bstarIndex.execute();
    // bstarIndex.print_tree();
    Index<BPlusTree, Record<80, 6>, 10> bplusIndex(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    bplusIndex.execute();
    bplusIndex.print_tree();
    return 0;
}