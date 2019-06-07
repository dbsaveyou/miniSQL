#ifndef CATALOG_H
#define CATALOG_H

#include <cstdio>
#include <vector>
#include "BufferManager.h"
#include "Basic.h"
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
    /*true-unique, false not unique*/
    bool Check_AttributeNum(string tableName, int AttributeNum);
    /*总数符合true，不符合false（用于判断插入数据的属性值数量是否一样）*/
    bool Attribute_Exist(string tableName,  string Attr_name);     
    /*存在该attr-true*/
    int getAttributeType(string AttributeName, string tableName);
    /*返回type定义与attr类定义一致*/
    vector<Attribute> List_TableAttributes(string table_name);
    /*列出表中所有attr*/


    //INDEX
    int CreateIndex(Create_Index index);
    int setIndexOnAttribute(string tableName, string Attr_name,string indexName);/*用于修改table中变量*/
    /*31 table不存在
      32 attr 不存在
      33 indexname已存在
      34 attr non-unique
      30 success*/   
    int Delete_Index(Drop_Index index);
    int revokeIndexOnAttribute(string tableName, string Attr_name); 
    /*41 index不存在
      40 success*/
    bool Index_Exist(string indexName);
    /*判断index是否存在 true-存在*/
    IndexInfo Find_Index(string indexName);
    /*返回index信息，格式参考indexinfo*/
    vector<IndexInfo> List_AllIndex();
    /*列出所有index*/

    
};

#endif /* CatalogManager_h */
