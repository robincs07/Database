#include "index.h"

using namespace std;

bool Node::Get_IsLeaf()
{
    return isLeaf;
}
vector<int> Node::Get_keys()
{
    return keys;
}

LeafNode ::LeafNode()
{
    isLeaf = true;
    pre = this;
    next = this;
}
//insert Leafnode key
void LeafNode::Insert(int key, int value)
{
    //set a iterator point to the key that is greater or equal
    vector<int>::iterator idx = lower_bound(keys.begin(), keys.end(), key);

    //insert new key
    keys.insert(idx, key);
    //idx point to the new key
    idx = lower_bound(keys.begin(), keys.end(), key);
    //insert key new value in the value matrix
    values.insert(values.begin() + (idx - keys.begin()), value);
}

Node *LeafNode::Split(int *keytoparent)
{
    //rightnode for the new leafnode
    LeafNode *rightnode = new LeafNode;

    //key to put to the internalnode is the middle of the key
    *keytoparent = keys[keys.size() / 2];

    //copy the right half keys and values to the rightnode
    rightnode->keys.assign(keys.begin() + keys.size() / 2, keys.end());
    rightnode->values.assign(values.begin() + values.size() / 2, values.end());

    //erase the leftnode's right half key and value
    keys.erase(keys.begin() + keys.size() / 2, keys.end());
    values.erase(values.begin() + values.size() / 2, values.end());

    //rightnode's next point to the next, pre point to the leftnode
    rightnode->next = next;
    rightnode->pre = this;
    //leftnode's next point to the rightnode
    next = rightnode;
    //rightnode's next leafnode's pre point to the rightnode
    (rightnode->next)->pre = rightnode;

    return rightnode;
}
vector<int> LeafNode::Get_values()
{
    return values;
}
Node *LeafNode::Get_next()
{
    return next;
}

InternalNode::InternalNode()
{
    isLeaf = false;
}
//insert key and rightchild to internalnode
void InternalNode::Insert(int key, Node *rightchild)
{
    //idx point to the key, and insert key
    vector<int>::iterator idx = lower_bound(keys.begin(), keys.end(), key);
    keys.insert(idx, key);

    //reidx the key
    idx = lower_bound(keys.begin(), keys.end(), key);

    //insert rightnode to the children, and must +1
    children.insert(children.begin() + (idx - keys.begin() + 1), rightchild);
}

//insert the internalnode to the root
void InternalNode::Insert(int key, Node *leftchild, Node *rightchild)
{
    //new root node
    keys.push_back(key);
    children.push_back(leftchild);
    children.push_back(rightchild);
}

Node *InternalNode::Split(int *keytoparent)
{
    //length equal to the node's key size
    int length = keys.size();

    //new a internalnode
    InternalNode *rightnode = new InternalNode;

    //keytoparent must be the middle of the key
    *keytoparent = keys[length / 2];

    //copy the length/2+1 to the rightnode
    rightnode->keys.assign(keys.begin() + (length / 2 + 1), keys.end());
    rightnode->children.assign(children.begin() + (length / 2 + 1), children.end());

    //erase the leftnode right half remember that we do not need to have the keytoparent
    //internalnode do not have the same key
    keys.erase(keys.begin() + length / 2, keys.end());
    children.erase(children.begin() + (length / 2 + 1), children.end());

    return rightnode;
}

vector<Node *> InternalNode::Get_children()
{
    return children;
}

Index::Index(int num_rows, vector<int> key, vector<int> value)
{
    order = 50;
    root = NULL;
    for (int i = 0; i < num_rows; i++)
    {
        //cout<<key[i]<<" "<<value[i]<<" "<<i<<endl;
        Insert(key[i], value[i]);
    }
}

void Index::Search_Path(Node *node, int key, stack<Node *> *path)
{
    //push first node to the stack
    path->push(node);

    //if the node is a internalnode
    if (!node->Get_IsLeaf())
    {
        //copy the keys and children of the internalnode
        vector<int> keys = node->Get_keys();
        vector<Node *> children = node->Get_children();

        //idx point to the key that is greater or equal to the key
        vector<int>::iterator idx = lower_bound(keys.begin(), keys.end(), key);

        //find the key, go to the rightnode
        if (key == keys[idx - keys.begin()])
        {
            Search_Path(children[(idx - keys.begin()) + 1], key, path);
        }
        //smaller than the key go to leftnode
        else
        {
            Search_Path(children[idx - keys.begin()], key, path);
        }
    }
}

void Index::Destroy(Node *node)
{
    //if node is the internalnode
    if (!node->Get_IsLeaf())
    {
        vector<Node *> children = node->Get_children();
        //for next level node delete it
        for (vector<Node *>::iterator idx = children.begin(); idx != children.end(); idx++)
        {
            Destroy(*idx);
        }
    }

    //delete node
    delete (node);
}

void Index::Insert(int key, int value)
{
    //if root is NULL, new a leafnode and insert key
    if (root == NULL)
    {
        root = new LeafNode;
        root->Insert(key, value);
    }

    //root have node
    else
    {
        //new leftnode, rightnode, rootpopped for the no root
        Node *leftnode = NULL;
        Node *rightnode = NULL;
        int *keytoparent = new int;
        bool rootpopped = false;

        //search key in the tree
        stack<Node *> *path = new stack<Node *>;
        Search_Path(root, key, path);

        //insert the key in the leafnode
        path->top()->Insert(key, value);

        //if the keys in the leafnode is over order-1
        //from the leafnode to the rootnode
        while (path->top()->Get_keys().size() == order)
        {
            //leftnode =leafnode, rightnode is split from the leftnode
            leftnode = path->top();
            rightnode = leftnode->Split(keytoparent);
            //pop the leftnode
            path->pop();

            //if  path is not empty then insert the rightnode
            if (!path->empty())
            {
                //top now is the internalnode and is up level
                //so insert the rightnode in the internalnode
                path->top()->Insert(*keytoparent, rightnode);
            }
            //all the node be popped
            else
            {
                rootpopped = true;
                break;
            }
        }
        //if root popped
        if (rootpopped)
        {
            //internalnode for the new root
            InternalNode *newroot = new InternalNode;
            //new root, so insert the leftnode and rightnode
            //assign newroot be the root
            newroot->Insert(*keytoparent, leftnode, rightnode);
            root = newroot;
        }

        //delete keytoparent and path
        delete (keytoparent);
        delete (path);
    }
}

void Index::key_query(vector<int> query_keys)
{
    ofstream outputfile;
    outputfile.open("key_query_out.txt", ios::out | ios::trunc);
    for (vector<int>::iterator index = query_keys.begin(); index != query_keys.end(); index++)
    {
        //search key
        int key = query_keys[index - query_keys.begin()];
        stack<Node *> *path = new stack<Node *>;
        Search_Path(root, key, path);
        int i = 0;

        //copy keys and values, idx point to the key
        vector<int> keys = path->top()->Get_keys();
        vector<int> values = path->top()->Get_values();
        vector<int>::iterator idx = lower_bound(keys.begin(), keys.end(), key);

        //if found key
        if (key == keys[idx - keys.begin()])
        {
            outputfile << values[idx - keys.begin()] << endl;
        }
        //not found key
        else
        {
            outputfile << -1 << endl;
        }
        delete (path);
    }
    outputfile.close();
}

void Index::range_query(vector<pair<int, int>> query_pairs)
{
    ofstream outputfile;
    outputfile.open("range_query_out.txt", ios::out | ios::trunc);
    //pass: have the ans yet, firstkey: for first key found
    for (vector<pair<int, int>>::iterator index = query_pairs.begin(); index != query_pairs.end(); index++)
    {
        priority_queue<int> q;
        int key1 = index->first;
        int key2 = index->second;
        bool pass = true;
        int firstkey;
        //search for key1
        stack<Node *> *path = new stack<Node *>;
        Search_Path(root, key1, path);

        //copy the keys and value and next, idx point to the key1
        vector<int> keys = path->top()->Get_keys();
        vector<int> values = path->top()->Get_values();
        Node *next = path->top()->Get_next();
        vector<int>::iterator idx = lower_bound(keys.begin(), keys.end(), key1);

        //for finding the value in the range
        while (1)
        {
            int i = 0;

            //reach the end of the leafnode
            if ((idx - keys.begin()) == keys.size())
            {
                keys = next->Get_keys();
                values = next->Get_values();
                next = next->Get_next();
                idx = keys.begin();
            }

            //don't have any value, add first key
            if (pass)
            {
                firstkey = keys[idx - keys.begin()];
            }

            //if have value and idx not point to the firstkey and break
            if (!(pass || (keys[idx - keys.begin()] != firstkey)))
            {
                break;
            }

            //key in the range
            if ((key1 <= keys[idx - keys.begin()]) && (keys[idx - keys.begin()] <= key2))
            {
                q.push(values[idx - keys.begin()]);
            }
            //out the range
            else
            {
                //no value
                if (pass)
                {
                    outputfile << "-1" << endl;
                }

                break;
            }

            pass = false;
            idx++;
        }
        outputfile << q.top() << endl;
    }
    outputfile.close();
}

void Index::clear_index()
{
    Destroy(root);
}