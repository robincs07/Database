#include "index.h"

Node::Node()
{
    key = new int[MAX];
    children = new Node *[MAX + 1];
    value = new int[MAX];
    prev = this;
    next = this;
    for(int i=0; i<MAX; i++)
    {
        key[i]=INT_MIN;
        children[i]=NULL;
        value[i]=INT_MIN;
    }
    children[MAX]=NULL;
    size=0;
}
Index::Index()
{
    root = NULL;
}

Index::Index(int num_rows, vector<int> key, vector<int> value)
{
    root = NULL;
    for (int i = 0; i < num_rows; i++)
    {
        insert(key[i], value[i]);
    }
}

void Index::insert(int k, int v)
{
    //if there is no root
    if (root == NULL)
    {
        root = new Node;
        root->key[0] = k;
        root->value[0] = v;
        root->isLeaf = true;
        root->size = 1;
        for(int i=0; i<MAX+1; i++)
        {
            root->children[i]=NULL;
        }
    }
    //have root
    else
    {
        Node *current = root;
        Node *parent = NULL;
        //if current is not leaf，then find the leaf node
        while (current->isLeaf == false)
        {
            parent = current;
            for (int i = 0; i < current->size; i++)
            {
                if (k < current->key[i])
                {
                    current = current->children[i];
                    break;
                }
                if (i == current->size - 1)
                {
                    current = current->children[i + 1];
                    break;
                }
            }
        }
        //if leaf node is not full
        if (current->size < MAX)
        {
            int i = 0;
            while (k > current->key[i] && i < current->size)
                i++;
            for (int j = current->size; j > i; j--)
            {
                current->key[j] = current->key[j - 1];
                current->value[j] = current->value[j - 1];
            }
            current->key[i] = k;
            current->value[i] = v;
            current->size++;
        }
        //leaf node is full need split
        else
        {
            //for split
            Node *newLeaf = new Node;
            //rightnode
            int rightnodekey[MAX + 1];
            int rightnodevalue[MAX + 1];
            //right node is same as current
            for (int i = 0; i < MAX; i++)
            {
                rightnodekey[i] = current->key[i];
                rightnodevalue[i] = current->value[i];
            }
            int i = 0;
            while (k > rightnodekey[i] && i < MAX)
                i++;
            for (int j = MAX; j > i; j--)
            {
                rightnodekey[j] = rightnodekey[j - 1];
                rightnodevalue[j] = rightnodevalue[j - 1];
            }
            rightnodekey[i] = k;
            rightnodevalue[i] = v;
            newLeaf->isLeaf = true;
            //current size/2
            current->size = (MAX + 1) / 2;
            //new leaf is half of current
            newLeaf->size = MAX + 1 - (MAX + 1) / 2;
            newLeaf->next = current->next;
            newLeaf->prev = current;
            current->next = newLeaf;

            for (i = 0; i < current->size; i++)
            {
                current->key[i] = rightnodekey[i];
                current->value[i] = rightnodevalue[i];
            }
            int p = 0;
            for (int l = current->size; l < newLeaf->size + current->size; l++)
            {
                newLeaf->key[p] = rightnodekey[l];
                newLeaf->value[p] = rightnodevalue[l];
                p++;
            }
            for(int i=0; i<MAX+1; i++)
            {
                newLeaf->children[i]=NULL;
            }
            if (current == root)
            {

                Node *newRoot = new Node;
                newRoot->key[0] = newLeaf->key[0];
                newRoot->value[0] = newLeaf->value[0];
                newRoot->children[0] = current;
                newRoot->children[1] = newLeaf;
                newRoot->isLeaf = false;
                newRoot->size = 1;
                root = newRoot;
            }
            else
            {

                insertInternal(newLeaf->key[0], parent, newLeaf);
            }
        }
    }
}

void Index::insertInternal(int k, Node *current, Node *child)
{
    //internalnode is not full
    if (current->size < MAX)
    {
        int i = 0;
        while (k > current->key[i] && i < current->size)
            i++;
        for (int j = current->size; j > i; j--)
        {
            current->key[j] = current->key[j - 1];
        }
        for (int j = current->size + 1; j > i + 1; j--)
        {
            current->children[j] = current->children[j - 1];
        }
        current->key[i] = k;
        current->size++;
        current->children[i + 1] = child;
    }

    else
    {
        Node *newInternal = new Node;
        int virtualKey[MAX + 1];
        Node *virtualchildren[MAX + 2];
        //cout << cursor->key[0];
        for (int i = 0; i < MAX; i++)
        {
            //cout << "cursor->key[i]=" << cursor->key[i] << endl;
            virtualKey[i] = current->key[i];
            virtualchildren[i] = current->children[i];
        }
        virtualchildren[MAX] = current->children[MAX];

        int i = 0, j;
        while (k > virtualKey[i] && i < MAX) //x= new_leaf_key[0]
            i++;
        for (int j = MAX; j > i; j--)
        {
            virtualKey[j] = virtualKey[j - 1];
        }
        virtualKey[i] = k;
        for (int j = MAX + 1; j > i + 1; j--)
        {
            virtualchildren[j] = virtualchildren[j - 1];
        }
        virtualchildren[i + 1] = child;
        newInternal->isLeaf = false;
        current->size = (MAX + 1) / 2;
        newInternal->size = MAX - (MAX + 1) / 2;
        for (i = 0; i < current->size + 1; i++)
        {
            current->key[i] = virtualKey[i];
        }
        for (i = 0; i < current->size + 1; i++)
        {
            current->children[i] = virtualchildren[i];
        }
        int hh = current->size + 1;
        for (int j = 0; j < newInternal->size; j++)
        {
            newInternal->key[j] = virtualKey[hh];
            hh++;
        }
        int h = current->size + 1;
        for (int j = 0; j < newInternal->size + 1; j++)
        {
            newInternal->children[j] = virtualchildren[h];
            h++;
        }
        if (current == root)
        {
            Node *newRoot = new Node;
            newRoot->key[0] = virtualKey[current->size];
            newRoot->children[0] = current;
            newRoot->children[1] = newInternal;
            newRoot->isLeaf = false;
            newRoot->size = 1;
            root = newRoot;
        }
        else
        {
            insertInternal(current->key[current->size], findParent(root, current), newInternal);
        }
    }
}

Node *Index::findParent(Node *current, Node *child)
{
    Node *parent = NULL;
    if (current->isLeaf || (current->children[0])->isLeaf)
    {
        return NULL;
    }
    for (int i = 0; i < current->size + 1; i++)
    {
        if (current->children[i] == child)
        {
            parent = current;
            return parent;
        }
        else
        {
            parent = findParent(current->children[i], child);
            if (parent != NULL)
                return parent;
        }
    }
    return parent;
}

void Index::key_query(vector<int> query_keys)
{

    ofstream outputfile;
    outputfile.open("key_query_out.txt", ios::out | ios::trunc);
    for (int h = 0; h < query_keys.size(); h++)
    {
        bool flag = 0;
        Node *current = root;
        while (current->isLeaf == false)
        {
            for (int i = 0; i < current->size; i++)
            {
                if (query_keys[h] < current->key[i])
                {
                    current = current->children[i];
                    break;
                }
                if (i == current->size - 1)
                {
                    current = current->children[i + 1];
                    break;
                }
            }
        }
        for (int i = 0; i < current->size; i++)
        {
            if (current->key[i] == query_keys[h])
            {
                outputfile << current->value[i] << endl;
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            outputfile << "-1" << endl;
        }
    }
    outputfile.close();
}

void Index::range_query(vector<pair<int, int>> query_pairs)
{
    ofstream outputfile;
    outputfile.open("range_query_out.txt", ios::out | ios::trunc);
    for (vector<pair<int, int>>::iterator idx = query_pairs.begin(); idx != query_pairs.end(); idx++)
    {
        int key1 = idx->first;
        int key2 = idx->second;
        Node *current = root;
        int max=-1;

        while (current->isLeaf == false)
        {
            for (int i = 0; i < current->size; i++)
            {
                if (key1 < current->key[i])
                {
                    current = current->children[i];
                    break;
                }
                if (i == current->size - 1)
                {
                    current = current->children[i + 1];
                    break;
                }
            }
        }
        //cout << "found leaf" << endl;
        int index=-1;
        for (int i = 0; i < current->size; i++)
        {
            if (key1 <= current->key[i])
            {
                index = i;
                break;
            }

        }
        if(index==-1)
        {
            index=0;
            current=current->next;

        }
        while(current->key[index]<=key2)
        {
            if(current->value[index]>max)
            {
                max=current->value[index];
            }
            index++;
            if(index==current->size)
            {
                index=0;
                current=current->next;
            }
        }   
        outputfile << max << endl;
    }
    outputfile.close();
}

void Index::Destroy(Node *node)
{
    //if node is the internalnode
    if (!(node->isLeaf))
    {
        //for next level node delete it
        for (int i = 0; i < node->size + 1; i++)
        {
            Destroy(node->children[i]);
        }
    }
    //delete node
    delete [](node->key);
    delete [](node->value);
    delete [](node->children);
    delete (node);
}

void Index::clear_index()
{
    Destroy(root);
}