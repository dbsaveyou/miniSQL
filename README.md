# MiniSQL README

### 1 编译

* Linux、Mac以及unix平台下，直接使用提供的makefile编译即可
* WIN10 平台：Visual Studio 2017: Debug X86 ，各文件之间的依赖关系请参照makefile

### 2 磁盘文件定义

<table>
   <tr>
      <td>操作模块</td>
      <td>类型</td>
      <td>格式</td>
      <td>存储方式</td>
   </tr>
   <tr>
      <td rowspan = "2">CATALOG</td>
      <td>表信息文件</td>
      <td>表名，属性信息</td>
      <td rowspan = "4">二进制文件</td>
   </tr>
   <tr>
      <td>Indexs信息文件</td>
      <td>INDEX信息（所在表，索引名，属性名）</td>
   </tr>
   <tr>
      <td>RECORD</td>
      <td>表内容文件</td>
      <td>每个属性的值</td>
   </tr>
   <tr>
      <td>INDEX</td>
      <td>Index内容文件</td>
      <td>key值和偏移量值</td>
   </tr>
</table>

### 3 测试方式

​	**MiniSQL**支持标准的**SQL**语句格式，每一条**SQL**语句以分号结尾，一条**SQL**语句可写在一行或多行。为简化编程，要求所有的关键字都为小写。在以下语句的语法说明中，用黑体显示的部分表示语句中的原始字符串，如**create**就严格的表示字符串**“create”**，否则含有特殊的含义，如 表名 并不是表示字符串 “表名”，而是表示表的名称。

**创建表语句**

该语句的语法如下：

```sql
create table 表名 (
   列名 类型 ,
   列名 类型 ,
   列名 类型 ,
   primary key ( 列名 )
);
```

其中类型的说明见总体设计

若该语句执行成功，则输出执行成功信息；若失败，必须告诉用户失败的原因。

示例语句：
```sql
create table student (
	sno char(8),
	sname char(16) unique,
	sage int,
	sgender char (1),
	primary key ( sno )
);
```
**删除表语句**

该语句的语法如下：

```sq
drop table 表名 ;
```

若该语句执行成功，则输出执行成功信息；若失败，必须告诉用户失败的原因。

示例语句：

```sq
drop table student;
```

**创建索引语句**

该语句的语法如下：

```sq
create index 索引名 on 表名 ( 列名 );
```

若该语句执行成功，则输出执行成功信息；若失败，必须告诉用户失败的原因。

示例语句：

```sq
create index stunameidx on student ( sname );
```

**删除索引语句**

该语句的语法如下：

```sq
drop index 索引名 ;
```

若该语句执行成功，则输出执行成功信息；若失败，必须告诉用户失败的原因。

示例语句：

```sq
drop index stunameidx;
```

**选择语句**

该语句的语法如下：

```sq
select * from 表名 ;
```

或：

```sq
select * from 表名 where 条件 ;
```

其中“条件”具有以下格式：列 **op** 值 **and** 列 **op** 值 … **and** 列 **op** 值。

op是算术比较符：=   <>     <       >       <=     >=

若该语句执行成功且查询结果不为空，则按行输出查询结果，第一行为属性名，其余每一行表示一条记录；若查询结果为空，则输出信息告诉用户查询结果为空；若失败，必须告诉用户失败的原因。

示例语句：

```sq
select * from student;
select * from student where sno = ‘88888888’;
select * from student where sage > 20 and sgender = ‘F’;
```

**插入记录语句**

该语句的语法如下：

```sql
insert into 表名 values ( 值1 , 值2 , … , 值n );
```

若该语句执行成功，则输出执行成功信息；若失败，必须告诉用户失败的原因。

示例语句：

```sq
insert into student values (‘12345678’,’wy’,22,’M’);
```

**删除记录语句**

该语句的语法如下：

```sq
delete from 表名 ;
```

或：

```sq
delete from 表名 where 条件 ;
```

若该语句执行成功，则输出执行成功信息，其中包括删除的记录数；若失败，必须告诉用户失败的原因。

示例语句：

```sq
delete from student;
delete from student where sno = ‘88888888’;
```

**退出MiniSQL系统语句**

该语句的语法如下：

```sq
quit;
```

**执行SQL脚本文件语句**

该语句的语法如下：

```sql
execfile 文件名 ;
```



​	**SQL**脚本文件中可以包含任意多条上述8种**SQL**语句，**MiniSQL**系统读入该文件，然后按序依次逐条执行脚本中的**SQL**语句。