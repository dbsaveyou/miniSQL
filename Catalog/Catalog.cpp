#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include "Catalog.h"
#include "IndexInfo.h"
#include "DBFile.h"
using namespace std;

//----------------TABLE-----------------
bool Catalog::Table_Exist(string tableName)
{
    FILE *fp;
    fp = fopen(tableName.c_str(), "r");
    if (fp == NULL)
        return false;
    else
    {
        fclose(fp);
    }
    return true;
}
int Catalog::CreateTable(Create_Table table)
{
    bool table_exist = Table_Exist(table.table_name);
    if (table_exist == true)
        return 11;
    FILE *fp;
    fp = fopen(table.table_name.c_str(), "w+");

    /*获得指向文件块的指针*/
    File_Node *fn = bm.get_File(table.table_name.c_str());
    Block_Node *bn = bm.getBlockHead(fn);

    /*判断主键是否符合要求*/
    /*存储方式：主键index+attnum+att结构体*/
    char *addressBegin = (*bn).get_Content();
    *addressBegin = Get_PrimaryKey(table.attr);
    if (*addressBegin == -2)
        return 12;
    if (*addressBegin != -1)
    {
        Create_Index index;
        index.col_name = table.attr[*addressBegin].AttributeName;
        index.table_name = table.table_name;
        index.index_name = "Primary" + index.col_name;
        Create_Index(index);
    }
    addressBegin += sizeof(int);
    *addressBegin = table.num_attr;
    addressBegin += sizeof(int);
    (*bn).set_UsedSize((*bn).get_UsedSize() + 2 * sizeof(int));
    int count = 0;
    for (int i = 0; i < table.num_attr; i++)
    {
        count++;
        if ((*bn).get_RemainedSize() >= count * sizeof(Attribute))
        {
            memcpy(addressBegin, &(table.attr[i]), sizeof(Attribute));
            addressBegin += sizeof(Attribute);
        }
        else
        {
            (*bn).set_UsedSize((*bn).get_UsedSize() + (i - 1) * sizeof(Attribute));
            (*bn).set_Dirty(true);
            bn = bm.getNextBlock(fn, bn);
            count = 0;
        }
    }

    return 10;
}
int Catalog::DropTable(Drop_Table table)
{
    bool table_exist = Table_Exist(table.table_name);
    if (table_exist == false)
        return 21;
    //删除
    bm.free_FileNode(table.table_name.c_str());

    return 20;
}

//----------------ATTRIBUTE-----------------
int Catalog::Get_PrimaryKey(vector<Attribute> attr)
{
    int i;
    int pk = -1;
    for (i = 0; i < attr.size(); i++)
    {
        if (attr[i].primarykey == true && pk == -1)
            pk = i;

        if (attr[i].primarykey == true && pk != -1)
            return -2;
    }
    if (pk == -1)
        return -1;
    return pk;
}

bool Catalog::CheckAttributeUnique(string tableName, string Attr_name)
{
    File_Node *fn = bm.get_File(tableName.c_str());
    Block_Node *bn = bm.getBlockHead(fn);

    char *addressBegin = (*bn).get_Content();
    addressBegin += sizeof(int);
    int size = *addressBegin;
    addressBegin += sizeof(int);
    Attribute *a = (Attribute *)addressBegin;
    for (int i = 0; i < size; i++)
    {
        int count = 0;
        if (a[count].AttributeName == Attr_name && a[i].Unique == true)
            return true;
        a++;
        count++;
        if (i == count && (*bn).get_UsedSize() < count * sizeof(Attribute) + 2 * sizeof(int) || i == count && (*bn).get_UsedSize() < count * sizeof(Attribute))
        {
            bn = bm.getNextBlock(fn, bn);
            addressBegin = (*bn).get_Content();
            Attribute *a = (Attribute *)addressBegin;
            count = 0;
        }
    }

    return false;
}

bool Catalog::Check_AttributeNum(string tableName, int AttributeNum)
{
    File_Node *fn = bm.get_File(tableName.c_str());
    Block_Node *bn = bm.getBlockHead(fn);

    char *addressBegin = (*bn).get_Content();
    addressBegin += sizeof(int);
    int size = *addressBegin;
    if (size != AttributeNum)
        return false;
    else
        return true;
}
bool Catalog::Attribute_Exist(string tableName, string Attr_name)
{
    File_Node *fn = bm.get_File(tableName.c_str());
    Block_Node *bn = bm.getBlockHead(fn);

    char *addressBegin = (*bn).get_Content();
    addressBegin += sizeof(int);
    int size = *addressBegin;
    addressBegin += sizeof(int);
    Attribute *a = (Attribute *)addressBegin;
    for (int i = 0; i < size; i++)
    {
        int count = 0;
        if (a[count].AttributeName == Attr_name)
            return true;
        a++;
        count++;
        if (i == count && (*bn).get_UsedSize() < count * sizeof(Attribute) + 2 * sizeof(int) || i == count && (*bn).get_UsedSize() < count * sizeof(Attribute))
        {
            bn = bm.getNextBlock(fn, bn);
            addressBegin = (*bn).get_Content();
            Attribute *a = (Attribute *)addressBegin;
            count = 0;
        }
    }

    return false;
}
int Catalog::getAttributeType(string AttributeName, string tableName)
{
    File_Node *fn = bm.get_File(tableName.c_str());
    Block_Node *bn = bm.getBlockHead(fn);

    char *addressBegin = (*bn).get_Content();
    addressBegin += sizeof(int);
    int size = *addressBegin;
    addressBegin += sizeof(int);
    Attribute *a = (Attribute *)addressBegin;

    for (int i = 0; i < size; i++)
    {
        int count = 0;
        if (a[count].AttributeName == AttributeName)
            return a[count].AttributeType;
        a++;
        count++;
        if (i == count && (*bn).get_UsedSize() < count * sizeof(Attribute) + 2 * sizeof(int) || i == count && (*bn).get_UsedSize() < count * sizeof(Attribute))
        {
            bn = bm.getNextBlock(fn, bn);
            addressBegin = (*bn).get_Content();
            Attribute *a = (Attribute *)addressBegin;
            count = 0;
        }
    }
}
vector<Attribute> Catalog::List_TableAttributes(string table_name)
{
    File_Node *fn = bm.get_File(table_name.c_str());
    Block_Node *bn = bm.getBlockHead(fn);

    vector<Attribute> Attr;
    char *addressBegin = (*bn).get_Content();
    addressBegin += (1 + sizeof(int));
    int size = *addressBegin;
    addressBegin++;
    Attribute *a = (Attribute *)addressBegin;
    for (int i = 0; i < size; i++)
    {
        int count = 0;
        Attr.push_back(*a);
        a++;
        count++;
        if (i == count && (*bn).get_UsedSize() < count * sizeof(Attribute) + 2 * sizeof(int) || i == count && (*bn).get_UsedSize() < count * sizeof(Attribute))
        {
            bn = bm.getNextBlock(fn, bn);
            addressBegin = (*bn).get_Content();
            Attribute *a = (Attribute *)addressBegin;
            count = 0;
        }
    }

    return Attr;
}

//----------------INDEX-----------------
int Catalog::CreateIndex(Create_Index index)
{
    bool cindex = Index_Exist(index.index_name);
    if (cindex == true)
        return 33;
    bool table_exist = Table_Exist(index.table_name);
    if (table_exist == false)
        return 31;
    bool attribute_exist = Attribute_Exist(index.table_name, index.col_name);
    if (attribute_exist == false)
        return 32;
    bool attribute_unique = CheckAttributeUnique(index.table_name, index.col_name);
    if (attribute_unique == false)
        return 34;

    File_Node *fn = bm.get_File("Indexs");
    Block_Node *bn = bm.getBlockHead(fn);
    IndexInfo i(index.index_name, index.table_name, index.col_name, getAttributeType(index.col_name, index.table_name));
    while (bn != NULL)
    {
        if ((*bn).get_RemainedSize() >= sizeof(IndexInfo))
        {
            char *addressBegin;
            addressBegin = (*bn).get_Content() + (*bn).get_UsedSize();
            memcpy(addressBegin, &i, sizeof(IndexInfo));
            (*bn).set_UsedSize((*bn).get_UsedSize() + sizeof(IndexInfo));
            (*bn).set_Dirty(true);
            return this->setIndexOnAttribute(index.table_name, index.col_name, index.index_name);
        }
        else
        {
            bn = bm.getNextBlock(fn, bn);
        }
    }
}
int Catalog::setIndexOnAttribute(string tableName, string Attr_name, string indexName)
{
    File_Node *fn = bm.get_File(tableName.c_str());
    Block_Node *bn = bm.getBlockHead(fn);
    char *addressBegin = (*bn).get_Content();
    addressBegin += sizeof(int);
    int size = *addressBegin;
    addressBegin += sizeof(int);
    Attribute *a = (Attribute *)addressBegin;
    int i;

    for (int i = 0; i < size; i++)
    {
        int count = 0;
        if (a[count].AttributeName == Attr_name)
        {
            a->withindex = true;
            (*bn).set_Dirty(true);
            return 30;
        }
        a++;
        count++;
        if (i == count && (*bn).get_UsedSize() < count * sizeof(Attribute) + 2 * sizeof(int) || i == count && (*bn).get_UsedSize() < count * sizeof(Attribute))
        {
            bn = bm.getNextBlock(fn, bn);
            addressBegin = (*bn).get_Content();
            Attribute *a = (Attribute *)addressBegin;
            count = 0;
        }
    }
}
int Catalog::Delete_Index(Drop_Index index)
{
    bool cindex = Index_Exist(index.index_name);
    if (cindex == false)
        return 41;

    File_Node *fn = bm.get_File("Indexs");
    Block_Node *bn = bm.getBlockHead(fn);

    char *addressBegin;
    addressBegin = (*bn).get_Content();
    IndexInfo *i = (IndexInfo *)addressBegin;
    int j = 0;
    for (j = 0; j < ((*bn).get_UsedSize() / sizeof(IndexInfo)); j++)
    {
        if ((*i).indexName == index.index_name)
        {
            break;
        }
        i++;
    }
    this->revokeIndexOnAttribute((*i).tableName, (*i).Attribute);
    for (; j < ((*bn).get_UsedSize() / sizeof(IndexInfo) - 1); j++)
    {
        (*i) = *(i + sizeof(IndexInfo));
        i++;
    }
    (*bn).set_UsedSize((*bn).get_UsedSize() - sizeof(IndexInfo));
    (*bn).set_Dirty(true);

    return 40;
}
int Catalog::revokeIndexOnAttribute(string tableName, string Attr_name)
{
    File_Node *fn = bm.get_File(tableName.c_str());
    Block_Node *bn = bm.getBlockHead(fn);

    char *addressBegin = (*bn).get_Content();
    addressBegin += sizeof(int);
    int size = *addressBegin;
    addressBegin++;
    Attribute *a = (Attribute *)addressBegin;
    int i;

    for (int i = 0; i < size; i++)
    {
        int count = 0;
        if (a[count].AttributeName == Attr_name)
        {
            a->withindex = false;
            (*bn).set_Dirty(true);
            return 30;
        }
        a++;
        count++;
        if (i == count && (*bn).get_UsedSize() < count * sizeof(Attribute) + 2 * sizeof(int) || i == count && (*bn).get_UsedSize() < count * sizeof(Attribute))
        {
            bn = bm.getNextBlock(fn, bn);
            addressBegin = (*bn).get_Content();
            Attribute *a = (Attribute *)addressBegin;
            count = 0;
        }
    }
}


bool Catalog::Index_Exist(string indexName)
{
    File_Node *fn = bm.get_File("Indexs");
    Block_Node *bn = bm.getBlockHead(fn);

    while (bn != NULL)
    {
        char *addressBegin;
        addressBegin = (*bn).get_Content();
        IndexInfo *i = (IndexInfo *)addressBegin;
        for (int j = 0; j < ((*bn).get_UsedSize() / sizeof(IndexInfo)); j++)
        {
            if ((*i).indexName == indexName)
            {
                return true;
            }
            i++;
        }
        bn = bm.getNextBlock(fn, bn);
    }
    return false;
}

vector<IndexInfo> Catalog::List_AllIndex()
{
    File_Node *fn = bm.get_File("Indexs");
    Block_Node *bn = bm.getBlockHead(fn);

    vector<IndexInfo> indexall;
    while (bn != NULL)
    {
        char *addressBegin;
        addressBegin = (*bn).get_Content();
        IndexInfo *i = (IndexInfo *)addressBegin;

        for (int j = 0; j < ((*bn).get_UsedSize() / sizeof(IndexInfo)); j++)
        {
            indexall.push_back((*i));
            i++;
        }
        bn = bm.getNextBlock(fn, bn);
    }
    return indexall;
}

IndexInfo Catalog::Find_Index(string indexName)
{
    File_Node *fn = bm.get_File("Indexs");
    Block_Node *bn = bm.getBlockHead(fn);

    while (bn != NULL)
    {
        char *addressBegin;
        addressBegin = (*bn).get_Content();
        IndexInfo *i = (IndexInfo *)addressBegin;
        for (int j = 0; j < ((*bn).get_UsedSize() / sizeof(IndexInfo)); j++)
        {
            if ((*i).indexName == indexName)
                return *i;
            i++;
        }
        bn = bm.getNextBlock(fn, bn);
    }
}
