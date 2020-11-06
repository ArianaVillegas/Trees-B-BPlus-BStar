#pragma once
#include "pagemanager.h"
#include <memory>
#include <iostream>
#include <vector>
#include <cmath>

template <class T, int ORDER>
class BPlusTree;

template <class T, int ORDER>
class BPlusTreeIterator;

template<class T, int ORDER>
class BPlusTreeNode{

    T keys [ORDER + 1];
    long children [ORDER + 2];
    long records_id[ORDER + 1]; //id of the record on disk

    long n_keys = 0;
    bool is_leaf = false;

    long next_node  = -1; //link to the next node if is a leaf
    long prev_node = -1; //link to the previous node if is a leaf
    long disk_id = -1; //id of the index on disk

    void initChildrensWithZeros(){
        for (int i = 0; i < ORDER + 2; i++)
            children[i] = 0;
    }
public:

    /**
     * @brief Construct a new BPlusTreeNode object
         *
     * @param d_id diskManager id of the node on disk
     */
    BPlusTreeNode(long d_id){
        disk_id = d_id;
        initChildrensWithZeros();

    };

    /**
     * @brief Construct a new BPlusTreeNode object for leaf
     *
     * @param d_id
     * @param is_leaf
     */
    BPlusTreeNode(long d_id, bool is_leaf_flag){
        disk_id = d_id;
        is_leaf = is_leaf_flag;
        initChildrensWithZeros();
    };

    /**
     * @brief Function to insert a key_value value in a given position
     *
     * @param key_value
     * @param pos
     */
    void insertKeyInPosition(int pos, const T &key_value, const long record_id){
        //Move to the right until we find the pos of the key_value value
        for(int i = n_keys; i > pos; i--){
            keys[i] = keys[i - 1];
            records_id[i] = records_id[i - 1];
            children[i + 1] = children[i];
        }
        keys[pos] = key_value;
        records_id[pos] = record_id;
        children[pos + 1] = children [pos];
        n_keys += 1;

    };

    /**
     * @brief Check is the node is in overflow
     *
     * @return true overflow
     * @return false not overflow
     */
    bool isOverflow(){
        return n_keys > ORDER;
    }

    friend class BPlusTree<T, ORDER>;
    friend class BPlusTreeIterator<T,ORDER>;
};

/**
 * @brief BPlusTree class
 * 
 * @tparam T type of the key value
 * @tparam 3 default b+tree order 
 */
template<typename T, int ORDER = 3>
class BPlusTree{

  using node = BPlusTreeNode<T,ORDER>;
  using iterator = BPlusTreeIterator<T,ORDER>;
  using diskManager = std::shared_ptr<pagemanager>;

  enum state { BP_OVERFLOW, NORMAL}; //state of the node insertion

  diskManager disk_manager; // disk manager of the index file

  //struct to store the starting node and the number of existing nodes
  struct Header{
      long disk_id = 1;
      long n_nodes = 0;
  } header;

  protected:

    /**
     * @brief Create a BPlusTreeNode object by a isLeaf flag
     *
     * @param isLeaf if the node is a leaf node
     * @return node node created
     */
    node createNode(bool isLeaf){
        header.n_nodes++; //position of the new node
        disk_manager->save(0, header);
        node new_node(header.n_nodes, isLeaf);
        return new_node;
    }

    /**
     * @brief Create a BPlusTreeNode object by a disk_id and isLeaf flag,
     * Its create a node with the same disk id of a previous one,
     * it is used in the split function to overwrite an splitted
     * node with just the left child values
     *
     * @param disk_id the disk id of the previous node
     * @param isLeaf is the node leaf?
     * @return node node created
     */
    node createNode(long disk_id, bool isLeaf){
        node new_node(disk_id, isLeaf);
        return new_node;
    }


    /**
     * @brief Read a node from disk by a given disk id position
     *
     * @param disk_id
     * @return node
     */
    node readNode(long disk_id){
        node new_node(-1);
        disk_manager->recover(disk_id, new_node);
        return new_node;
    }

    /**
     * @brief Write a node to disk by a given disk id position
     *
     * @param disk_id
     * @param n
     */
    void writeNode(long disk_id, node n){
        disk_manager->save(disk_id, n);
    }

    /**
     * @brief Insert a value to a given node, to do this, it search
     * right position for the value, and insert on it if is a leaf node,
     * else call insert of a value to a child
     *
     * @param ptr_node
     * @param value
     * @return int
     */
    int insert(node &ptr_node, const T value, const long record_id){
        int pos = 0;

        //find position on node
        while (pos < ptr_node.n_keys && ptr_node.keys[pos] < value)
            pos++;

        if (ptr_node.is_leaf){
            ptr_node.insertKeyInPosition(pos, value, record_id);       //insert if is a leaf node
            writeNode(ptr_node.disk_id, ptr_node);
        } else {        //search for the child node to insert
            long page_id = ptr_node.children[pos];
            node child = readNode(page_id);
            int state = insert(child, value, record_id);
            if (state == BP_OVERFLOW){
                splitNode(ptr_node, pos);
            }
        }
        return ptr_node.isOverflow() ? BP_OVERFLOW : NORMAL; //the insertion status
    }

    /**
     * @brief split the root node in left and write child, by a left based split
     *
     */
    void splitRoot() {
        node ptr_node = readNode(header.disk_id);
        node left_node = createNode(ptr_node.is_leaf);
        node right_node = createNode(ptr_node.is_leaf);

        int iter_child; //for child
        int iter_keys = 0;//for keys
        int pos = 0;

        for (iter_child = 0; iter_keys < ORDER / 2; iter_child++) { //copy to left node
            left_node.children[iter_child] = ptr_node.children[iter_keys];
            left_node.keys[iter_child] = ptr_node.keys[iter_keys];
            left_node.records_id[iter_child] = ptr_node.records_id[iter_keys];
            left_node.n_keys++;
            iter_keys++;
        }
        left_node.children[iter_child] = ptr_node.children[iter_keys];

        if (ptr_node.is_leaf){ //if is the first split of the root node
            left_node.keys[iter_child] = ptr_node.keys[iter_keys]; //left base split
            left_node.records_id[iter_child] = ptr_node.records_id[iter_keys];
            left_node.n_keys++;
        }

        iter_keys++;

        for (iter_child = 0; iter_keys < ORDER + 1; iter_child++) {  //copy to right node
            right_node.children[iter_child] = ptr_node.children[iter_keys];
            right_node.keys[iter_child] = ptr_node.keys[iter_keys];
            right_node.records_id[iter_child] = ptr_node.records_id[iter_keys];
            right_node.n_keys++;
            iter_keys++;
        }

        right_node.children[iter_child] = ptr_node.children[iter_keys];

        if (ptr_node.is_leaf){ //link the children nodes if is the first split
            left_node.next_node = right_node.disk_id;
            right_node.prev_node = left_node.disk_id;
        }


        //update children array
        ptr_node.children[pos] = left_node.disk_id;

        ptr_node.keys[0] = ptr_node.keys[ORDER / 2];
        ptr_node.records_id[0] = ptr_node.records_id[ORDER/2];
        ptr_node.children[pos + 1] = right_node.disk_id;
        ptr_node.n_keys = 1;
        ptr_node.is_leaf = false;

        writeNode(ptr_node.disk_id, ptr_node);
        writeNode(left_node.disk_id, left_node);
        writeNode(right_node.disk_id, right_node);
    }


    /**
     * @brief split a node in left and write child, by a left based split
     *
     * @param parent_node parent of the node to be splitted
     * @param pos position of the node to be splitted in the parent node
     */
    void splitNode (node &parent_node, int pos){
        node ptr_node = readNode(parent_node.children[pos]);
        node left_node = createNode(parent_node.children[pos], ptr_node.is_leaf);
        node right_node = createNode(ptr_node.is_leaf);

        int iter_child;
        int iter_keys = 0;
        for (iter_child = 0; iter_keys < ORDER / 2; iter_child++) {
            left_node.children[iter_child] = ptr_node.children[iter_keys];
            left_node.keys[iter_child] = ptr_node.keys[iter_keys];
            left_node.records_id[iter_child] = ptr_node.records_id[iter_keys];
            left_node.n_keys++;
            iter_keys++;
        }
        left_node.children[iter_child] = ptr_node.children[iter_keys];

        if (ptr_node.is_leaf) { //if the node splitted is leaf
            left_node.keys[iter_child] = ptr_node.keys[iter_keys]; //left based split
            left_node.records_id[iter_child] = ptr_node.records_id[iter_keys];
            left_node.n_keys++;

            //update the next and previous nodes
            left_node.next_node = right_node.disk_id;
            left_node.prev_node = ptr_node.prev_node;
            right_node.prev_node = left_node.disk_id;
            right_node.next_node = ptr_node.next_node;

            //update the previous node of the next node before split
            if (right_node.next_node != -1){
                node temp = readNode(right_node.next_node);
                temp.prev_node = right_node.disk_id;
                writeNode(temp.disk_id, temp);
            }

        }
        parent_node.insertKeyInPosition(pos, ptr_node.keys[iter_keys], ptr_node.records_id[iter_keys]); //key promoted

        iter_keys++;

        for (iter_child = 0; iter_keys < ORDER + 1; iter_child++) {
            right_node.children[iter_child] = ptr_node.children[iter_keys];
            right_node.keys[iter_child] = ptr_node.keys[iter_keys];
            right_node.records_id[iter_child] = ptr_node.records_id[iter_keys];
            right_node.n_keys++;

            iter_keys++;
        }
        right_node.children[iter_child] = ptr_node.children[iter_keys];
        parent_node.children[pos] = left_node.disk_id;
        parent_node.children[pos + 1] = right_node.disk_id;
        parent_node.is_leaf = false; //update the parent to non leaf


        writeNode(parent_node.disk_id, parent_node);
        writeNode(left_node.disk_id, left_node);
        writeNode(right_node.disk_id, right_node);
    }

public:
    /**
     *@brief Default constructor
     * */
    BPlusTree(){};

    /**
     * @brief Construct a new BPlusTree object by a disk manager object
     * 
     * @param d_manager disk manager to write and read access on file
     */
    BPlusTree(diskManager d_manager){
        disk_manager = d_manager;

        if (!disk_manager->is_empty())
            disk_manager->recover(0, header);
        else{  
            //Init the file with the header info
            node root (header.disk_id, true);
            disk_manager->save(root.disk_id, root);
            header.n_nodes++;
            disk_manager->save(0, header);
        }
    }
    /**
     * @brief Insert operation of a value, it calls to another insert
     * function to store the value to a specific node and returns is
     * a overflow occurs, if that true calls to split
     *
     * @param value
     */
    void insert(const T value, const long record_id = -1){
        node root = readNode(header.disk_id);
        int state = insert(root, value, record_id);
        if (state == BP_OVERFLOW) {
            splitRoot();
        }
    }


    /**
     * @brief Print the tree values to the console
     * 
     */
    void print_tree() {
        node root = readNode(header.disk_id);
        showTree(root, 0);
        std::cout << "________________________\n";
    }

    /**
     * @brief Print the tree values to the console by a tree level
     * @param ptr_node ndoe to be printed
     * @param tree_level tree level
     */
    void showTree(node &ptr_node, int tree_level) {
        int i;
        for (i = ptr_node.n_keys - 1; i >= 0; i--) {
            if (ptr_node.children[i + 1]) { //right child
                node child = readNode(ptr_node.children[i + 1]);
                showTree(child, tree_level + 1);
            }
            for (int k = 0; k < tree_level; k++) {
                std::cout << "    ";
            }
            std::cout << ptr_node.keys[i] << "\n";
        }
        if (ptr_node.children[i + 1]) {//left child
            node child = readNode(ptr_node.children[i + 1]);
            showTree(child, tree_level + 1);
        }
    }
    /**
    * @brief Print just the leaf node values to the console, it is used just for testing
    *
    * @param out
    */
    void print(std::ostream& out) {
        node root = readNode(header.disk_id);
        print(root, 0, out);
    }

    /**
     * @brief Print just the leaf node values to the console
     * by a given tree level, it is used just for testing
     *
     * @param out
     */
    void print(node &ptr_node, int tree_level, std::ostream& out) {
        int i;
        for (i = 0; i < ptr_node.n_keys; i++) {
            if (ptr_node.children[i]) {
                node child = readNode(ptr_node.children[i]);
                print(child, tree_level + 1, out);
            }
            if (ptr_node.is_leaf)
                out << ptr_node.keys[i];
        }
        if (ptr_node.children[i]) {
            node child = readNode(ptr_node.children[i]);
            print(child, tree_level + 1, out);
        }
    }


    /**
     * @brief Returns an iterator with the first  leaf node
     * 
     * @return iterator 
     */
    iterator begin(){
        node temp = readNode(header.disk_id);
        while (!temp.is_leaf)
            temp = readNode(temp.children[0]);
        iterator my_iter (disk_manager, temp.disk_id);
        return my_iter;
    }

    /**
     * @brief Returns an iterator with the last leaf node
     * 
     * @return iterator 
     */
    iterator end(){
        node temp = readNode(header.disk_id);
        while (!temp.is_leaf){
            temp = readNode(temp.children[temp.n_keys]);
        }
        iterator my_iter (disk_manager, temp.disk_id, temp.n_keys - 1);
        return my_iter;
    }

    /**
     * @brief Create a null iterator to check if we exceed the last or first node
     * 
     * @return iterator 
     */
    iterator null(){
        iterator my_iter (disk_manager, -1);
        return my_iter;
    }

    ~BPlusTree(){
    }

    bool isKeyPresent(const T &val){
        node root = readNode(header.disk_id);
        int key_pos = -1;
        long key_disk_id = findKey(root, val, key_pos);
        if (key_disk_id == -1)
            return false;
        else
            return true;
    }

    long getRecordIdByKeyValue(const T &val, int &disk_access){
        node root = readNode(header.disk_id);
        disk_access++;
        int key_pos = -1;
        long key_disk_id = findKey(root, val, key_pos, disk_access);
        if (key_disk_id != -1){
            node key_node = readNode(key_disk_id);
            disk_access++;
            return key_node.records_id[key_pos];
        }
        return key_disk_id; //return -1
    }

    void find(const T &val, long &record_id ,int &key_pos){
        node root = readNode(header.disk_id);
        record_id = findKey(root, val, key_pos);
    }

    long findKey (node &ptr, const T &val, int &key_pos, int &disk_access){
        int pos = 0;
        while (pos < ptr.n_keys && ptr.keys[pos] < val)
            pos++;

        if (!ptr.is_leaf){
            long page_id = ptr.children [pos];
            node child = readNode (page_id);
            disk_access++;
            return findKey(child, val, key_pos, disk_access);
        } else {
            if (ptr.keys [pos] != val)
                return -1;
            else {
                key_pos = pos;
                return ptr.disk_id;
            }
        }

    }

    long findKey (node &ptr, const T &val, int &key_pos){
        int pos = 0;
        while (pos < ptr.n_keys && ptr.keys[pos] < val)
            pos++;

        if (!ptr.is_leaf){
            long page_id = ptr.children [pos];
            node child = readNode (page_id);
            return findKey(child, val, key_pos);
        } else {
            if (ptr.keys [pos] != val)
                return -1;
            else {
                key_pos = pos;
                return ptr.disk_id;
            }
        }

    }

/**
     * @brief
     *
     * @param val
     */
    void search (const T &val) {
        node root = readNode(header.disk_id);
        int res = search (root, val);
        if (res == -1)
            std::cout << "Not found\n";
        else
            std::cout << "Found!\n";

    }

    /**
     * @brief
     *
     * @param ptr
     * @param val
     * @return int
     */
    int search (node &ptr, const T &val){
        int pos = 0;
        while (pos < ptr.n_keys && ptr.keys[pos] < val)
            pos++;

        if (!ptr.is_leaf){
            long page_id = ptr.children [pos];
            node child = readNode (page_id);
            return search (child, val);
        } else {
            if (ptr.keys [pos] != val){
                return -1;
            }else {
                long page_record = ptr.children [pos];
                return page_record;
            }
        }

    }

    /**
     * @brief
     *
     * @param first
     * @param end
     * @return std::vector<long>
     */
    std::vector<long> range_search (const T &first, const T &end){
        node root = readNode(header.disk_id);
        std::vector <long> res;
        range_search (root, first, end, res);
        return res;
    }


    /**
     * @brief
     *
     * @param ptr
     * @param first
     * @param second
     * @param res
     */
    void range_search (node &ptr, const T &first, const T &second, std::vector <long> &res){
        int pos = 0;
        while (pos < ptr.n_keys && ptr.keys[pos] < first)
            pos++;

        if (!ptr.is_leaf){
            long page_id = ptr.children [pos];
            node child = readNode (page_id);
            range_search (child, first, second, res);
        } else {
            if (ptr.keys [pos] >= first){
                std::cout << ptr.keys [pos] << "." << ptr.disk_id << " - ";
                //res.push_back (ptr.children [pos]);
                res.push_back (ptr.records_id[pos]);
            }
            pos++;
            node temp = ptr;
            while (temp.keys [pos] <= second && pos < temp.n_keys){
                std::cout << temp.keys [pos] << "." << temp.disk_id << " - ";
                //res.push_back (temp.children [pos]);
                res.push_back (temp.records_id[pos]);
                pos++;
                if (pos == temp.n_keys ){
                    temp = readNode (temp.next_node);
                    pos = 0;
                }
            }
        }
    }



};

/**
 * @brief B Plus Tree Iterator Object
 * 
 * @tparam T type of the index
 * @tparam ORDER order of the btree
 */
template <class T, int ORDER>
class BPlusTreeIterator{

    using node = BPlusTreeNode<T, ORDER>;
    using diskManager = std::shared_ptr<pagemanager>;
    
    long node_disk_id; //node id on disk
    int keys_pos; //iterator for keys elements
    
    diskManager disk_manager;

    /**
     * @brief Read a node from disk by a given disk id position
     * 
     * @param disk_id position on disk to be read
     * @return node node with the read values 
     */
    node readNode(long disk_id){
        node new_node(-1);
        disk_manager->recover(disk_id, new_node);
        return new_node;
    }

public:

    friend class BPlusTree<T, ORDER>;

    /**
     * @brief Construct a new BPlusTreeIterator object
     * 
     * @param manager disk manager of the btree
     * @param ndi disk id of the node
     */
    BPlusTreeIterator(diskManager &manager, long ndi){
        disk_manager = manager;
        node_disk_id = ndi;
        keys_pos = 0;
    }

    /**
     * @brief Construct a new BPlusTreeIterator object
     * 
     * @param manager disk manager of the btree
     * @param ndi disk id of the node
     * @param _keys_pos position on key to start the iterator
     */
    BPlusTreeIterator(diskManager &manager, long ndi, int _keys_pos){
        disk_manager = manager;
        node_disk_id = ndi;
        keys_pos = _keys_pos;
    }

    /**
     * @brief Construct a new BPlusTreeIterator object by and other iterator
     * 
     * @param bpti other B+Tree iterator
     */
    BPlusTreeIterator(const BPlusTreeIterator & bpti){
        node_disk_id = bpti.node_disk_id;
        disk_manager = bpti.disk_manager;
        keys_pos = bpti.keys_pos;
    }

    /**
     * @brief Prefix ++ operator
     * Increase in one the value of keys_pos, if we reach the end of a 
     * node, we go to the next node
     * 
     * @return BPlusTreeIterator& 
     */
    BPlusTreeIterator& operator++(){
        keys_pos++;
        node temp = readNode(node_disk_id);
        if (keys_pos >= temp.n_keys){    //if we reach the end of the keys, go to the next node
            node_disk_id = temp.next_node;
            keys_pos = 0;
        }
        return *this;
    }

    /**
     * @brief Postfix ++ operator
     * Increase in one the value of keys_pos, if we reach the end of a 
     * node, we go to the next node
     * 
     * @return BPlusTreeIterator& 
     */
    BPlusTreeIterator operator++(int){
        BPlusTreeIterator tmp (*this);
        operator++();
        return tmp;
    }

    /**
     * @brief Prefix -- operator
     * Decrease in one the value of keys_pos, if we reach the start -1 position 
     * of a node, we go to the previous node
     * 
     * @return BPlusTreeIterator& 
     */
    BPlusTreeIterator& operator--(){
        keys_pos--;
        node temp = readNode(node_disk_id);
        if (keys_pos < 0){    //if we reach the end of the keys, go to the next node
            node_disk_id = temp.prev_node;
            if (node_disk_id != -1){
                node prev = readNode(node_disk_id);
                keys_pos = prev.n_keys -1;
            }else{
                keys_pos = 0;
            }

        }
        return *this;
    }

    /**
     * @brief Postfix -- operator
     * Decrease in one the value of keys_pos, if we reach the start -1 position 
     * of a node, we go to the previous node
     * 
     * @return BPlusTreeIterator& 
     */
    BPlusTreeIterator operator--(int){
        BPlusTreeIterator tmp (*this);
        operator--();
        return tmp;
    }

    /**
     * @brief Assing the value of one iterator to another
     * 
     * @param bpti 
     * @return BPlusTreeIterator& 
     */
    BPlusTreeIterator& operator=(const BPlusTreeIterator& bpti){
        keys_pos = bpti.keys_pos;
        node_disk_id = bpti.node_disk_id;
        disk_manager = bpti.disk_manager;
    }

    /**
     * @brief Check if the two iterators are equal
     * 
     * @param bpti another iterator
     * @return true are equal
     * @return false are different
     */
    bool operator==(const BPlusTreeIterator& bpti){
        bool nodes_equal = (node_disk_id == bpti.node_disk_id);
        bool keys_pos_equal = (keys_pos == bpti.keys_pos);
        return nodes_equal && keys_pos_equal;
    }

    /**
     * @brief Check is the two iterators are different
     * 
     * @param bpti another iterator
     * @return true are different
     * @return false are equal
     */
    bool operator!=(const BPlusTreeIterator& bpti){
        return !((*this)==bpti);
    }

    /**
     * @brief Dereference operator, return the value of the key in the 
     * current keys_pos position
     * 
     * @return T key value
     */
    T operator*(){
        node temp = readNode(node_disk_id);
        return temp.keys[keys_pos];
    }

    long getRecordId(){
        node temp = readNode(node_disk_id);
        return temp.records_id[keys_pos];
    }

};