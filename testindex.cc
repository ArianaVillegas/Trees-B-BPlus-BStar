#include <iostream>

#include "src/index.h"
#include "src/record.h"
#include "src/btree.h"
#include "src/bstar.h"
#include "src/bplustree.h"

using namespace std;

int main() {
    const std::string INDEXFILE{"index.db"};
    Index<BTree, Record<100, 6>, 4> btreeIndex4(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    btreeIndex4.execute();
    btreeIndex4.experiment("data10.txt", "btree10_4.txt");
    btreeIndex4.experiment("data100.txt", "btree100_4.txt");
    btreeIndex4.experiment("data1000.txt", "btree1000_4.txt");
    btreeIndex4.experiment("data10000.txt", "btree10000_4.txt");

    Index<BTree, Record<100, 6>, 8> btreeIndex8(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    btreeIndex8.execute();
    btreeIndex8.experiment("data10.txt", "btree10_8.txt");
    btreeIndex8.experiment("data100.txt", "btree100_8.txt");
    btreeIndex8.experiment("data1000.txt", "btree1000_8.txt");
    btreeIndex8.experiment("data10000.txt", "btree10000_8.txt");

    Index<BTree, Record<100, 6>, 16> btreeIndex16(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    btreeIndex16.execute();
    btreeIndex16.experiment("data10.txt", "btree10_16.txt");
    btreeIndex16.experiment("data100.txt", "btree100_16.txt");
    btreeIndex16.experiment("data1000.txt", "btree1000_16.txt");
    btreeIndex16.experiment("data10000.txt", "btree10000_16.txt");

    Index<BTree, Record<100, 6>, 32> btreeIndex32(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    btreeIndex32.execute();
    btreeIndex32.experiment("data10.txt", "btree10_32.txt");
    btreeIndex32.experiment("data100.txt", "btree100_32.txt");
    btreeIndex32.experiment("data1000.txt", "btree1000_32.txt");
    btreeIndex32.experiment("data10000.txt", "btree10000_32.txt");

    Index<BTree, Record<100, 6>, 64> btreeIndex64(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    btreeIndex64.execute();
    btreeIndex64.experiment("data10.txt", "btree10_64.txt");
    btreeIndex64.experiment("data100.txt", "btree100_64.txt");
    btreeIndex64.experiment("data1000.txt", "btree1000_64.txt");
    btreeIndex64.experiment("data10000.txt", "btree10000_64.txt");

    Index<bstar, Record<100, 6>, 4> bstarIndex4(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    bstarIndex4.execute();
    bstarIndex4.experiment("data10.txt", "bstar10_4.txt");
    bstarIndex4.experiment("data100.txt", "bstar100_4.txt");
    bstarIndex4.experiment("data1000.txt", "bstar1000_4.txt");
    bstarIndex4.experiment("data10000.txt", "bstar10000_4.txt");

    Index<bstar, Record<100, 6>, 8> bstarIndex8(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    bstarIndex8.execute();
    bstarIndex8.experiment("data10.txt", "bstar10_8.txt");
    bstarIndex8.experiment("data100.txt", "bstar100_8.txt");
    bstarIndex8.experiment("data1000.txt", "bstar1000_8.txt");
    bstarIndex8.experiment("data10000.txt", "bstar10000_8.txt");

    Index<bstar, Record<100, 6>, 16> bstarIndex16(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    bstarIndex16.execute();
    bstarIndex16.experiment("data10.txt", "bstar10_16.txt");
    bstarIndex16.experiment("data100.txt", "bstar100_16.txt");
    bstarIndex16.experiment("data1000.txt", "bstar1000_16.txt");
    bstarIndex16.experiment("data10000.txt", "bstar10000_16.txt");

    Index<bstar, Record<100, 6>, 32> bstarIndex32(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    bstarIndex32.execute();
    bstarIndex32.experiment("data10.txt", "bstar10_32.txt");
    bstarIndex32.experiment("data100.txt", "bstar100_32.txt");
    bstarIndex32.experiment("data1000.txt", "bstar1000_32.txt");
    bstarIndex32.experiment("data10000.txt", "bstar10000_32.txt");

    Index<bstar, Record<100, 6>, 64> bstarIndex64(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    bstarIndex64.execute();
    bstarIndex64.experiment("data10.txt", "bstar10_64.txt");
    bstarIndex64.experiment("data100.txt", "bstar100_64.txt");
    bstarIndex64.experiment("data1000.txt", "bstar1000_64.txt");
    bstarIndex64.experiment("data10000.txt", "bstar10000_64.txt");

    // Index<BPlusTree, Record<100, 6>, 4> bplusIndex4(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    // bplusIndex4.execute();
    // bplusIndex4.experiment("data10.txt", "bplus10_4.txt");
    // bplusIndex4.experiment("data100.txt", "bplus100_4.txt");
    // bplusIndex4.experiment("data1000.txt", "bplus1000_4.txt");
    // bplusIndex4.experiment("data10000.txt", "bplus10000_4.txt");

    // Index<BPlusTree, Record<100, 6>, 8> bplusIndex8(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    // bplusIndex8.execute();
    // bplusIndex8.experiment("data10.txt", "bplus10_8.txt");
    // bplusIndex8.experiment("data100.txt", "bplus100_8.txt");
    // bplusIndex8.experiment("data1000.txt", "bplus1000_8.txt");
    // bplusIndex8.experiment("data10000.txt", "bplus10000_8.txt");

    // Index<BPlusTree, Record<100, 6>, 16> bplusIndex16(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    // bplusIndex16.execute();
    // bplusIndex16.experiment("data10.txt", "bplus10_16.txt");
    // bplusIndex16.experiment("data100.txt", "bplus100_16.txt");
    // bplusIndex16.experiment("data1000.txt", "bplus1000_16.txt");
    // bplusIndex16.experiment("data10000.txt", "bplus10000_16.txt");

    // Index<BPlusTree, Record<100, 6>, 32> bplusIndex32(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    // bplusIndex32.execute();
    // bplusIndex32.experiment("data10.txt", "bplus10_32.txt");
    // bplusIndex32.experiment("data100.txt", "bplus100_32.txt");
    // bplusIndex32.experiment("data1000.txt", "bplus1000_32.txt");
    // bplusIndex32.experiment("data10000.txt", "bplus10000_32.txt");

    // Index<BPlusTree, Record<100, 6>, 64> bplusIndex64(INDEXFILE, true, "data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt");
    // bplusIndex64.execute();
    // bplusIndex64.experiment("data10.txt", "bplus10_64.txt");
    // bplusIndex64.experiment("data100.txt", "bplus100_64.txt");
    // bplusIndex64.experiment("data1000.txt", "bplus1000_64.txt");
    // bplusIndex64.experiment("data10000.txt", "bplus10000_64.txt");

    return 0;
}