#include <gtest/gtest.h>
#include <btree.h>
#include <bplustree.h>
#include <bstar.h>
#include <pagemanager.h>
#include <record.h>
#include <numeric>


// PAGE_SIZE 64 bytes
#define PAGE_SIZE  64 

// Other examples:
// PAGE_SIZE 1024 bytes => 1Kb
// PAGE_SIZE 1024*1024 bytes => 1Mb

// PAGE_SIZE = 2 * sizeof(long) +  (BTREE_ORDER + 1) * sizeof(int) + (BTREE_ORDER + 2) * sizeof(long)  
// PAGE_SIZE = 2 * sizeof(long) +  (BTREE_ORDER) * sizeof(int) + sizeof(int) + (BTREE_ORDER) * sizeof(long) + 2 * sizeof(long)
// PAGE_SIZE = (BTREE_ORDER) * (sizeof(int) + sizeof(long))  + 2 * sizeof(long) + sizeof(int) +  2 * sizeof(long)
//  BTREE_ORDER = PAGE_SIZE - (2 * sizeof(long) + sizeof(int) +  2 * sizeof(long)) /  (sizeof(int) + sizeof(long))
//#define BTREE_ORDER   ((PAGE_SIZE - (2 * sizeof(long) + sizeof(int) +  2 * sizeof(long)) ) /  (sizeof(int) + sizeof(long)))
#define BTREE_ORDER 3

struct DiskBasedBtree : public ::testing::Test{};
struct DiskBasedBPlusTree : public ::testing::Test{};
struct DiskBasedBStarTree : public ::testing::Test{};
struct RecordTest : public ::testing::Test{};

TEST_F(RecordTest, Operators){
    std::string palabra = "palabra2";
    std::string palabra2 = "palabra";
    Record<> myRecord(palabra.c_str());
    Record<> myRecord2(palabra2.c_str());
    Record<> myRecord3;


    std::cout << "R1: " << myRecord << std::endl;
    std::cout << "R2: " << myRecord2 << std::endl;
    myRecord3 = myRecord2;
    std::cout << "R3: " << myRecord2 << std::endl;
    std::cout << "Equal: " << (myRecord2 == myRecord) << std::endl;
    std::cout << "Diff: " << (myRecord2 != myRecord) << std::endl;
    std::cout << "Less: " << (myRecord2 < myRecord) << std::endl;
    std::cout << "Less equal: " << (myRecord2 <= myRecord) << std::endl;
    std::cout << "Greater: " << (myRecord2 > myRecord) << std::endl;
    std::cout << "Greater equal: " << (myRecord2 >= myRecord) << std::endl;

}

TEST_F(DiskBasedBtree, IndexingRandomElements) {
  bool trunc_file = true;
  std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index", trunc_file);
  std::cout << "PAGE_SIZE: " << PAGE_SIZE << std::endl;
  std::cout << "BTREE_ORDER: " << BTREE_ORDER << std::endl;
  BTree<Record<>, BTREE_ORDER> bt(pm);

  std::vector<std::string> all_values = {"rojo", "azul", "amarillo", "verde", "morado", "naranja", "blanco", "negro", "marron", "gris"};
  for(auto c : all_values) {
    Record<> r(c.c_str());
    bt.insert(r);
    bt.print_tree();
  }
  std::ostringstream out;
   bt.print(out);
  std::sort(all_values.begin(), all_values.end());
  std::string result = std::accumulate(all_values.begin(), all_values.end(), std::string(""));
  EXPECT_EQ(out.str(), result.c_str());
}

TEST_F(DiskBasedBtree, Persistence) {
     std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index");
    BTree<Record<>, BTREE_ORDER> bt(pm);
    std::vector<std::string> all_values = {"rojo", "azul", "amarillo", "verde", "morado", "naranja", "blanco", "negro", "marron", "gris"};
    std::vector<std::string> values = {"cyan", "magenta", "celeste", "limon"};
    for(auto c : values) {
        all_values.push_back(c);
        Record<> r(c.c_str());
        bt.insert(r);
        bt.print_tree();
    }
    bt.print_tree();

    std::ostringstream out;
    bt.print(out);
    std::sort(all_values.begin(), all_values.end());
    std::string result = std::accumulate(all_values.begin(), all_values.end(), std::string(""));
    EXPECT_EQ(out.str(), result.c_str());
}

/* B+Tree Tests */

TEST_F(DiskBasedBPlusTree, IndexingRandomElements) {
    bool trunc_file = true;
    std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index", trunc_file);
    std::cout << "PAGE_SIZE: " << PAGE_SIZE << std::endl;
    std::cout << "BTREE_ORDER: " << BTREE_ORDER << std::endl;
    BPlusTree<Record<>, BTREE_ORDER> bt(pm);

    std::vector<std::string> all_values = {"rojo", "azul", "amarillo", "verde", "morado", "naranja", "blanco", "negro", "marron", "gris"};
    for(auto c : all_values) {
        Record<> r(c.c_str());
        bt.insert(r);
        bt.showTree();
    }
    std::ostringstream out;
    bt.print(out);
    std::sort(all_values.begin(), all_values.end());
    std::string result = std::accumulate(all_values.begin(), all_values.end(), std::string(""));
    EXPECT_EQ(out.str(), result.c_str());
}

TEST_F(DiskBasedBPlusTree, Persistence) {
    std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index");
    BPlusTree<Record<>, BTREE_ORDER> bt(pm);
    std::vector<std::string> all_values = {"rojo", "azul", "amarillo", "verde", "morado", "naranja", "blanco", "negro", "marron", "gris"};
    std::vector<std::string> values = {"cyan", "magenta", "celeste", "limon"};
    for(auto c : values) {
        all_values.push_back(c);
        Record<> r(c.c_str());
        bt.insert(r);
        bt.showTree();
    }
    bt.showTree();

    std::ostringstream out;
    bt.print(out);
    std::sort(all_values.begin(), all_values.end());
    std::string result = std::accumulate(all_values.begin(), all_values.end(), std::string(""));
    EXPECT_EQ(out.str(), result.c_str());
}


/* B*Tree Tests */

TEST_F(DiskBasedBStarTree, IndexingRandomElements) {
    bool trunc_file = true;
    std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index", trunc_file);
    std::cout << "PAGE_SIZE: " << PAGE_SIZE << std::endl;
    std::cout << "BTREE_ORDER: " << BTREE_ORDER << std::endl;
    bstar<Record<>, BTREE_ORDER> bt(pm);

    std::vector<std::string> all_values = {"rojo", "azul", "amarillo", "verde", "morado", "naranja", "blanco", "negro", "marron", "gris"};
    for(auto c : all_values) {
        Record<> r(c.c_str());
        bt.insert(r);
        bt.print_tree();
    }
    std::ostringstream out;
    bt.print(out);
    std::sort(all_values.begin(), all_values.end());
    std::string result = std::accumulate(all_values.begin(), all_values.end(), std::string(""));
    EXPECT_EQ(out.str(), result.c_str());
}

TEST_F(DiskBasedBStarTree, Persistence) {
    std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index");
    bstar<Record<>, BTREE_ORDER> bt(pm);
    std::vector<std::string> all_values = {"rojo", "azul", "amarillo", "verde", "morado", "naranja", "blanco", "negro", "marron", "gris"};
    std::vector<std::string> values = {"cyan", "magenta", "celeste", "limon"};
    for(auto c : values) {
        all_values.push_back(c);
        Record<> r(c.c_str());
        bt.insert(r);
        bt.print_tree();
    }
    bt.print_tree();

    std::ostringstream out;
    bt.print(out);
    std::sort(all_values.begin(), all_values.end());
    std::string result = std::accumulate(all_values.begin(), all_values.end(), std::string(""));
    EXPECT_EQ(out.str(), result.c_str());
}