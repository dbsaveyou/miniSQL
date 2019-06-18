#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdio>
#include "Catalog.h"
#include "Basic.h"

using namespace std;
int main()
{
/*   
    // -------------------------table--------------
    cout<<"-------------------TEST TABLE-------------------"<<endl;
    Catalog Catalog_test;
    Create_Table table_test;
    table_test.table_name = "student";
    table_test.num_attr = 2;
    Attribute att_temp;
    att_temp.AttributeName = "stuid";
    att_temp.AttributeType = 5;
    att_temp.primarykey = true;
    att_temp.Unique = true;
    att_temp.withindex = true;
    table_test.attr.push_back(att_temp);
    att_temp.AttributeName = "name";
    att_temp.AttributeType = 4;
    att_temp.primarykey = false;
    att_temp.Unique = false;
    att_temp.withindex = false;
    table_test.attr.push_back(att_temp);
    // create table and success
    int test = Catalog_test.CreateTable(table_test);
    cout << "Create a table named 'student' and success, the return value: " << test << endl;
    //check table exist
    bool exist = Catalog_test.Table_Exist("student");
    cout << "Check table named 'student' exists, the return value: " << exist << endl;
    //create table with the existed name
    test = Catalog_test.CreateTable(table_test);
    cout << "Create a table named 'student'(exist name) and failed, the return value: " << test << endl;
    //drop table existed
    Drop_Table droptable_test;
    droptable_test.table_name = "student";
    test = Catalog_test.DropTable(droptable_test);
    cout << "Drop table named 'student'(exist name) and success, the return value: " << test << endl;
    //drop table non-existed
    test = Catalog_test.DropTable(droptable_test);
    cout << "Drop table named 'student'(non-exist name) and failed, the return value: " << test << endl;
    //create table with two attribute using the same name
    table_test.attr.pop_back();
    att_temp.AttributeName = "stuid";
    table_test.attr.push_back(att_temp);
    test = Catalog_test.CreateTable(table_test);
    cout << "Create table named 'student' with two attribute using the same name and failed, the return value: " << test << endl;
    //create table with more than one primary key
    table_test.attr.pop_back();
    table_test.attr.pop_back();
    att_temp.AttributeName = "stuid";
    att_temp.AttributeType = 5;
    att_temp.primarykey = true;
    att_temp.Unique = false;
    att_temp.withindex = false;
    table_test.attr.push_back(att_temp);
    att_temp.AttributeName = "name";
    att_temp.AttributeType = 4;
    att_temp.primarykey = true;
    att_temp.Unique = false;
    att_temp.withindex = false;
    table_test.attr.push_back(att_temp);
    test = Catalog_test.CreateTable(table_test);
    cout << "Create table named 'student' with more than one primary key and failed, the return value: " << test << endl;
    table_test.attr.pop_back();
    table_test.attr.pop_back();
*/

/*
    //----------------ATTRIBUTE-----------------
    // cout<<"-------------------TEST ATTRIBUTE-------------------"<<endl;
    Catalog Catalog_test;
    Create_Table table_test;
    Attribute att_temp;
    table_test.table_name = "student";
    table_test.num_attr = 3;

    att_temp.AttributeName = "stuid";
    att_temp.AttributeType = 10;
    att_temp.primarykey = true;
    att_temp.Unique = true;
    att_temp.withindex = true;
    table_test.attr.push_back(att_temp);
    att_temp.AttributeName = "name";
    att_temp.AttributeType = 15;
    att_temp.primarykey = false;
    att_temp.Unique = true;
    att_temp.withindex = false;
    table_test.attr.push_back(att_temp);
    att_temp.AttributeName = "age";
    att_temp.AttributeType = -1;
    att_temp.primarykey = false;
    att_temp.Unique = false;
    att_temp.withindex = false;
    table_test.attr.push_back(att_temp);
    Catalog_test.CreateTable(table_test);
    //get primary key
    int test = Catalog_test.Get_PrimaryKey(table_test.attr);
    cout << "Get primary key(return index) and success, the return value: " << test << endl;
    //check attribute unique (pass)
    bool unique = Catalog_test.CheckAttributeUnique("student", "name");
    cout << "Check attribute(name) unique and return true, the return value: " << unique << endl;
    unique = Catalog_test.CheckAttributeUnique("student", "age");
    cout << "Check attribute(age) unique and return false, the return value: " << unique << endl;
    //check attribute num (pass)
    bool num = Catalog_test.Check_AttributeNum("student", 3);
    cout << "Check attribute num and return true, the return value: " << num << endl;
    num = Catalog_test.Check_AttributeNum("student", 2);
    cout << "Check attribute num and return false, the return value: " << num << endl;
    //check attribute exist (pass)
    bool exist = Catalog_test.Attribute_Exist("student", "age");
    cout << "Check attribute(age) exist and return true, the return value: " << exist << endl;
    exist= Catalog_test.Attribute_Exist("student", "age_");
    cout << "Check attribute(age_) exist and return false, the return value: " << exist << endl;
    //get attribute type (pass)
    test = Catalog_test.getAttributeType("age", "student");
    cout << "Check attribute(age) type and return true, the return value: " << test << endl;
    test = Catalog_test.getAttributeType("name", "student");
    cout << "Check attribute(name) type and return true, the return value: " << test << endl;
    //List attribute (pass)
    vector<Attribute> listattr_temp;
    listattr_temp = Catalog_test.List_TableAttributes("student");
    cout << "List attribute in table, the return value: " << endl;
    int size = listattr_temp.size();
    int i;
    for (i = 0; i < size; i++)
    {
        cout << "Attribute No." << i << endl;
        cout << "Attributename = " << listattr_temp[i].AttributeName << endl;
        cout << "Attributetype = " << listattr_temp[i].AttributeType << endl;
        cout << "Primarykey = " << listattr_temp[i].primarykey << endl;
        cout << "Unique = " << listattr_temp[i].Unique << endl;
        cout << "index = " << listattr_temp[i].withindex << endl;
    }
*/

/*
    //----------------INDEX-----------------
    Catalog Catalog_test;
    Create_Table table_test;
    Attribute att_temp;
    table_test.table_name = "student";
    table_test.num_attr = 3;

    att_temp.AttributeName = "stuid";
    att_temp.AttributeType = 10;
    att_temp.primarykey = true;
    att_temp.Unique = true;
    att_temp.withindex = true;
    table_test.attr.push_back(att_temp);
    att_temp.AttributeName = "name";
    att_temp.AttributeType = 15;
    att_temp.primarykey = false;
    att_temp.Unique = true;
    att_temp.withindex = false;
    table_test.attr.push_back(att_temp);
    att_temp.AttributeName = "age";
    att_temp.AttributeType = -1;
    att_temp.primarykey = false;
    att_temp.Unique = false;
    att_temp.withindex = false;
    table_test.attr.push_back(att_temp);
    Catalog_test.CreateTable(table_test);
    // check index exist (pass)
    bool exist = Catalog_test.Index_Exist("Primarystuid");
    cout << "Check index(Primarystuid) exist and return true, the return value: " << exist << endl;
    exist = Catalog_test.Index_Exist("Primaryname");
    cout << "Check index(Primaryname) exist and return false, the return value: " << exist << endl;
    //create index (pass)
    //success
    Create_Index index_temp;
    index_temp.table_name = "student";
    index_temp.index_name = "nameindex";
    index_temp.col_name = "name";
    int test = Catalog_test.CreateIndex(index_temp);
    cout << "Create index(nameindex) on name and success, the return value: " << test << endl;

    //non-unique (pass)
    index_temp.index_name = "ageindex";
    index_temp.col_name = "age";
    test = Catalog_test.CreateIndex(index_temp);
    cout << "Create index(ageindex) on name(non-unique) and failed, the return value: " << test << endl;
    //index exist (pass)
    index_temp.index_name = "nameindex";
    index_temp.col_name = "name";
    test = Catalog_test.CreateIndex(index_temp);
    cout << "Create index(nameindex) on name(index existed) and failed, the return value: " << test << endl;

    // list all index (pass)
    vector<IndexInfo> index_list;
    index_list = Catalog_test.List_AllIndex();
    int size = index_list.size();
    int i;
    for (i = 0; i < size; i++)
    {
        cout << "Index No. = " << i << endl;
        cout << "Index tablename = " << index_list[i].tableName << endl;
        cout << "Index attributename = " << index_list[i].Attribute << endl;
        cout << "Index indexname = " << index_list[i].indexName << endl;
        cout << "Index attributetype = " << index_list[i].type << endl;
    }
    //list table index (pass)
    vector<IndexInfo> index_list_table;
    index_list_table = Catalog_test.List_Table_AllIndex("student");
    size = index_list.size();
    i;
    for (i = 0; i < size; i++)
    {
        cout << "Index No. = " << i << endl;
        cout << "Index tablename = " << index_list_table[i].tableName << endl;
        cout << "Index attributename = " << index_list_table[i].Attribute << endl;
        cout << "Index indexname = " << index_list_table[i].indexName << endl;
        cout << "Index attributetype = " << index_list_table[i].type << endl;
    }
    //find index (pass)
    IndexInfo index_find;
    index_find = Catalog_test.Find_Index("nameindex");
    cout << "Index No. = " << i << endl;
    cout << "Index tablename = " << index_find.tableName << endl;
    cout << "Index attributename = " << index_find.Attribute << endl;
    cout << "Index indexname = " << index_find.indexName << endl;
    cout << "Index attributetype = " << index_find.type << endl;

    //drop index (pass)
    Drop_Index dindex_temp;

    dindex_temp.index_name = "Primarystuid";
    test = Catalog_test.Delete_Index(dindex_temp);
    cout << "Drop index(Primarystuid) on stuid and success, the return value: " << test << endl;

    dindex_temp.index_name = "ageindex";
    test = Catalog_test.Delete_Index(dindex_temp);
    cout << "Drop index(ageindex non-exist) on age and failed, the return value: " << test << endl;
*/

    return 0;
}
