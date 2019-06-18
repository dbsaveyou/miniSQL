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

    int i, j;
    for (i = 0; i < table.num_attr; i++)
    {
        for (j = 0; j < table.num_attr; j++)
        {
            if (table.attr[i].AttributeName == table.attr[j].AttributeName && i != j)
                return 13;
        }
    }

    FILE *fp;
    fp = fopen(table.table_name.c_str(), "w+");
    fclose(fp);

    File_Node *fn = bm.get_File(table.table_name.c_str());
    Block_Node *bn = bm.getBlockHead(fn);

    char *addressBegin = (*bn).get_Content();

    *addressBegin = Get_PrimaryKey(table.attr);
    int primary_key_record = *addressBegin;
    // cout<<"attributesize="<<sizeof(Attribute)<<endl;
    // cout<<"pr"<<Get_PrimaryKey(table.attr)<<endl;
    if (*addressBegin == -2)
        return 12;

    addressBegin += sizeof(int);
    *addressBegin = table.num_attr;
    addressBegin += sizeof(int);

    (*bn).set_UsedSize((*bn).get_UsedSize() + 2 * sizeof(int));
    (*bn).set_Dirty(true);
    for (int i = 0; i < table.num_attr; i++)
    {
        int all_size = sizeof(int) + table.attr[i].AttributeName.length() + sizeof(table.attr[i].AttributeType) + sizeof(table.attr[i].primarykey) + sizeof(table.attr[i].Unique) + sizeof(table.attr[i].withindex);
        if ((*bn).get_RemainedSize() >= all_size)
        {
            int sizename = table.attr[i].AttributeName.length();
            memcpy(addressBegin, &sizename, sizeof(int));
            addressBegin += sizeof(int);
            memcpy(addressBegin, table.attr[i].AttributeName.c_str(), sizeof(char) * table.attr[i].AttributeName.length());
            addressBegin += sizeof(char) * table.attr[i].AttributeName.length();
            memcpy(addressBegin, &(table.attr[i].AttributeType), sizeof(table.attr[i].AttributeType));
            addressBegin += sizeof(table.attr[i].AttributeType);
            memcpy(addressBegin, &(table.attr[i].primarykey), sizeof(table.attr[i].primarykey));
            addressBegin += sizeof(table.attr[i].primarykey);
            memcpy(addressBegin, &(table.attr[i].Unique), sizeof(table.attr[i].Unique));
            addressBegin += sizeof(table.attr[i].Unique);
            memcpy(addressBegin, &(table.attr[i].withindex), sizeof(table.attr[i].withindex));
            addressBegin += sizeof(table.attr[i].withindex);

            // Attribute temp_test;
            // addressBegin -= all_size;
            // int namelength = *(int *)addressBegin;
            // addressBegin += sizeof(int);

            // char *a = new char[namelength];
            // memcpy(a, addressBegin, sizeof(char) * namelength);
            // temp_test.AttributeName = a;

            // addressBegin += namelength;
            // temp_test.AttributeType = *(int *)addressBegin;
            // addressBegin += sizeof(table.attr[i].AttributeType);
            // temp_test.primarykey = *(bool *)addressBegin;
            // addressBegin += sizeof(table.attr[i].primarykey);
            // temp_test.Unique = *(bool *)addressBegin;
            // addressBegin += sizeof(table.attr[i].Unique);
            // temp_test.withindex = *(bool *)addressBegin;
            // addressBegin += sizeof(table.attr[i].withindex);

            // cout << temp_test.AttributeName << endl;
            // cout << temp_test.AttributeType << endl;
            // cout << temp_test.primarykey << endl;
            // cout << temp_test.Unique << endl;
            // cout << temp_test.withindex << endl;
            // return false;

            // addressBegin += all_size;
            (*bn).set_Dirty(true);
            (*bn).set_UsedSize((*bn).get_UsedSize() + all_size);
        }
        else
        {
            bn = bm.getNextBlock(fn, bn);
        }
    }
    if (primary_key_record != -1)
    {
        // table.attr[*addressBegin].withindex = true;
        // table.attr[*addressBegin].Unique = true;
        Create_Index index_temp;
        int pin = *(int *)addressBegin;
        index_temp.col_name = table.attr[pin].AttributeName;
        index_temp.table_name = table.table_name;
        index_temp.index_name = "Primary" + index_temp.col_name;
        CreateIndex(index_temp);
    }

    return 10;
}
int Catalog::DropTable(Drop_Table table)
{
    bool table_exist = Table_Exist(table.table_name);
    if (table_exist == false)
        return 21;
    bm.free_FileNode(table.table_name.c_str());
    vector<IndexInfo> index_temp;
    index_temp = List_AllIndex();
    int i, size = index_temp.size();
    for (i = 0; i < size; i++)
    {
        if (index_temp[i].tableName == table.table_name)
        {
            Drop_Index indexdrop;
            indexdrop.index_name = index_temp[i].indexName;
            Delete_Index(indexdrop);
        }
    }
    FILE *fp;
    fp = fopen(table.table_name.c_str(), "w+");
    fclose(fp);
    remove(table.table_name.c_str());

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
        else if (attr[i].primarykey == true && pk != -1)
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
    int i;

    addressBegin += sizeof(int);
    int size = *addressBegin;
    // cout << size << endl;
    // int k;
    // for (k = 0; k < 12; k++)
    // {
    //     size = *addressBegin;
    //     addressBegin += sizeof(int);
    //     cout << size << endl;
    // }

    addressBegin += sizeof(int);
    Attribute temp_a;
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        int namelength = *addressBegin;
        addressBegin += sizeof(int);
        // cout << namelength << endl;
        char *a = new char[namelength + 1];
        memcpy(a, addressBegin, sizeof(char) * namelength);
        a[namelength] = '\0';
        temp_a.AttributeName = a;
        addressBegin += sizeof(char) * namelength;
        temp_a.AttributeType = *(int *)addressBegin;
        addressBegin += sizeof(int);
        temp_a.primarykey = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.Unique = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.withindex = *(bool *)addressBegin;
        addressBegin += sizeof(bool);

        // cout << temp_a.AttributeName << endl;
        // cout << temp_a.AttributeType << endl;
        // cout << temp_a.primarykey << endl;
        // cout << temp_a.Unique << endl;
        // cout << temp_a.withindex << endl;

        if (temp_a.AttributeName == Attr_name && temp_a.Unique == true)
            return true;
        int usedsize = 11 + namelength;
        if (count == 0 && (*bn).get_UsedSize() == usedsize + 3 * sizeof(int) || count != 0 && (*bn).get_UsedSize() == usedsize + sizeof(int))
        {
            bn = bm.getNextBlock(fn, bn);
            count = 1;
            if ((*bn).get_UsedSize() == sizeof(size_t))
                break;
            addressBegin = (*bn).get_Content();
            usedsize = 0;
            // Attribute *a = (Attribute *)addressBegin;
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

    Attribute temp_a;
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        int namelength = *addressBegin;
        addressBegin += sizeof(int);
        // cout << namelength << endl;
        char *a = new char[namelength + 1];
        memcpy(a, addressBegin, sizeof(char) * namelength);
        a[namelength] = '\0';
        temp_a.AttributeName = a;
        addressBegin += sizeof(char) * namelength;
        temp_a.AttributeType = *(int *)addressBegin;
        addressBegin += sizeof(int);
        temp_a.primarykey = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.Unique = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.withindex = *(bool *)addressBegin;
        addressBegin += sizeof(bool);

        // cout << temp_a.AttributeName << endl;
        // cout << temp_a.AttributeType << endl;
        // cout << temp_a.primarykey << endl;
        // cout << temp_a.Unique << endl;
        // cout << temp_a.withindex << endl;

        if (temp_a.AttributeName == Attr_name)
            return true;
        int usedsize = 11 + namelength;
        if (count == 0 && (*bn).get_UsedSize() == usedsize + 3 * sizeof(int) || count != 0 && (*bn).get_UsedSize() == usedsize + sizeof(int))
        {
            bn = bm.getNextBlock(fn, bn);
            count = 1;
            if ((*bn).get_UsedSize() == sizeof(size_t))
                break;
            addressBegin = (*bn).get_Content();
            usedsize = 0;
            // Attribute *a = (Attribute *)addressBegin;
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

    Attribute temp_a;
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        int namelength = *addressBegin;
        addressBegin += sizeof(int);
        // cout << namelength << endl;
        char *a = new char[namelength + 1];
        memcpy(a, addressBegin, sizeof(char) * namelength);
        a[namelength] = '\0';
        temp_a.AttributeName = a;
        addressBegin += sizeof(char) * namelength;
        temp_a.AttributeType = *(int *)addressBegin;
        addressBegin += sizeof(int);
        temp_a.primarykey = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.Unique = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.withindex = *(bool *)addressBegin;
        addressBegin += sizeof(bool);

        // cout << temp_a.AttributeName << endl;
        // cout << temp_a.AttributeType << endl;
        // cout << temp_a.primarykey << endl;
        // cout << temp_a.Unique << endl;
        // cout << temp_a.withindex << endl;

        if (temp_a.AttributeName == AttributeName)
            return temp_a.AttributeType;
        int usedsize = 11 + namelength;
        if (count == 0 && (*bn).get_UsedSize() == usedsize + 3 * sizeof(int) || count != 0 && (*bn).get_UsedSize() == usedsize + sizeof(int))
        {
            bn = bm.getNextBlock(fn, bn);
            count = 1;
            if ((*bn).get_UsedSize() == sizeof(size_t))
                break;
            addressBegin = (*bn).get_Content();
            usedsize = 0;
            // Attribute *a = (Attribute *)addressBegin;
        }
    }
}
vector<Attribute> Catalog::List_TableAttributes(string table_name)
{
    File_Node *fn = bm.get_File(table_name.c_str());
    Block_Node *bn = bm.getBlockHead(fn);

    vector<Attribute> Attr;
    char *addressBegin = (*bn).get_Content();
    addressBegin += sizeof(int);
    int size = *addressBegin;
    addressBegin += sizeof(int);

    Attribute temp_a;
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        int namelength = *addressBegin;
        addressBegin += sizeof(int);
        // cout << namelength << endl;
        char *a = new char[namelength + 1];
        memcpy(a, addressBegin, sizeof(char) * namelength);
        a[namelength] = '\0';
        temp_a.AttributeName = a;
        addressBegin += sizeof(char) * namelength;
        temp_a.AttributeType = *(int *)addressBegin;
        addressBegin += sizeof(int);
        temp_a.primarykey = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.Unique = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.withindex = *(bool *)addressBegin;
        addressBegin += sizeof(bool);

        // cout << temp_a.AttributeName << endl;
        // cout << temp_a.AttributeType << endl;
        // cout << temp_a.primarykey << endl;
        // cout << temp_a.Unique << endl;
        // cout << temp_a.withindex << endl;
        Attr.push_back(temp_a);
        int usedsize = 11 + namelength;
        if (count == 0 && (*bn).get_UsedSize() == usedsize + 3 * sizeof(int) || count != 0 && (*bn).get_UsedSize() == usedsize + sizeof(int))
        {
            bn = bm.getNextBlock(fn, bn);
            count = 1;
            if ((*bn).get_UsedSize() == sizeof(size_t))
                break;
            addressBegin = (*bn).get_Content();
            usedsize = 0;
            // Attribute *a = (Attribute *)addressBegin;
        }
    }
    return Attr;
}

//----------------INDEX-----------------
int Catalog::CreateIndex(Create_Index index)
{
    bool cindex = Index_Exist(index.index_name);
    // cout << cindex << endl;
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
    int count = 0;
    while ((*bn).get_UsedSize() != sizeof(size_t) || count == 0)
    {
        int all_size = 4 * sizeof(int) + sizeof(char) * i.indexName.length() + sizeof(char) * i.tableName.length() + sizeof(char) * i.Attribute.length();
        if ((*bn).get_RemainedSize() >= all_size)
        {
            char *addressBegin;
            addressBegin = (*bn).get_Content() - 4 + (*bn).get_UsedSize();

            int length_index_name = i.indexName.length();
            memcpy(addressBegin, &length_index_name, sizeof(int));
            addressBegin += sizeof(int);
            memcpy(addressBegin, i.indexName.c_str(), sizeof(char) * i.indexName.length());
            addressBegin += sizeof(char) * i.indexName.length();
            int length_table_name = i.tableName.length();
            memcpy(addressBegin, &length_table_name, sizeof(int));
            addressBegin += sizeof(int);
            memcpy(addressBegin, i.tableName.c_str(), sizeof(char) * i.tableName.length());
            addressBegin += sizeof(char) * i.tableName.length();
            int length_col_name = i.Attribute.length();
            memcpy(addressBegin, &length_col_name, sizeof(int));
            addressBegin += sizeof(int);
            memcpy(addressBegin, i.Attribute.c_str(), sizeof(char) * i.Attribute.length());
            addressBegin += sizeof(char) * i.Attribute.length();
            memcpy(addressBegin, &(i.type), sizeof(int));
            addressBegin += sizeof(int);

            (*bn).set_UsedSize((*bn).get_UsedSize() + all_size);
            (*bn).set_Dirty(true);
            return this->setIndexOnAttribute(index.table_name, index.col_name, index.index_name);
        }
        else
        {
            bn = bm.getNextBlock(fn, bn);
            count = 1;
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
    int i;
    Attribute temp_a;
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        int namelength = *addressBegin;
        addressBegin += sizeof(int);
        // cout << namelength << endl;
        char *a = new char[namelength + 1];
        memcpy(a, addressBegin, sizeof(char) * namelength);
        a[namelength] = '\0';
        temp_a.AttributeName = a;
        addressBegin += sizeof(char) * namelength;
        temp_a.AttributeType = *(int *)addressBegin;
        addressBegin += sizeof(int);
        temp_a.primarykey = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.Unique = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.withindex = *(bool *)addressBegin;

        // cout << temp_a.AttributeName << endl;
        // cout << temp_a.AttributeType << endl;
        // cout << temp_a.primarykey << endl;
        // cout << temp_a.Unique << endl;
        // cout << temp_a.withindex << endl;
        if (temp_a.AttributeName == Attr_name)
        {
            *addressBegin = true;
            // addressBegin += sizeof(bool);
            (*bn).set_Dirty(true);
            return 30;
        }
        addressBegin += sizeof(bool);
        int usedsize = 11 + namelength;
        if (count == 0 && (*bn).get_UsedSize() == usedsize + 3 * sizeof(int) || count != 0 && (*bn).get_UsedSize() == usedsize + sizeof(int))
        {
            bn = bm.getNextBlock(fn, bn);
            count = 1;
            if ((*bn).get_UsedSize() == sizeof(size_t))
                break;
            addressBegin = (*bn).get_Content();
            usedsize = 0;
            // Attribute *a = (Attribute *)addressBegin;
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

    vector<IndexInfo> indexlist = List_AllIndex();
    while ((*bn).get_UsedSize() != sizeof(size_t))
    {
        (*bn).set_UsedSize(sizeof(size_t));
        (*bn).set_Dirty(true);
        bn = bm.getNextBlock(fn, bn);
    }
    int size = indexlist.size();
    int i;
    for (i = 0; i < size; i++)
    {
        if(indexlist[i].indexName!=index.index_name)
        {
            Create_Index index_temp;
            index_temp.index_name = indexlist[i].indexName;
            index_temp.col_name = indexlist[i].Attribute;
            index_temp.table_name = indexlist[i].tableName;
            CreateIndex(index_temp);
        }
        else
        {
            this->revokeIndexOnAttribute(indexlist[i].tableName, indexlist[i].Attribute);
        }
        
    }

    return 40;
}
int Catalog::revokeIndexOnAttribute(string tableName, string Attr_name)
{
    File_Node *fn = bm.get_File(tableName.c_str());
    Block_Node *bn = bm.getBlockHead(fn);

    char *addressBegin = (*bn).get_Content();
    addressBegin += sizeof(int);
    int size = *addressBegin;
    addressBegin += sizeof(int);
    int i;
    Attribute temp_a;
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        int namelength = *addressBegin;
        addressBegin += sizeof(int);
        // cout << namelength << endl;
        char *a = new char[namelength + 1];
        memcpy(a, addressBegin, sizeof(char) * namelength);
        a[namelength] = '\0';
        temp_a.AttributeName = a;
        addressBegin += sizeof(char) * namelength;
        temp_a.AttributeType = *(int *)addressBegin;
        addressBegin += sizeof(int);
        temp_a.primarykey = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.Unique = *(bool *)addressBegin;
        addressBegin += sizeof(bool);
        temp_a.withindex = *(bool *)addressBegin;

        // cout << temp_a.AttributeName << endl;
        // cout << temp_a.AttributeType << endl;
        // cout << temp_a.primarykey << endl;
        // cout << temp_a.Unique << endl;
        // cout << temp_a.withindex << endl;
        if (temp_a.AttributeName == Attr_name)
        {
            *addressBegin = false;
            // addressBegin += sizeof(bool);
            (*bn).set_Dirty(true);
            return 30;
        }
        addressBegin += sizeof(bool);
        int usedsize = 11 + namelength;
        if (count == 0 && (*bn).get_UsedSize() == usedsize + 3 * sizeof(int) || count != 0 && (*bn).get_UsedSize() == usedsize + sizeof(int))
        {
            bn = bm.getNextBlock(fn, bn);
            count = 1;
            if ((*bn).get_UsedSize() == sizeof(size_t))
                break;
            addressBegin = (*bn).get_Content();
            usedsize = 0;
            // Attribute *a = (Attribute *)addressBegin;
        }
    }
}

bool Catalog::Index_Exist(string indexName)
{
    File_Node *fn = bm.get_File("Indexs");
    Block_Node *bn = bm.getBlockHead(fn);
    while ((*bn).get_UsedSize() != sizeof(size_t))
    {
        int count = 0;
        char *addressBegin;
        addressBegin = (*bn).get_Content();
        IndexInfo i;
        int sizecount = 4;
        // cout << (*bn).get_UsedSize() << endl;

        while ((*bn).get_UsedSize() > sizecount)
        {
            int length_indexname = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *a = new char[length_indexname + 1];
            memcpy(a, addressBegin, sizeof(char) * length_indexname);
            a[length_indexname] = '\0';
            i.indexName = a;
            addressBegin += sizeof(char) * length_indexname;

            int length_tablename = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *b = new char[length_tablename + 1];
            memcpy(b, addressBegin, sizeof(char) * length_tablename);
            b[length_tablename] = '\0';
            i.tableName = b;
            addressBegin += sizeof(char) * length_tablename;

            int length_colname = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *c = new char[length_colname + 1];
            memcpy(c, addressBegin, sizeof(char) * length_colname);
            c[length_colname] = '\0';
            i.Attribute = c;
            addressBegin += sizeof(char) * length_colname;

            i.type = *(int *)addressBegin;
            addressBegin += sizeof(int);

            int all_size = 4 * sizeof(int) + sizeof(char) * i.indexName.length() + sizeof(char) * i.tableName.length() + sizeof(char) * i.Attribute.length();
            sizecount += all_size;
            if (i.indexName == indexName)
            {
                return true;
            }
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
    while ((*bn).get_UsedSize() != sizeof(size_t))
    {
        int count = 0;
        char *addressBegin;
        addressBegin = (*bn).get_Content();
        IndexInfo i;
        int sizecount = 4;
        // cout << (*bn).get_UsedSize() << endl;

        while ((*bn).get_UsedSize() > sizecount)
        {
            int length_indexname = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *a = new char[length_indexname + 1];
            memcpy(a, addressBegin, sizeof(char) * length_indexname);
            a[length_indexname] = '\0';
            i.indexName = a;
            addressBegin += sizeof(char) * length_indexname;

            int length_tablename = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *b = new char[length_tablename + 1];
            memcpy(b, addressBegin, sizeof(char) * length_tablename);
            b[length_tablename] = '\0';
            i.tableName = b;
            addressBegin += sizeof(char) * length_tablename;

            int length_colname = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *c = new char[length_colname + 1];
            memcpy(c, addressBegin, sizeof(char) * length_colname);
            c[length_colname] = '\0';
            i.Attribute = c;
            addressBegin += sizeof(char) * length_colname;

            i.type = *(int *)addressBegin;
            addressBegin += sizeof(int);

            int all_size = 4 * sizeof(int) + sizeof(char) * i.indexName.length() + sizeof(char) * i.tableName.length() + sizeof(char) * i.Attribute.length();
            sizecount += all_size;

            indexall.push_back(i);
        }
        bn = bm.getNextBlock(fn, bn);
    }
    return indexall;
}

vector<IndexInfo> Catalog::List_Table_AllIndex(string tableName)
{
    File_Node *fn = bm.get_File("Indexs");
    Block_Node *bn = bm.getBlockHead(fn);

    vector<IndexInfo> indexall;

    while ((*bn).get_UsedSize() != sizeof(size_t))
    {
        int count = 0;
        char *addressBegin;
        addressBegin = (*bn).get_Content();
        IndexInfo i;
        int sizecount = 4;
        // cout << (*bn).get_UsedSize() << endl;

        while ((*bn).get_UsedSize() > sizecount)
        {
            int length_indexname = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *a = new char[length_indexname + 1];
            memcpy(a, addressBegin, sizeof(char) * length_indexname);
            a[length_indexname] = '\0';
            i.indexName = a;
            addressBegin += sizeof(char) * length_indexname;

            int length_tablename = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *b = new char[length_tablename + 1];
            memcpy(b, addressBegin, sizeof(char) * length_tablename);
            b[length_tablename] = '\0';
            i.tableName = b;
            addressBegin += sizeof(char) * length_tablename;

            int length_colname = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *c = new char[length_colname + 1];
            memcpy(c, addressBegin, sizeof(char) * length_colname);
            c[length_colname] = '\0';
            i.Attribute = c;
            addressBegin += sizeof(char) * length_colname;

            i.type = *(int *)addressBegin;
            addressBegin += sizeof(int);

            int all_size = 4 * sizeof(int) + sizeof(char) * i.indexName.length() + sizeof(char) * i.tableName.length() + sizeof(char) * i.Attribute.length();
            sizecount += all_size;
            if (i.tableName == tableName)
                indexall.push_back(i);
        }
        bn = bm.getNextBlock(fn, bn);
    }

    return indexall;
}

IndexInfo Catalog::Find_Index(string indexName)
{
    File_Node *fn = bm.get_File("Indexs");
    Block_Node *bn = bm.getBlockHead(fn);

    while ((*bn).get_UsedSize() != sizeof(size_t))
    {
        int count = 0;
        char *addressBegin;
        addressBegin = (*bn).get_Content();
        IndexInfo i;
        int sizecount = 4;

        while ((*bn).get_UsedSize() > sizecount)
        {
            int length_indexname = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *a = new char[length_indexname + 1];
            memcpy(a, addressBegin, sizeof(char) * length_indexname);
            a[length_indexname] = '\0';
            i.indexName = a;
            addressBegin += sizeof(char) * length_indexname;

            int length_tablename = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *b = new char[length_tablename + 1];
            memcpy(b, addressBegin, sizeof(char) * length_tablename);
            b[length_tablename] = '\0';
            i.tableName = b;
            addressBegin += sizeof(char) * length_tablename;

            int length_colname = *(int *)addressBegin;
            addressBegin += sizeof(int);
            char *c = new char[length_colname + 1];
            memcpy(c, addressBegin, sizeof(char) * length_colname);
            c[length_colname] = '\0';
            i.Attribute = c;
            addressBegin += sizeof(char) * length_colname;

            i.type = *(int *)addressBegin;
            addressBegin += sizeof(int);

            int all_size = 4 * sizeof(int) + sizeof(char) * i.indexName.length() + sizeof(char) * i.tableName.length() + sizeof(char) * i.Attribute.length();
            sizecount += all_size;
            if (i.indexName == indexName)
                return i;
        }
        bn = bm.getNextBlock(fn, bn);
    }
}
