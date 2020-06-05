#ifndef INDEX_H_
#define INDEX_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <queue>

using namespace std;

class Node
{
protected:
    //isLeaf: consider node is Leafnode, keys: key's vector
    bool isLeaf;
    vector<int> keys;

public:
    bool Get_IsLeaf();
    vector<int> Get_keys();
    //below is virtual function
    virtual void Insert(int key, int value){};
    virtual void Insert(int key, Node *rightchild){};
    virtual void Insert(int key, Node *leftchild, Node *rightchild){};
    virtual Node *Split(int *keytoparent){};
    virtual vector<Node *> Get_children(){};
    virtual vector<int> Get_values(){};
    virtual Node *Get_next(){};
};

class LeafNode : public Node
{
private:
    //leafnode have pre: pre leafnode, next: leafnode, values: leafnode's values
    LeafNode *pre;
    LeafNode *next;
    vector<int> values;
public:
    //constructor
    LeafNode();
    //insert key
    void Insert(int key, int value);
    //split the leafnode
    Node *Split(int *keytoparent);
    vector<int> Get_values();
    Node* Get_next();
};

class InternalNode : public Node
{
private:
    vector<Node *> children;

public:
    InternalNode();
    //insert key and rightchild to internalnode
    void Insert(int key, Node *rightchild);
    //insert the internalnode to the root
    void Insert(int key, Node *leftchild, Node *rightchild);
    Node *Split(int *keytoparent);
    vector<Node *> Get_children();
};

class Index
{
private:
    int order;
    Node *root;
    //search the key from root to the leafnode
    void Search_Path(Node *node, int key, stack<Node *> *path);
    void Destroy(Node *node);

public:
    Index(int num_rows, vector<int> key, vector<int> value);
    void Initialize(int m);
    void Insert(int key, int value);
    void key_query(vector<int> query_keys);
    void range_query(vector<pair<int, int> > query_pairs);
    void clear_index();
};

#endif
