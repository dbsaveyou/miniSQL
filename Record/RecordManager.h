#ifndef RECORD_H
#define RECORD_H
#include "BufferManager.h"
#include "Basic.h"
#include "API.h"
using namespace std;
class API;

class RecordManager{
public:
	RecordManager() {}
	BufferManager bm;
	API * api;
	vector <char *> data_vec;//用于存储select的返回值

	int Create_table(string tableName);
	int Drop_table(string tableName);

	int Insert_record(Insert record);

	int select_record(Select record);
	int block_select_record(Select record, int blockOffset);


	int delete_record(Delete record);
	int delete_block_record(Delete record, int blockOffset);


	string Get_table_file(string tableName);
	string Get_index_file(string indexName);

	void record_save(char * recordBegin, int recordSize);

private:
	int block_select(Select record, Block_Node* block);
	int block_delete(Delete record, Block_Node* block);

	bool record_match(char * recordBegin, int recordSize, vector<Attribute> * attributeVector, vector<Condition>* conditionVector);
	bool content_match(char * content, int type, Condition* condition);
};
#endif // !RECORD_H

