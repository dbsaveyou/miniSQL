#ifndef IndexManager_h
#define IndexManager_h

#include <string>
#include <stdio.h>
#include <map>
#include <sstream>
#include "BPlusTree.h"
#include "Basic.h"
//#include "IndexInfo.h"
using namespace std;


class API;

class IndexManager{
private:
    typedef map<string,BPlusTree<int> *> intMap;
    typedef map<string,BPlusTree<string> *> stringMap;
    typedef map<string,BPlusTree<float> *> floatMap;
    //
    int static const TYPE_FLOAT = -1;
    int static const TYPE_INT = 0;
    static const int BLOCK_SIZE = 4096;
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


public:
    //IndexManager(API *api);
    IndexManager();
    ~IndexManager();
    void createIndex(string filePath,int type);
    void dropIndex(string filePath,int type);               //need type
    int searchIndex(string filePath,string key,int type);
    vector<int> searchIndexLeft(string filePath, string key, int type);
    vector<int> searchIndexRight(string filePath, string key, int type);
    
    void insertIndex(string filePath,string key,int blockOffset,int type);
    void deleteIndex(string filePath, string key,int type);
    int getDegree(int type);
    int getKeySize(int type);
    void setKey(string key, int type);
    
    void CreateTable(Create_Table table);
    void DropTable(Drop_Table table);
    void Create_Index(Create_Index index);
    void Delete_Index(Drop_Index index);
    
};
#endif
