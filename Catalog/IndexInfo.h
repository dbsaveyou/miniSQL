#ifndef IndexManager_h 
#define IndexManager_h
#include <string>
#include <stdio.h>
#include <map>
#include <sstream>

using namespace std;
class IndexInfo
{
public:
    string indexName;
    string tableName;
    string Attribute;
    int type;

    IndexInfo(string indexName, string tableName, string Attribute, int type) : indexName(indexName), tableName(tableName), Attribute(Attribute), type(type) {}
};

#endif
// direclty here
// Modify local to remote test
