#ifndef RECORD_H
#define RECORD_H
#include "BufferManager.h"
#include "Catalog.h"
#include "Basic.h"
#include <fstream>
#include <sstream>
#include "IndexManager.h"
using namespace std;
class API;

class RecordManager{
public:

	Select_Res res;
	vector<string>Table;
	BufferManager bm;
	Catalog cm;
	API * api;
	IndexManager im;
	void res_pop()
	{
		res.res.clear();
		res.num_tuple = 0;
	}
	int Create_table(string tableName);
	int Drop_table(string tableName);

	int Insert_record(Insert record);

	int select_record(Select record);
	int block_select_record(Select record, int blockOffset);


	int delete_record(Delete record);
	int delete_block_record(Delete record, int blockOffset);


	string Get_table_file(string tableName);
	RecordManager() { res.res.clear();}
	~RecordManager() {}

private:

	//IndexManager im;
	int block_select(Select record, Block_Node* block);
	int block_delete(Delete record, Block_Node* block);

	bool record_match(char * recordBegin, int recordSize, vector<Attribute> * attributeVector, vector<Condition>* conditionVector);
	bool content_match(char * content, int type, Condition* condition);

	void record_save(string table_name, char * recordBegin, int recordSize);
	bool Table_Exist(string table_name)
	{
		return cm.Table_Exist(table_name);
	}
	bool checkattributetype(string table_name, vector<string>values, vector<int>types, int states)//state 0 insert; place for cond attr in table
	{
		vector<Attribute> tmp = cm.List_TableAttributes(table_name);
		if (states == 0)
		{
			if (cm.Check_AttributeNum(table_name, values.size()) == false) return false;
			for (int i = 0; i < values.size(); i++)
			{
				if (types[i] != tmp[i].AttributeType)
					return false;
			}
			return true;
		}
		else
		{
			for (int i = 0; i < types.size(); i++)
			{
				if (types[i] == tmp[states].AttributeType) return true;
			}
			return false;
		}

		
	}
	int recordSizeGet(string table_name)
	{
		vector<Attribute>tmp;
		int size = 0;
		tmp = cm.List_TableAttributes(table_name);
		for (int i = 0; i < tmp.size(); i++)
		{
			size += typeSizeGet(tmp[i].AttributeType);
		}
		return size;
	}
	vector<Attribute>attributeGet(string table_name)
	{
		vector<Attribute>tmp;
		tmp = cm.List_TableAttributes(table_name);
		return tmp;
	}
	int  typeSizeGet(int type)
	{
		int size;
		switch (type)
		{
		case -1: size = sizeof(int); break;
		case 0: size = sizeof(float); break;
		default: size = sizeof(char) * type; break;

		}
		return size;
	}
};
#endif // !RECORD_H

