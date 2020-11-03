#include <gtest/gtest.h>
#include <btree.h>
#include <bplustree.h>
#include <bstar.h>
#include <pagemanager.h>


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
#define BTREE_ORDER 4

struct DiskBasedBtree : public ::testing::Test{};
struct DiskBasedBPlusTree : public ::testing::Test{};
struct DiskBasedBStarTree : public ::testing::Test{};

TEST_F(DiskBasedBtree, IndexingRandomElements) {
  bool trunc_file = true;
  std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index", trunc_file);
  std::cout << "PAGE_SIZE: " << PAGE_SIZE << std::endl;
  std::cout << "BTREE_ORDER: " << BTREE_ORDER << std::endl;
  BTree<char, BTREE_ORDER> bt(pm);
  std::string values = "qwertyuioplkjhgfdsazxcvbnm";
  for(auto c : values) {
    bt.insert(c);
    bt.print_tree();
  }
  std::ostringstream out;
  bt.print(out);
  std::sort(values.begin(), values.end());
  EXPECT_EQ(out.str(), values.c_str());
}
 
TEST_F(DiskBasedBtree, Persistence) {
  std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index");
  BTree<char, BTREE_ORDER> bt(pm);
  std::string values = "1234567890=";
  for(auto c : values) {
    bt.insert(c);
  }
  bt.print_tree();

  std::ostringstream out;
  bt.print(out);
  std::string all_values = "qwertyuioplkjhgfdsazxcvbnm1234567890=";
  std::sort(all_values.begin(), all_values.end());
  EXPECT_EQ(out.str(), all_values.c_str());
}

/* B+Tree Tests */

TEST_F(DiskBasedBPlusTree, IndexingRandomElements) {
    bool trunc_file = true;
    std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index", trunc_file);
    std::cout << "PAGE_SIZE: " << PAGE_SIZE << std::endl;
    std::cout << "BTREE_ORDER: " << BTREE_ORDER << std::endl;
    BPlusTree<char, BTREE_ORDER> bt(pm);
    std::string values = "qwertyuioplkjhgfdsazxcvbnm";
    for(auto c : values) {
        bt.insert(c);
        bt.showTree();
    }
    std::ostringstream out;
    bt.print(out);
    std::sort(values.begin(), values.end());
    EXPECT_EQ(out.str(), values.c_str());
}

TEST_F(DiskBasedBPlusTree, Persistence) {
    std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index");
    BPlusTree<char, BTREE_ORDER> bt(pm);
    std::string values = "1234567890=";
    for(auto c : values) {
        bt.insert(c);
    }
    bt.showTree();

    std::ostringstream out;
    bt.print(out);
    std::string all_values = "qwertyuioplkjhgfdsazxcvbnm1234567890=";
    std::sort(all_values.begin(), all_values.end());
    EXPECT_EQ(out.str(), all_values.c_str());
}


/* B*Tree Tests */

TEST_F(DiskBasedBStarTree, IndexingRandomElements) {
    bool trunc_file = true;
    std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index", trunc_file);
    std::cout << "PAGE_SIZE: " << PAGE_SIZE << std::endl;
    std::cout << "BTREE_ORDER: " << BTREE_ORDER << std::endl;
    bstar<char, BTREE_ORDER> bt(pm);
    std::string values = "qwertyuioplkjhgfdsazxcvbnm";
    for(auto c : values) {
        bt.insert(c);
        bt.print_tree();
    }
    std::ostringstream out;
    bt.print(out);
    std::sort(values.begin(), values.end());
    EXPECT_EQ(out.str(), values.c_str());
}

TEST_F(DiskBasedBStarTree, Persistence) {
    std::shared_ptr<pagemanager> pm = std::make_shared<pagemanager>("BTree.index");
    bstar<char, BTREE_ORDER> bt(pm);
    std::string values = "1234567890=";
    for(auto c : values) {
        bt.insert(c);
    }
    bt.print_tree();

    std::ostringstream out;
    bt.print(out);
    std::string all_values = "qwertyuioplkjhgfdsazxcvbnm1234567890=";
    std::sort(all_values.begin(), all_values.end());
    EXPECT_EQ(out.str(), all_values.c_str());
}