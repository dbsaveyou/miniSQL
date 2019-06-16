#ifndef IndexManager_h
#define IndexManager_h

#include <string>
#include <stdio.h>
#include <map>
#include <sstream>
#include "bplusself.h"
#include "basic.h"
using namespace std;

class IndexInfo
{
private:
    string indexName;
    string tableName;
    string Attribute;
    int type;
public:
    IndexInfo(string indexName,string tableName,string Attribute,int type) : indexName(indexName), tableName(tableName), Attribute(Attribute), type(type){}
};

class API;

class IndexManager{
private:
    typedef map<string,BPlusTree<int> *> intMap;
    typedef map<string,BPlusTree<string> *> stringMap;
    typedef map<string,BPlusTree<float> *> floatMap;
    //
    int static const TYPE_FLOAT = -1;
    int static const TYPE_INT = 0;
    // other values mean the size of the char.Eg, 4 means char(4);
    
    API *api; // to call the functions of API
    
    intMap indexIntMap;
    stringMap indexStringMap;
    floatMap indexFloatMap;
    struct keyTmp{
        int intTmp;
        float floatTmp;
        string stringTmp;
    }; // the struct to help to convert the inputed string to specfied type

    struct keyTmp keyTmps;
    int getDegree(int type);
    int getKeySize(int type);  
    void setKey(int type,string key);
    
    
public:
    IndexManager(API *api);
    ~IndexManager();
    void createIndex(string filePath,int type);
    void dropIndex(string filePath,int type);               //need type
    int searchIndex(string filePath,string key,int type);       
    void insertIndex(string filePath,string key,int blockOffset,int type);
    void deleteIndex(string filePath, string key,int type);

    void Create_Index(Create_Index tmp2);
    void Delete_index(Drop_index tmp3);


};
#endif
