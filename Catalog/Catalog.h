#ifndef CATALOG_H
#define CATALOG_H

#include <cstdio>
#include <vector>
#include "BufferManager.h"
#include "Basic.h"
#include "IndexInfo.h"
using namespace std;

class Catalog
{
public:
    BufferManager bm;
    Catalog(){};
    ~Catalog(){};
    //TABLE
    bool Table_Exist(string tableName);
    /*TRUE EXIST, FALSE NON-EXISTED */
    int CreateTable(Create_Table table);
    int DropTable(Drop_Table table);
    //ATTRIBUTE
    int Get_PrimaryKey(vector<Attribute> attr);
    bool CheckAttributeUnique(string tableName, string Attr_name);
    bool Check_AttributeNum(string tableName, int AttributeNum);
    bool Attribute_Exist(string tableName,  string Attr_name);     
    int getAttributeType(string AttributeName, string tableName);
    vector<Attribute> List_TableAttributes(string table_name);
    //INDEX
    int CreateIndex(Create_Index index);
    int setIndexOnAttribute(string tableName, string Attr_name,string indexName);
    int Delete_Index(Drop_Index index);
    int revokeIndexOnAttribute(string tableName, string Attr_name); 
    bool Index_Exist(string indexName);
    IndexInfo Find_Index(string indexName);
    vector<IndexInfo> List_AllIndex();
    vector<IndexInfo> List_Table_AllIndex(string tableName);

    
};

#endif /* CatalogManager_h */
