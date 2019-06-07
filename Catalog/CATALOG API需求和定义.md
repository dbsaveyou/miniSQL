# CATALOG API需求和定义

已经定义的buffer接口可以实现CATALOG全部功能

## CATALOG对外提供接口

*//TABLE*

建表时，**先判断primary key，存在则自动创建pk的索引**

**判断表是否存在：**

```c++
bool Table_Exist(string tableName);
    /*TRUE EXIST, FALSE NON-EXISTED */
```



*//ATTRIBUTE*

可能index需要用到：主键位置判断，属性unique判断

```c++
int Get_PrimaryKey(vector<Attribute> attr);
/*返回pk的index，不存在pk返回-1，多个pk返回-2*/
bool CheckAttributeUnique(string tableName, string Attr_name);
/*true-unique, false not unique*/

```
record插入需要用到：属性总数

```c++
bool Check_AttributeNum(string tableName, int AttributeNum);
/*总数符合true，不符合false（用于判断插入数据的属性值数量是否一样）*/
```
record插入需要用到：属性是否存在

```c++
bool Attribute_Exist(string tableName,  string Attr_name);     
/*存在该attr-true*/
```
返回属性类型和列出所有属性：

```c++
int getAttributeType(string AttributeName, string tableName);
/*返回type定义与attr类定义一致*/
vector<Attribute> List_TableAttributes(string table_name);
/*列出表中所有attr*/
```



*//INDEX*

创建删除索引：

```c++
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
```

INDEX需要：索引是否存在，返回需要查找的索引信息，列出所有索引
```c++
bool Index_Exist(string indexName);
    /*判断index是否存在 true-存在*/
IndexInfo Find_Index(string indexName);
    /*返回index信息，格式参考indexinfo*/
vector<IndexInfo> List_AllIndex();
    /*列出所有index*/
```




