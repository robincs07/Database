#ifndef _Index_
#define _Index_

#include <vector>
#include <iostream>
#include <sstream>
#include <climits>
#include <fstream>

using namespace std;
#define MAX 60

class Index;
class Node
{
    bool isLeaf;
    int *key, size;
    int *value;
    Node **children;
    Node *prev;
    Node *next;
    friend class Index;

public:
    //constructor
    Node();
};
class Index
{
    Node *root;
    void insertInternal(int, Node *, Node *);
    Node *findParent(Node *, Node *);

public:
    Index();
    Index(int num_rows, vector<int> key, vector<int> val);
    void insert(int k, int v);
    void key_query(vector<int> query_keys);
    void range_query(vector<pair<int, int>> query_pairs);
    void Destroy(Node *node);
    void clear_index();
};
#endif