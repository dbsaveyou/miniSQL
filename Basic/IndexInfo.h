#ifndef INDEXINFO_H 
#define INDEXINFO_H
#include <string>
#include <stdio.h>
#include <map>
#include <sstream>
//test
using namespace std;
class IndexInfo
{
public:
    string indexName;
    string tableName;
    string Attribute;
    int type;
    IndexInfo(){};
    ~IndexInfo(){};
    IndexInfo(string indexName, string tableName, string Attribute, int type) : indexName(indexName), tableName(tableName), Attribute(Attribute), type(type) {}
};

#endif
// direclty here
// Modify local to remote test
