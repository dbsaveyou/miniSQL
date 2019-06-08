### Record Manager部分提供的接口以及API调用

#### ***提供的接口**

* 创建/删除数据表文件

  ```c++
  int Create_table(string tableName);
  int Drop_table(string tableName);
  ```

  >表文件创建：先检查表是否已经存在，不存在则创建文件
  >
  >· 文件打开失败，返回0
  >
  >· 成功创建打开并关闭，返回10
  >
  >· 表已经存在，返回11

  >表文件删除：先检查表是否存在，存在则删除
  >
  >· 文件删除失败，返回-1
  >
  >·成功删除，返回 20
  >
  >· 表不存在，返回21

* 记录的增删改

  ```c++
  int Insert_record(Insert record);
  
  int select_record(Select record);
  int block_select_record(Select record, int blockOffset);
  //提供blockoffset接口给API用于判断索引存在的查找和删除
  
  int delete_record(Delete record);
  int delete_block_record(Delete record, int blockOffset);
  ```

  >插入：先判断表是否存在，再判断插入属性的合法性。
  >
  >//将表中的一条记录插入block中
  >
  >· 插入成功，返回50
  >
  >· 插入失败，变量名不匹配，返回52
  >
  >· 表不存在，返回51

  >删除：先判断表是否存在，再判断删除条件属性的合法性。
  >
  >· 删除成功，返回70
  >
  >· 删除失败，变量名不匹配，返回72
  >
  >· 表不存在，返回71

  >查询：先判断表是否存在，再判断查询条件属性的合法性。
  >
  >· 查询成功，返回60
  >
  >· 删除失败，变量名不匹配，返回62
  >
  >· 表不存在，返回61
  >
  >注意：查询结果将被存入 vector<char *>data_vec中

* 获取数据表的名字

  ```c++
  string Get_table_file(string tableName);
  ```

  >能够根据表名得到，表所在的文件名

* 返回select查询后得到的数据的向量

  ```c++
  vector <char *> data_vec;
  ```

  >返回指向每一条字符串的指针向量集

#### ***API调用**

```c++
Table_Exist(tableName);
    
checkattributetype(string table_name, vector<string>values, vector<int>types);
    
recordSizeGet(string table_name);

attributeGet(string table_name, vector<Attribute> * attributeVector);

typeSizeGet(int type);
```

>1. 用于判断表是否存在的函数。参数：表名
>
>2. 用于判断属性合法化的函数，包含检查属性数量是否一致，属性值与属性类型是否对应。参数：表名，属性名向量，属性类型向量
>3. 得到对应表的记录长度的函数。参数：表名
>4. 得到对应表的属性向量的函数。参数：表名，属性向量指针（作为返回值）
>5. 根据类型得到类型大小的函数。参数：类型的数字