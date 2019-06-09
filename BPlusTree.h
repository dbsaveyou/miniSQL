#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <string>
using namespace std;

template <typename KeyType>
class TreeNode
{
private:
    int degree;
public:
    vector <KeyType> keys;
    vector <TreeNode*> children;
    vector <int> vals;
    int count;
    bool isLeaf;
    TreeNode* Parent;
    TreeNode* NextLeaf;
    TreeNode(int degree, bool isLeaf = false);
    ~TreeNode();
    TreeNode* splite();
    bool search(KeyType key, int &index);
    bool insert(KeyType key);                           //insert on the branch
    bool insert(KeyType key, int val);            //insert on the leaf
    bool remove(KeyType key);
    TreeNode* splite(KeyType& key);
    bool reset_keys(KeyType key, int index);
    bool reset_vals(int val, int index);
    bool reset_children(TreeNode* newchild, int index);
};


/* BplusTree */
template <typename KeyType>
class BPlusTree
{
    typedef TreeNode<KeyType>* Node;
    struct AimNode{
        Node ANode;
        int index;
    };
public:
    size_t KeyNum;
    size_t height;
    size_t NodeNum;
    int KeySize;
    int degree;
    string filePath;
    Node root;
    Node LeafHead; // the head of the leaf node
    BPlusTree(int degree, int KeySize, string filePath);
    ~BPlusTree();
    
    void ReadDiskNode(blockNode* btmp);
    void ReadDiskAll();
    void WriteDisk();

    
    void init_tree();
    bool search(Node TestNode, KeyType key, AimNode& AN);           // search the value of specific key
    bool find(Node TestNode, KeyType key, AimNode& AN);         
    bool insert(KeyType key,int val);
    bool remove(KeyType key);
    bool AdjustAfterInsert(Node TestNode);
    bool AdjustAfterDelete(Node TestNode);
    void dropTree(Node Testnode);
    bool judge(Node TestNode);
};



template <typename KeyType>
TreeNode < KeyType>::TreeNode(int degree, bool isLeaf): count(0), Parent(NULL), NextLeaf(NULL), degree(degree), isLeaf(isLeaf) 
{
    for (int i=0; i < degree; i++)
    {
        children.push_back(NULL);
        keys.emplace_back(KeyType());
        vals.emplace_back(int());
    }
    children.push_back(NULL);
}

template <typename KeyType>
TreeNode <KeyType>::~TreeNode(){}


template <typename KeyType>
bool TreeNode<KeyType>::search(KeyType key, int& index) 
{
    if (count == 0)
        return false;
    if (key == 0)
    {
        index = count-1;
        return true;
    }
    else if (keys[count-1]<key)
    {
        index=count;
        return false;
    }
    else if (keys[0]>key)
    {
        index = 0;
        return false;
    }
    else //binary search
    {
        int left = 0, pos = 0, right = count - 1;
        while (right>left+1)
        {
            pos = (right + left) / 2;
            if(keys[pos] == key)
            {
                index = pos;
                return true;
            }
            else if(keys[pos] < key)
                left = pos;
            else if(keys[pos] > key)
                right = pos;
        }    
        if(keys[left] >= key)
        {
            index = left;
            return (keys[left] == key);
        }
        else if(keys[right] >= key)
        {
            index = right;
            return (keys[right] == key);
        }
        else if(keys[right] < key)
        {
            index = right++;
            return false;
        }
    }

    return false;
}


template <typename KeyType>
bool TreeNode<KeyType>::insert(KeyType key, int val)        //just push the key and val into leaf and without considering 
{
    if (!this->isLeaf)
    {
        cout << "Error:This is not a leaf" << endl;
        exit(3);   
    }

    if (count == 0)
    {
        this->keys[0] = key;
        this->vals[0] = val;
        this->count = 1;
    }
    else 
    {
        int index = 0;
        if (this->search(key, index))
        {
            cout << "Error:In add(Keytype &key),key has already in the tree!" << endl;
            exit(3);
        }
        else
        {
            for (int i = count; i > index; i--)
            {
                this->keys[i] = this->keys[i-1];
                this->vals[i] = this->vals[i-1];
            }
            this->keys[index] = key;
            this->vals[index] = val;
            this->count++;
        }
    }
    return true;
}


template <typename KeyType>
bool TreeNode<KeyType>::insert(KeyType key)                
{
    if (count == 0)
    {
        this->keys[0] = key;
        this->count++;
    }
    else
    {
        int index = 0; 
        if (search(key, index))
        {
            cout << "Error:In insert(Keytype &key),key has already in the tree!" << endl;
            exit(3);
        }
        else // add the key into the node
        {
            for(int i = count;i > index;i --)
                this->keys[i] = this->keys[i-1];
            this->keys[index] = key;
            
            for(int i = count + 1;i > index+1;i --)
                this->children[i] = this->children[i-1];
            this->children[index+1] = NULL; // this child will link to another node
            
            this->count++;
        }
    }

    return true;
}


template <typename KeyType>
bool TreeNode<KeyType>::remove(KeyType key)
{
    int index = 0;
    if (!search(key, index))
    {
        cout << "Error:In reomve(Keytype &key),key has not in the tree!" << endl;
        return false;
    }
    else if (isLeaf)
    {
        for (int i = index; i < count-1; i++)
        {
            keys[i] = keys[i+1];
            vals[i] = vals[i+1];
        }
        keys[count-1] = KeyType();
        vals[count-1] = int();
        count--;
    }
    else
    {
        for (int i = index; i < count-1; i++)
            keys[i] = keys[i+1];
        keys[count-1] = KeyType();

        for (int i = index+1; i < count; i++)
            children[i] = children[i+1];
        children[count] = NULL;
        count--;
    }

    return true;
}


template <typename KeyType>
TreeNode<KeyType>* TreeNode<KeyType>::splite(KeyType& key)
{
    int miniumsize = (degree-1) / 2;
    TreeNode* newNode = new TreeNode(degree, isLeaf);
    if (isLeaf)
    {
        key = keys[miniumsize+1];
        for (int i = miniumsize+1; i < degree; i++)
        {
            newNode->keys[i-miniumsize-1] = this->keys[i];
            newNode->vals[i-miniumsize-1] = this->vals[i];
            this->vals[i] = int();
            this->keys[i] = KeyType();
        }
        newNode->NextLeaf = this->NextLeaf;
        this->NextLeaf = newNode;
        newNode->Parent = this->Parent;
        this->count = miniumsize+1;
        newNode->count = miniumsize;
    }
    else
    {
        key = keys[miniumsize];
        for (int i = miniumsize+1; i < degree; i++)
        {
            newNode->keys[i-miniumsize-1] = this->keys[i];
            newNode->children[i-miniumsize-1] = this->children[i];
            newNode->children[i-miniumsize-1]->Parent = newNode;
            this->children[i] = NULL;
            this->keys[i] = KeyType();
        }
        this->keys[miniumsize] = KeyType();
        newNode->children[degree-miniumsize-1] = this->children[degree];
        this->children[degree] = NULL;
        newNode->children[degree-miniumsize-1]->Parent = newNode;

        newNode->Parent = this->Parent;
        this->count = miniumsize;
        newNode->count = miniumsize;
    }
    
    return newNode;
}


template <typename KeyType>
bool TreeNode<KeyType>::reset_children(TreeNode* newchild, int index)
{
    children[index] = newchild;
}


template <typename KeyType>
bool TreeNode<KeyType>::reset_keys(KeyType key, int index)
{
    keys[index] = key;
}


template <typename KeyType>
bool TreeNode<KeyType>::reset_vals(int val, int index)
{
    vals[index] = val;
}


template <typename KeyType>
BPlusTree<KeyType>::BPlusTree(int degree, int KeySize, string filePath):KeyNum(0),height(0),NodeNum(0),root(NULL),LeafHead(NULL),degree(degree), KeySize(KeySize), filePath(filePath)
{
    init_tree();
//    readFromDiskAll();
}


template <class KeyType>
BPlusTree<KeyType>:: ~BPlusTree()
{
    dropTree(root);
    KeyNum = 0;
    root = NULL;
    height = 0;
}


template <typename KeyType>
void BPlusTree<KeyType>::init_tree()
{
    root = new TreeNode<KeyType>(degree, true);
    this->height = 1;
    this->NodeNum = 1;
    this->degree = degree;
    this->KeyNum = 0;
    this->LeafHead = root;
    
}


template <typename KeyType>
bool BPlusTree<KeyType>::search(Node TestNode, KeyType key, AimNode& AN)
{
    int index = 0;
    if (TestNode->search(key,index)) // find the key in the node
    {
        if (TestNode->isLeaf)
        {
            AN.ANode = TestNode;
            AN.index = index;
            return true; 
        }
        TestNode = TestNode->children[index+1];
        while (!TestNode->isLeaf)
           TestNode = TestNode->children[0];
        AN.ANode = TestNode;
        AN.index = 0;
        return true;
    }
    else 
    {
        if(TestNode->isLeaf)
        {
            AN.ANode = TestNode;
            AN.index = index;
            return false;
        }
        else
            search(TestNode->children[index],key,AN);
    }
}


/*
template <typename KeyType>
bool BPlusTree<KeyType>::find(Node TestNode, KeyType key, AimNode& AN)
{
    int index = index_test = 0;
    while (TestNode->search(key, index))
    {
        if (TestNode->isLeaf)
        {
            AN.ANode = TestNode;
            AN->index = index;
            return true;
        }
        TestNode = TestNode->children[index+1];
        index_test = index;

        if (!TestNode->search(key, index_test))
            break;   
    }
}
*/


template <typename KeyType>
bool BPlusTree<KeyType>::insert(KeyType key,int val)
{
    if (!root) init_tree();

    AimNode AN;
    if (search(root, key, AN))
    {
        cout << "Error:in insert key to index: the duplicated key!" << endl;
        exit(3);
        return false;
    }
    else 
    {
        AN.ANode->insert(key, val);
        if (AN.ANode->count == degree)
            AdjustAfterInsert(AN.ANode);
        KeyNum++;
        return true;
    }
}


template <typename KeyType>
bool BPlusTree<KeyType>::remove(KeyType key)
{
    if (!root) 
    {
        cout << "Error:in delete key: No nodes in the tree!" << endl;
        exit(3);
    }
    
    AimNode AN;
    if (!search(root, key, AN))
    {
        cout << "Error:in delete key: No keys in the tree!" << endl;
        exit(3);
        return false;
    }
    else 
    {
        if (AN.ANode->Parent == NULL)
        {
            AN.ANode->remove(key);
            KeyNum--;
            return AdjustAfterDelete(AN.ANode);
        }    
        else if (AN.index == 0 && AN.ANode != LeafHead)
        {
            int index = 0;
            Node TestNode = AN.ANode->Parent;
            while (!(TestNode->search(key, index)))
                TestNode = TestNode->Parent;
            
            //suppose the degree is bigger than 2
            TestNode->reset_keys(AN.ANode->keys[1], index);
            AN.ANode->remove(key);
            KeyNum--;
            return AdjustAfterDelete(AN.ANode);
        }
        else 
        {
            AN.ANode->remove(key);
            KeyNum--;
            return AdjustAfterDelete(AN.ANode);
        }
    }
    return true;
}


template <typename KeyType>
bool BPlusTree<KeyType>::AdjustAfterInsert(Node TestNode)
{
    KeyType key;
    Node NewNode = TestNode->splite(key);
    NodeNum++;

    if (TestNode->Parent == NULL)
    {
        Node root = new TreeNode<KeyType>(degree, true);
        if(root == NULL)
        {
            cout << "Error: can not allocate memory for the new root in AdjustAfterInsert" << endl;
            exit(1);
            return false;
        }
        this->root = root;
        root->insert(key);
        root->reset_children(TestNode, 0);
        root->reset_children(NewNode, 1);
        
        NewNode->Parent = root;
        TestNode->Parent = root;
        root->isLeaf = 0;
        height++;
        NodeNum++;
        return true;
    }
    else 
    {
        Node ParentNode = TestNode->Parent;
        ParentNode->insert(key);
        int index = 0;
        ParentNode->search(key, index);
        ParentNode->reset_children(NewNode, index+1);
        if (ParentNode->count == degree)
            return AdjustAfterInsert(ParentNode);
        return true;
    }
}

template <class KeyType>
bool BPlusTree<KeyType>::AdjustAfterDelete(Node TestNode)
{
    int mininumsize = (degree - 1)/2;

    if (judge(TestNode))
        return true;

    else 
    {
        if (TestNode->Parent == NULL)
        {
            if (TestNode->count != 0)
                return true;
            else if (TestNode->isLeaf)
                dropTree(root);
            else 
            {
                root = TestNode->children[0];
                root->Parent = NULL;
                delete TestNode;
                TestNode = NULL;
                height--;
                KeyNum--;
                NodeNum--;
            }
        }
        else if (TestNode->isLeaf)
        {
            Node ParentNode = TestNode->Parent, SiblingNode = NULL;
            int index = 0;
            ParentNode->search(TestNode->keys[0], index);
            if (ParentNode->children[0]!=TestNode && (index+1) == ParentNode->count)
            {
                SiblingNode = ParentNode->children[index];
                if (SiblingNode->count > mininumsize)
                {
                    TestNode->insert(SiblingNode->keys[SiblingNode->count-1], SiblingNode->vals[SiblingNode->count-1]);
                    SiblingNode->remove(SiblingNode->keys[SiblingNode->count-1]);
                    ParentNode->reset_keys(TestNode->keys[0], index);
                    ParentNode->reset_vals(TestNode->vals[0], index);
                    return true;
                }
                else 
                {
                    ParentNode->remove(ParentNode->keys[index]);
                    for (int i=0; i<TestNode->count; i++)
                    {
                        SiblingNode->reset_keys(TestNode->keys[i], SiblingNode->count+i);
                        SiblingNode->reset_vals(TestNode->vals[i], SiblingNode->count+i);
                    }
                    SiblingNode->count+=TestNode->count;
                    SiblingNode->NextLeaf = TestNode->NextLeaf;
                    delete TestNode;
                    TestNode = NULL;
                    NodeNum--;

                    return AdjustAfterDelete(ParentNode);
                }
            }
            else
            {
                ParentNode->children[0] == TestNode ? SiblingNode = ParentNode->children[1] : SiblingNode = ParentNode->children[index+2];
                if (SiblingNode->count > mininumsize-1)
                {
                    TestNode->insert(SiblingNode->keys[0], SiblingNode->vals[0]);
                    SiblingNode->remove(SiblingNode->keys[0]);
                    ParentNode->reset_keys(SiblingNode->keys[0], index);
                    ParentNode->reset_vals(SiblingNode->vals[0], index);
                    return true;
                }
                else 
                {
                    ParentNode->children[0] == TestNode ? ParentNode->remove(ParentNode->keys[0]) : ParentNode->remove(ParentNode->keys[index+1]);
                    for (int i=0; i<SiblingNode->count; i++)
                    {
                        TestNode->reset_keys(SiblingNode->keys[i], i+TestNode->count);
                        TestNode->reset_vals(SiblingNode->vals[i], i+TestNode->count);
                    }
                    TestNode->count+=SiblingNode->count;
                    TestNode->NextLeaf = SiblingNode->NextLeaf;
                    if (TestNode->Parent->count)
                        TestNode->Parent->reset_keys(TestNode->children[TestNode->count]->keys[0], TestNode->count);
                    delete SiblingNode;
                    SiblingNode = NULL;
                    NodeNum--;

                    return AdjustAfterDelete(ParentNode);
                }
            }
        }
        else 
        {
            Node ParentNode = TestNode->Parent, SiblingNode = NULL, IndexNode = TestNode->children[0];
            int index = 0;
            while (IndexNode->children[0])
                IndexNode = IndexNode->children[0];
            ParentNode->search(IndexNode->keys[0], index);
            if (ParentNode->children[0]!=TestNode && index+1 == ParentNode->count)
            {
                SiblingNode = ParentNode->children[index];
                if (SiblingNode->count > mininumsize)
                {
                    TestNode->insert(SiblingNode->keys[SiblingNode->count-1]);
                    for (int i=0; i<=TestNode->count; i++)
                        TestNode->reset_children(TestNode->children[i], i+1);
                    TestNode->reset_children(SiblingNode->children[SiblingNode->count], 0);
                    SiblingNode->children[SiblingNode->count]->Parent = TestNode;
                    TestNode->count++;
                    for (int i=0; i<TestNode->count; i++)
                        TestNode->reset_keys(TestNode->keys[i], i+1);
                    TestNode->keys[0] = TestNode->children[1]->keys[0];
                    TestNode->vals[0] = TestNode->children[1]->vals[0];
                    SiblingNode->remove(SiblingNode->keys[SiblingNode->count-1]);
                    ParentNode->reset_keys(TestNode->children[0]->keys[0], index);
                    return true;
                }
                else 
                {
                    SiblingNode->reset_keys(ParentNode->keys[index], SiblingNode->count);
                    SiblingNode->count++;
                    ParentNode->remove(ParentNode->keys[index]);
                    for (int i=0; i<TestNode->count; i++)
                    {
                        SiblingNode->reset_keys(TestNode->keys[i], SiblingNode->count+i);
                        SiblingNode->reset_children(TestNode->children[i], SiblingNode->count+i);
                        SiblingNode->children[SiblingNode->count+i]->Parent = SiblingNode;
                    }
                    SiblingNode->reset_children(TestNode->children[TestNode->count], SiblingNode->count+TestNode->count);
                    SiblingNode->children[SiblingNode->count+TestNode->count]->Parent = SiblingNode;
                    SiblingNode->count+=TestNode->count;
                    SiblingNode->NextLeaf = TestNode->NextLeaf;
                    delete TestNode;
                    TestNode = NULL;
                    NodeNum--;

                    return AdjustAfterDelete(ParentNode);
                }
            }
            else 
            {
                (ParentNode->children[0] == TestNode) ? SiblingNode = ParentNode->children[1] : SiblingNode = ParentNode->children[index+2];
                if (SiblingNode->count > mininumsize-1)
                {
                    TestNode->insert(SiblingNode->children[0]->keys[0]);
                    SiblingNode->children[0]->Parent = TestNode;
                    TestNode->reset_children(SiblingNode->children[0], TestNode->count);
                    for (int i=0; i<SiblingNode->count; i++)
                        SiblingNode->reset_children(SiblingNode->children[i+1], i);
                    SiblingNode->reset_children(NULL, SiblingNode->count);
                    for (int i=0; i<SiblingNode->count-1; i++)
                        SiblingNode->reset_keys(SiblingNode->keys[i+1], i);
                    SiblingNode->reset_keys(0,SiblingNode->count-1);
                    SiblingNode->count--;
                    ParentNode->reset_keys(SiblingNode->children[0]->keys[0], index);
                    return true;
                }
                else 
                {
                    TestNode->reset_keys(ParentNode->keys[index], TestNode->count);

                    TestNode->count++;
                    ParentNode->children[0] == TestNode ? ParentNode->remove(ParentNode->keys[0]) : ParentNode->remove(ParentNode->keys[index+1]);
                    for (int i=0; i<=SiblingNode->count; i++)
                    {
                        TestNode->reset_keys(SiblingNode->keys[i], TestNode->count+i);
                        TestNode->reset_children(SiblingNode->children[i], TestNode->count+i);
                        TestNode->children[TestNode->count+i]->Parent = SiblingNode;
                    }
                    TestNode->count+=SiblingNode->count;
                    TestNode->NextLeaf = SiblingNode->NextLeaf;
                    delete SiblingNode;
                    SiblingNode = NULL;
                    NodeNum--;

                    return AdjustAfterDelete(ParentNode);
                }
            }
        }
    }
}


template <typename KeyType>
void BPlusTree<KeyType>::dropTree(Node TestNode)
{
    if (!TestNode)  return;
    if (!TestNode->isLeaf)
    {
        for(int i=0;i <= TestNode->count;i++)
        {
            dropTree(TestNode->children[i] );
            TestNode->children[i] = NULL;
        }
    }

    delete TestNode;
    TestNode = NULL;
    NodeNum--;
    return;
}

template <class KeyType>
bool BPlusTree<KeyType>::judge(Node TestNode)
{
    if (TestNode->isLeaf)
    {
        if (TestNode->count < (degree-1)/2 || TestNode->count >= degree)
        {
            return false;
        }
        else 
        {
            return true;
        }
    }
    for (int i=0; i<TestNode->count; i++)
    {
        if (TestNode->children[i]->count!=0)
            if(!judge(TestNode->children[i]))
                return false;
    }
    if (TestNode->count == 0 || TestNode->keys[TestNode->count-1] == 0)
        return false;
    else 
        return true;
}




template <typename KeyType>
void BPlusTree<KeyType>::ReadDiskNode(blockNode* btmp)
{
    char* index = bm.getindex(*btmp);
    char* value = index[keySize];
    KeyType key;
    int value;
    int valueSize = sizeof(int);

    while(value - bm.getindex(*btmp) < bm.getSize(*btmp))
    {
        key = *(KeyType*)index;
        value = *(int*)value;
        insert(key, value);
        value += keySize + valueSize;
        index += keySize + valueSize;
    }
    
}

template <typename KeyType>
void BPlusTree<KeyType>::ReadDiskAll()
{
    file = bm.getFile(fileName.c_str());    ///?
    blockNode* btmp = bm.getBlockHead(file);    ///?
    while (btmp!=NULL)
    {   
        readFromDisk(btmp);
        if(btmp->ifbottom) break;
        btmp = bm.getNextBlock(file, btmp);
    }

}


template <class KeyType>
void BPlusTree<KeyType>::WriteDisk()
{
    blockNode* btmp = bm.getBlockHead(file);
    Node ntmp = this->LeafHead;
    int valueSize = sizeof(int);
    while(ntmp != NULL)
    {
        bm.set_usingSize(*btmp, 0);
        bm.set_dirty(*btmp);
        for(int i = 0;i < ntmp->count;i ++)
        {
            char* key = (char*)&(ntmp->keys[i]);
            char* value = (char*)&(ntmp->vals[i]);
            memcpy(bm.get_content(*btmp)+bm.get_usingSize(*btmp),key,keySize);
            bm.set_usingSize(*btmp, bm.get_usingSize(*btmp) + keySize);
            memcpy(bm.get_content(*btmp)+bm.get_usingSize(*btmp),value,valueSize);
            bm.set_usingSize(*btmp, bm.get_usingSize(*btmp) + valueSize);
        }
        
        btmp = bm.getNextBlock(file, btmp);
        ntmp = ntmp->nextLeafNode;
    }
    while(1)// delete the empty node
    {
        if(btmp->ifbottom)
            break;
        bm.set_usingSize(*btmp, 0);
        bm.set_dirty(*btmp);
        btmp = bm.getNextBlock(file, btmp);
    }
    
}




int main()
{    
    
    BPlusTree<int> tree(3,4,"a");
    for (int i=0; i<10; i++)
        tree.insert(1+i,0);



    for (int i=0; i<10; i++)
        tree.remove(i+1);
    /*
    BPlusTree<int> tree(3,4,"a");
    for (int i=0; i<10; i++)
        tree.insert(10-i,0);



    for (int i=0; i<10; i++)
        tree.remove(10-i);
    
    /*
    TreeNode<int> node(3, 1);
    for (int i=0; i<20; i++)
        node.insert(i+1, 0);
    for (int i=0; i<20; i++)
        node.remove(i+1);
    */
}