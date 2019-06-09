#include "IndexManager.h"
#include <iostream>
#include "API.h"
#include <vector>
using namespace std;

IndexManager::IndexManager(API *api)
{
    vector<IndexInfo>  nowIndexInfo;
    api->allIndexAddressInfoGet(&IndexInfo);
    for(vector<IndexInfo>::iterator i = IndexInfo.begin();i != IndexInfo.end();i ++)
        createIndex(i->indexName, i->type);
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
            cout <<"Error:in dropindex, no index " << filePath << " exist" << endl;
            return ;
        }
        else 
        {
            delete Intelement->second;
            return indexIntMap.erase(itInt);
        }
    }
    else if (type == TYPE_FLOAT)
    {
        floatMap::iterator Floatelement = indexFloatMap.find(filePath);
        if (Floatelement == indexFloatMap.end())
        {
            cout <<"Error:in dropindex, no index " << filePath << " exist" << endl;
            return ;
        }
        else 
        {
            delete Floatelement->second;
            return indexFloatMap.erase(Floatelement);
        }
    }
    else 
    {
        stringMap::iterator Stringelement = indexStringMap.find(filePath);
        if (Stringelement == indexStringMap.end())
        {
            cout <<"Error:in dropindex, no index " << filePath << " exist" << endl;
            return ;
        }
        else
        {
            delete Stringelement->second;
            return indexStringMap.erase(Stringelement);
        }
        
    }
}

int searchIndex(string filePath,string key,int type)
{
    setkey(type, key);
    if (type == TYPE_INT)
    {
        intMap::iterator Intelement = indexIntMap.find(filePath);
        if (Intelement == indexIntMap.end())
        {
            cout <<"Error:in dropindex, no index " << filePath << " exist" << endl;
            return ;
        }
        else 
        {
            return Intelement->second->search(this->keyTmps.intTmp);
        }   
    }
    else (type == TYPE_FLOAT)
    {
        floatMap::iterator Floatelement = indexFloatMap.find(filePath);
        if (Floatelement == indexFloatMap.end())
        {
            cout <<"Error:in dropindex, no index " << filePath << " exist" << endl;
            return;
        }
        else 
        {
            return Floatelement->second->search(this->keyTmps.intTmp);
        }
    }
    else 
    {
        StringMap::iterator Stringelement = indexStringMap.find(filePath);
        if (Stringelement == indexStirngMap.end())
        {
            cout <<"Error:in dropindex, no index " << filePath << " exist" << endl;
            return;
        }
        else 
        {
            return Stringelement->second->search(this->keyTmps.intTmp);
        }
    }
}

/*
void IndexManager::insertIndex(string filePath,string key,int blockOffset,int type)
{
    setKey(type, key);

    if(type == TYPE_INT)
    {
        intMap::iterator itInt = indexIntMap.find(filePath);
        if(itInt == indexIntMap.end())
        {
            cout << "Error:in search index, no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            itInt->second->insertKey(kt.intTmp,blockOffset);
        }
    }
    else if(type == TYPE_FLOAT)
    {
        floatMap::iterator itFloat = indexFloatMap.find(filePath);
        if(itFloat == indexFloatMap.end())
        {
            cout << "Error:in search index, no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            itFloat->second->insertKey(kt.floatTmp,blockOffset);
            
        }
    }
    else // string
    {
        stringMap::iterator itString = indexStringMap.find(filePath);
        if(itString == indexStringMap.end())
        {
            cout << "Error:in search index, no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            itString->second->insertKey(key,blockOffset);
        }
    }
}


void IndexManager::deleteIndexByKey(string filePath,string key,int type)
{
    setKey(type, key);

    if(type == TYPE_INT)
    {
        intMap::iterator itInt = indexIntMap.find(filePath);
        if(itInt == indexIntMap.end())
        {
            cout << "Error:in search index, no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            itInt->second->deleteKey(kt.intTmp);
        }
    }
    else if(type == TYPE_FLOAT)
    {
        floatMap::iterator itFloat = indexFloatMap.find(filePath);
        if(itFloat == indexFloatMap.end())
        {
            cout << "Error:in search index, no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            itFloat->second->deleteKey(kt.floatTmp);
            
        }
    }
    else // string
    {
        stringMap::iterator itString = indexStringMap.find(filePath);
        if(itString == indexStringMap.end())
        {
            cout << "Error:in search index, no index " << filePath <<" exits" << endl;
            return;
        }
        else
        {
            itString->second->deleteKey(key);
        }
    }
}

*/


int getKeySize(int type)
{
    if (type == TYPE_FLOAT)
        return sizeof(float);
    else if (type == TYPE_INT)
        return sizeof(int);
    else if (type > 0)
        return type.length()+1;
    else 
    {
        cout << "Error: In getKeySize: invaild input" << endl;
        exit(2);
    }
}

int getDegree(int type)
{
    int degree = bm.getBlockSize()/(sizeof(int)+getKeySize(type));
    if (degree%2 == 0) degree-=1;           //???why must be odd
    return degree;
}


void IndexManager::setKey(int type,string key)
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
        cout << "Error: in getKey: invalid type" << endl;

}

