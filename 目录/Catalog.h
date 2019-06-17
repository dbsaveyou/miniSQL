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
    /*11 table已存在
      12 多个primary key
      10 创建成功*/
    int DropTable(Drop_Table table);
    /*21 table不存在
      20 删除成功*/


    //ATTRIBUTE
    int Get_PrimaryKey(vector<Attribute> attr);
    /*返回pk的index，不存在pk返回-1，多个pk返回-2*/
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
