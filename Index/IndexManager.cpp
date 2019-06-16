#include <iostream>
#include <vector>
/*
#include "API.h"
#include "Catalog.h"
*/
#include "IndexManager.h"
#include "BPlusTree.h"
#include "BufferManager.h"
using namespace std;

/* 
IndexManager::IndexManager()
{
     
    vector<IndexInfo>  indexNameVector;
    indexNameVector = Catalog::List_AllIndex();
    
    for(vector<IndexInfo>::iterator i = indexNameVector.begin();i != indexNameVector.end();i ++)
        createIndex(i->indexName, i->type);
    
}
*/

IndexManager::IndexManager()
{
}
IndexManager::~IndexManager()
{
}

void IndexManager::createIndex(string filePath,int type)
{
    int keySize = getKeySize(type);
    int degree = getDegree(type);
    if(type == TYPE_INT)
    {
        BPlusTree<int> *tree = new BPlusTree<int>(filePath,keySize,degree);
        indexIntMap.insert(intMap::value_type(filePath, tree));
    }
    else if(type == TYPE_FLOAT)
    {
        BPlusTree<float> *tree = new BPlusTree<float>(filePath,keySize,degree);
        indexFloatMap.insert(floatMap::value_type(filePath, tree));
    }
    else // string
    {
        BPlusTree<string> *tree = new BPlusTree<string>(filePath,keySize,degree);
        indexStringMap.insert(stringMap::value_type(filePath, tree));
    }
}

void IndexManager::dropIndex(string filePath,int type)
{
    if (type == TYPE_INT)
    {
        intMap::iterator Intelement = indexIntMap.find(filePath);
        if (Intelement == indexIntMap.end())
        {
            cout <<"Error:in drop index(int), no index " << filePath << " exist" << endl;
            return ;
        }
        else
        {
            delete Intelement->second;
            indexIntMap.erase(Intelement);
        }
    }
    else if (type == TYPE_FLOAT)
    {
        floatMap::iterator Floatelement = indexFloatMap.find(filePath);
        if (Floatelement == indexFloatMap.end())
        {
            cout <<"Error:in drop index(float), no index " << filePath << " exist" << endl;
            return ;
        }
        else
        {
            delete Floatelement->second;
            indexFloatMap.erase(Floatelement);
        }
    }
    else
    {
        stringMap::iterator Stringelement = indexStringMap.find(filePath);
        if (Stringelement == indexStringMap.end())
        {
            cout <<"Error:in drop index(string), no index " << filePath << " exist" << endl;
            return ;
        }
        else
        {
            delete Stringelement->second;
            indexStringMap.erase(Stringelement);
        }

    }
}

int IndexManager::searchIndex(string filePath,string key,int type)
{
    setKey(key,type);
    if (type == TYPE_INT)
    {
        intMap::iterator Intelement = indexIntMap.find(filePath);
        if (Intelement == indexIntMap.end())
        {
            cout <<"Error:in search index(int), no index " << filePath << " exist" << endl;
            return -1;
        }
        else
        {
            return Intelement->second->search(keyTmps.intTmp);
        }
    }
    else if (type == TYPE_FLOAT)
    {
        floatMap::iterator Floatelement = indexFloatMap.find(filePath);
        if (Floatelement == indexFloatMap.end())
        {
            cout <<"Error:in search index(float), no index " << filePath << " exist" << endl;
            return -1;
        }
        else
        {
            return Floatelement->second->search(keyTmps.floatTmp);
        }
    }
    else
    {
        stringMap::iterator Stringelement = indexStringMap.find(filePath);
        if (Stringelement == indexStringMap.end())
        {
            cout <<"Error:in search index(string), no index " << filePath << " exist" << endl;
            return -1;
        }
        else
        {
            return Stringelement->second->search(keyTmps.stringTmp);
        }
    }
}


void IndexManager::insertIndex(string filePath,string key,int blockOffset,int type)
{
    setKey(key, type);

    if(type == TYPE_INT)
    {
        intMap::iterator Intelement = indexIntMap.find(filePath);
        if(Intelement == indexIntMap.end())
        {
            cout << "Error:in insert index(int), no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            Intelement->second->insert(keyTmps.intTmp, blockOffset);
        }
    }
    else if(type == TYPE_FLOAT)
    {
        floatMap::iterator Floatelement = indexFloatMap.find(filePath);
        if(Floatelement == indexFloatMap.end())
        {
            cout << "Error:in insert index(float), no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            Floatelement->second->insert(keyTmps.floatTmp, blockOffset);

        }
    }
    else // string
    {
        stringMap::iterator Stringelement = indexStringMap.find(filePath);
        if(Stringelement == indexStringMap.end())
        {
            cout << "Error:in insert index(string), no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            Stringelement->second->insert(keyTmps.stringTmp, blockOffset);
        }
    }
}


void IndexManager::deleteIndex(string filePath,string key,int type)
{
    setKey(key, type);

    if(type == TYPE_INT)
    {
        intMap::iterator Intelement = indexIntMap.find(filePath);
        if(Intelement == indexIntMap.end())
        {
            cout << "Error:in delete index(int), no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            Intelement->second->remove(keyTmps.intTmp);
        }
    }
    else if(type == TYPE_FLOAT)
    {
        floatMap::iterator Floatelement = indexFloatMap.find(filePath);
        if(Floatelement == indexFloatMap.end())
        {
            cout << "Error:in delete index(float), no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            Floatelement->second->remove(keyTmps.floatTmp);

        }
    }
    else // string
    {
        stringMap::iterator Stringelement = indexStringMap.find(filePath);
        if(Stringelement == indexStringMap.end())
        {
            cout << "Error:in delete index(string), no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            Stringelement->second->remove(keyTmps.stringTmp);
        }
    }
}


int IndexManager::getKeySize(int type)
{
    if (type == TYPE_FLOAT)
        return sizeof(float);
    else if (type == TYPE_INT)
        return sizeof(int);
    else if (type > 0)
        return type+1;
    else
    {
        cout << "Error: In getKeySize: invaild input" << endl;
        exit(2);
    }
}

int IndexManager::getDegree(int type)
{
    //int degree =  BLOCK_SIZE/(sizeof(int)+getKeySize(type));
    int degree =  BLOCK_SIZE/(sizeof(int)+getKeySize(type));
    if (degree%2 == 0) degree-=1;           //???why must be odd
    return degree;
}


void IndexManager::setKey(string key, int type)
{
    stringstream Tmp;
    Tmp << key;
    if(type == TYPE_INT)
        Tmp >> keyTmps.intTmp;
    else if(type == TYPE_FLOAT)
        Tmp >> keyTmps.floatTmp;
    else if(type > 0)
        Tmp >> keyTmps.stringTmp;
    else
        cout << "Error: in setKey: invalid type" << endl;
}

/* 
void IndexManager::CreateTable(Create_Table table)
{
}
void IndexManager::DropTable(Drop_Table table)
{    
}
void IndexManager::Create_Index(Create_Index index)
{
    IndexInfo info = Catalog::Find_index(index);
    createIndex(info.tablename, info.indexName, info.type);               //need 
}

void IndexManager::Delete_Index(Drop_Index index)
{
    IndexInfo info = Catalog::Find_index(index);
    dropIndex(info.tablename, info.indexname, info.type);
}
*/
