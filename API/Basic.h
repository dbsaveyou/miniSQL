#ifndef TRANS_CLASS_H  
#define TRANS_CLASS_H
#include<vector>
#include<string>
class Attribute{
    public:
        Attribute();
        ~Attribute();

        string AttributeName;
        int AttributeType; //-1->int,0->float,1~255->char
        bool Unique;//true->unique
        bool primarykey;//true->pk
        bool withindex;//true->index

};
class Table{
    public:
        Table();
        ~Table();

        string TableName;
        vector<Attribute> Attr;
        bool primarykey;
        int primarykeyLocation;

};
class Condition
{
    public:
        string col;
        int op;   // = 1 <> 2 > 3 < 4 <=5 >=6
        string value;  // include '
        int type;
};
// Cmd Here
class Create_Table
{   
    public:
        string table_name;
        int num_attr;   // num of attribute
        vector<Attribute> attr; 
};
class Drop_Table
{
    public:
        string table_name;
};
class Create_Index
{
    public:
        string index_name;
        string table_name;
        string col_name;
};
class Drop_Index
{
    public:
        string index_name;
};
class Insert
{
    public:
        string table_name;
        int num_vaule;
        vector<string> values;
        vector<int> types;
};
class Select
{
    public:
        string table_name;
        int num_cond; // num of condition
        vector<Condition> cond;
};
class Delete 
{
    public:
        string table_name;
        int num_cond;
        vector<Condition> cond;
};
class Trans_in
{
    public:
        int id;
        Create_Table *create_table;     // id=0
        Drop_Table *drop_table;         // id=1
        Create_Index *create_index;     // id=2
        Drop_Index *drop_index;         // id=3
        Insert *insert;                 // id=4
        Select *select;                 // id=5
        Delete *deletee;                // id=6
        Trans_in(int code=-1):id(code){}
        ~Trans_in(){}
}
class Select_Res
{
    public:
        int num_attr;
        vector<Attribute> attr;
        int num_tuple;
        vector< vector<string> > res;
        // use push_back to add 
}
#endif
