#pragma once

#include "pagemanager.h"
#include <memory>
#include <optional>



template <class T, int BTREE_ORDER = 3> class BTree {
    class Page {
    public:
        T keys[BTREE_ORDER + 1];
        long children[BTREE_ORDER + 2];
        long data[BTREE_ORDER + 2];

        long pageId{-1};
        long nKeys{0};
        bool isLeaf{true};

        Page(long page_id) : pageId{page_id} {
            nKeys = 0;
            for (int i = 0; i < BTREE_ORDER + 2; i++) { //init with zeros
                children[i] = 0;
            }
        }
        Page(long page_id, bool is_leaf) : pageId{page_id}, isLeaf{is_leaf} {
            nKeys = 0;
            for (int i = 0; i < BTREE_ORDER + 2; i++) { //init with zeros
                children[i] = 0;
            }
        }

        void insertInPage(int pos, const T &value, const long dataId) {
            int j = nKeys;
            while (j > pos) {
                keys[j] = keys[j - 1];
                data[j] = data[j - 1];
                children[j + 1] = children[j];
                j--;
            }
            keys[j] = value;
            data[j] = dataId;
            children[j + 1] = children[j];
            nKeys++;
        }

        bool isOverflow() { return nKeys > BTREE_ORDER; }

    };

    struct Header {
    long rootId{1};
    long nPages{0};
    } header;
    
    enum state {
      BT_OVERFLOW,
      BT_UNDERFLOW,
      NORMAL,
    };

    typedef std::shared_ptr<pagemanager> pageManager;
    typedef Page BTreePage;

    pageManager pm;

public:
    
    BTree(pageManager pm) : pm{pm} {
        if (pm->is_empty()) {
            BTreePage root(header.rootId, true);
            pm->save(root.pageId, root);
            header.nPages++;
            pm->save(0, header);
        } else 
            pm->recover(0, header);
    }

    BTreePage createPage(bool isLeaf) {
        header.nPages++;
        BTreePage newPage(header.nPages, isLeaf);
        pm->save(0, header);
        return newPage;
    }

    BTreePage readPage(long pageId) {
        BTreePage newPage(-1);
        pm->recover(pageId, newPage);
        return newPage;
    }

    bool writePage(long pageId, BTreePage page) {
        pm->save(pageId, page);
        return true;
    }

    void insert(const T &value, const long dataId = -1) {
        BTreePage root = readPage(header.rootId);
        int state = insert(root, value, dataId);
        if (state == BT_OVERFLOW) splitRoot();
    }

    int insert(BTreePage &ptr, const T &value, const long dataId) {
        int pos = 0;
        while (pos < ptr.nKeys && ptr.keys[pos] < value)
            pos++;

        if (ptr.isLeaf) {
            ptr.insertInPage(pos, value, dataId);
            writePage(ptr.pageId, ptr);
        } else {
            long pageId = ptr.children[pos];
            BTreePage child = readPage(pageId);
            int state = insert(child, value, dataId);
            if (state == BT_OVERFLOW) split(ptr, pos);
        }
        return ptr.isOverflow() ? BT_OVERFLOW : NORMAL;
    }

    void split(BTreePage &parent, int pos) {
        BTreePage overflowPage = this->readPage(parent.children[pos]);
        BTreePage newPage = this->createPage(overflowPage.isLeaf);

        int iter = BTREE_ORDER / 2;
        int i;
        overflowPage.nKeys= iter;
        parent.insertInPage(pos, overflowPage.keys[iter], overflowPage.data[iter]);//promotion
        iter++; // the middle element

        for (i = 0; iter < BTREE_ORDER + 1; i++) {
            newPage.children[i] = overflowPage.children[iter];
            newPage.keys[i] = overflowPage.keys[iter];
            newPage.nKeys++;
            iter++;
        }
        newPage.children[i] = overflowPage.children[iter];

        parent.children[pos] = overflowPage.pageId;
        parent.children[pos + 1] = newPage.pageId;

        writePage(parent.pageId, parent);
        writePage(overflowPage.pageId, overflowPage);
        writePage(newPage.pageId, newPage);
    }

    void splitRoot() {
        BTreePage overflowPage = this->readPage(this->header.rootId);
        BTreePage leftPage = this->createPage(overflowPage.isLeaf);
        BTreePage rightPage = this->createPage(overflowPage.isLeaf);

        int pos = 0;
        int iter = 0;
        int i;

        for (i = 0; iter < BTREE_ORDER / 2; i++) {
            leftPage.children[i] = overflowPage.children[iter];
            leftPage.keys[i] = overflowPage.keys[iter];
            leftPage.nKeys++;
            iter++;
        }

        leftPage.children[i] = overflowPage.children[iter];
        iter++; // the middle element

        for (i = 0; iter < BTREE_ORDER + 1; i++) {
            rightPage.children[i] = overflowPage.children[iter];
            rightPage.keys[i] = overflowPage.keys[iter];
            rightPage.nKeys++;
            iter++;
        }
        rightPage.children[i] = overflowPage.children[iter];

        overflowPage.children[pos] = leftPage.pageId;
        overflowPage.keys[0] = overflowPage.keys[BTREE_ORDER / 2];
        overflowPage.children[pos + 1] = rightPage.pageId;
        overflowPage.nKeys = 1;
        overflowPage.isLeaf = false;

        writePage(overflowPage.pageId, overflowPage);
        writePage(leftPage.pageId, leftPage);
        writePage(rightPage.pageId, rightPage);
    }

    void print(std::ostream& out) {
        BTreePage root = readPage(header.rootId);
        print(root, 0, out);
    }

    void print(BTreePage &ptr, int level, std::ostream& out) {
        int i;
        for (i = 0; i < ptr.nKeys; i++) {
          if (ptr.children[i]) {
            BTreePage child = readPage(ptr.children[i]);
            print(child, level + 1, out);
          }
          out << ptr.keys[i] << std::endl;
        }
        if (ptr.children[i]) {
          BTreePage child = readPage(ptr.children[i]);
          print(child, level + 1, out);
        }
    }
public:
    

  void print_tree() {
    BTreePage root = readPage(header.rootId);
      print_tree(root, 0);
    std::cout << "________________________\n";
  }

  void print_tree(BTreePage &ptr, int level) {
    int i;
    for (i = ptr.nKeys - 1; i >= 0; i--) {
      if (ptr.children[i + 1]) {
        BTreePage child = readPage(ptr.children[i + 1]);
          print_tree(child, level + 1);
      }

      for (int k = 0; k < level; k++) {
        std::cout << "    ";
      }
      std::cout << ptr.keys[i] << "\n";
    }
    if (ptr.children[i + 1]) {
      BTreePage child = readPage(ptr.children[i + 1]);
        print_tree(child, level + 1);
    }
  }

    T* find(T &value) {
        BTreePage root = readPage(header.rootId);
        return find(root, value);
    }
    
    T* find(BTreePage &ptr, T &value) {
        int pos = 0;
        while (pos < ptr.nKeys && ptr.keys[pos] < value)
            pos++;
        
        if (!ptr.isLeaf) {
            long pageId = ptr.children[pos];
            BTreePage child = readPage(pageId);
            return find(child, value);

        }
        T *result;
        result = ptr.keys[pos] == value? &ptr.keys[pos]: nullptr;
        return result;
    }


};