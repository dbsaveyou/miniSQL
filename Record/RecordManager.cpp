#define _CRT_SECURE_NO_WARNINGS
#include"RecordManager.h"
#define BlockSize 4096
int RecordManager::Create_table(string tableName)
{
	string File_name = Get_table_file(tableName);//get file name, but may change or delete  if the file name is konwn
	FILE * f;
	int flag = 0;
	//for (int i = 0; i < Table.size(); i++)
	//{
	//	if (tableName == Table[i])
	//	{
	//		flag = 1;
	//		break;
	//	}
	//}
	if (api->cm->Table_Exist(tableName) == false)
	{
		f = fopen(File_name.c_str(), "w+");//open for test 
		if (f == NULL)
		{
			return 11;//add 0 for open file failed
		}
		fclose(f);
		//Table.push_back(tableName);
		return 10;//success
	}
	return 11;
}
int RecordManager::Drop_table(string tableName)
{
	string File_name = Get_table_file(tableName);//get file name
	int flag = 0;
	int index = 0;
	//for (int i = 0; i < Table.size(); i++)
	//{
	//	if (tableName == Table[i])
	//	{
	//		flag = 1;
	//		index = i;
	//		break;
	//	}
	//}
	if (api->cm->Table_Exist(tableName) == true)
	{
		if (remove(File_name.c_str()) != 0)
		{
			return 21;//add -1 for remove file failed
		}
		//Table.erase(Table.begin() + index);
		return 20;
	}

	return 20;
}

int RecordManager::Insert_record(Insert record)
{

	//find the attribute that with index
	vector<Attribute> attr = api->cm->List_TableAttributes(record.table_name);

	Attribute index_attr;

	int count;
	vector<int> type;
	vector<string> key;
	vector<string> filepaths;
	string indexnamelist[100];
	count = 0;
	vector<IndexInfo> index_temp = api->cm->List_Table_AllIndex(record.table_name);
	int j;
	for (int i = 0; i < attr.size(); i++)
	{
		for (j = 0; j < index_temp.size(); j++)
		{
			if (index_temp[j].Attribute == attr[i].AttributeName)
				indexnamelist[i] = index_temp[j].indexName;
		}
	}

	for (int i = 0; i < attr.size(); i++)
	{
		if (attr[i].withindex == true)
		{
			type.push_back(attr[i].AttributeType);
			key.push_back(record.values[i]);
			string damnfilepath;
			damnfilepath += record.table_name;
			damnfilepath += "_";
			damnfilepath += indexnamelist[i];
			filepaths.push_back(damnfilepath);
			count++;
		}
	}
	//cm->Check_AttributeNum("test", 3);
	if (Table_Exist(record.table_name) == true)//test if the insert table exist
	{
		//cm->Check_AttributeNum("test", 3);
		if (checkattributetype(record.table_name, record.values, record.types, -1) == true)//test for the record's attribute
		{
			File_Node * file_temp = bm.get_File(Get_table_file(record.table_name).c_str());//get the table file
			Block_Node * block_temp = bm.getBlockHead(file_temp);//get the block head
			//cout << (int)file_temp << " " << (int)block_temp << endl;
			char * addr_begin = NULL;//define the begin address of the insert place
			int record_size = 0;

			//caculate the record size and get record attribute name for insert into the table
			for (int i = 0; i < record.types.size(); i++)
			{
				record_size += typeSizeGet(attr[i].AttributeType);
			}

			while (true)
			{
				if (block_temp == NULL) return -1;
				if (record_size <= block_temp->get_RemainedSize())//if the record is enough for the remain block
				{
					addr_begin = block_temp->get_Address() + block_temp->get_UsedSize();//move the ptr to the end of the content in the block



					for (int i = 0; i < count; i++)
					{

						//api->im->insertIndex(filepaths[i], key[i], block_temp->get_OffNum() * BlockSize + (addr_begin - block_temp->get_Address()), type[i]);
					}

					for (int i = 0; i < record.num_value; i++)
					{
						if (record.types[i] > 0)
						{
							if (record.values[i].size() > attr[i].AttributeType) return 52;
							memcpy(addr_begin, record.values[i].c_str(), record.values[i].size());
							addr_begin += typeSizeGet(attr[i].AttributeType);
						}
						else if (record.types[i] == 0)
						{
							float num = atof(record.values[i].c_str());
							memcpy(addr_begin, &num, sizeof(float));
							addr_begin += sizeof(float);
						}
						else if (record.types[i] == -1)
						{
							stringstream ss;
							int num;
							ss << record.values[i];
							ss >> num;

							memcpy(addr_begin, &num, sizeof(int));
							addr_begin += sizeof(int);
						}
					}
					//buffer setting
					block_temp->set_UsedSize(block_temp->get_UsedSize() + record_size);//update the used size
					block_temp->set_Dirty(true);//set dirty for the change in the block, after it is written in the file set dirty = false. 
					return 50;//Success
				}
				else
				{
					block_temp = bm.getNextBlock(file_temp, block_temp);//get next block place
				}
			}
			return 50;//success
		}
		else
		{
			return 52;
			//num of values doesn't match
		}

	}
	else
	{
		return 51;//can't find table
	}

}
//
int RecordManager::select_record(Select record)
{
	vector<Attribute> attributeVector;
	attributeVector = attributeGet(record.table_name);//get select attribute imformatiom
	int flag = 0;
	int states = 0;
	for (int i = 0; i < record.cond.size(); i++)
	{
		for (int j = 0; j < attributeVector.size(); j++)
		{
			if (record.cond[i].col == attributeVector[j].AttributeName)
			{
				states = j;
				if (attributeVector[j].Unique == true)
				{
					flag = 1;

					break;
				}
			}
		}
		if (flag == 1) break;
	}

	//if (record.num_cond == 0 || flag == 0)
	//{
	if (Table_Exist(record.table_name) == true)//check the select table for existence
	{
		vector<string>values;//save for attribute value
		vector<int>types;//save for attribute type
		for (int i = 0; i < record.cond.size(); i++)
		{
			values.push_back(record.cond[i].value);
			types.push_back(record.cond[i].type);

		}
		if (record.num_cond == 0 || checkattributetype(record.table_name, values, types, states) == true)//check if the attribute is legal 
		{
			File_Node * file_temp = bm.get_File(Get_table_file(record.table_name).c_str());//get file node ptr
			Block_Node * block_temp = bm.getBlockHead(file_temp);//get block node ptr
			int block_num;
			int count = 0;
			while (true)
			{
				if (block_temp == NULL)
				{
					return -1;
				}
				if (block_temp->get_IfEnd())//judge if we have read to the end
				{
					block_num = block_select(record, block_temp);//get the block num
					count += block_num;
					return 60;
				}
				else
				{
					block_num = block_select(record, block_temp);
					count += block_num;
					block_temp = bm.getNextBlock(file_temp, block_temp);//find next
				}
			}
			return 60; //success
		}
		else
		{
			return 62;
			//type of column doesn't match
		}

	}
	else
	{
		return 61;//can't find table
	}
	//}
	//else
	//{
	//	vector<int>address;
	//	vector<int>tmp;
	//	File_Node * file_temp = bm.get_File(Get_table_file(record.table_name).c_str());//get file node ptr
	//	Block_Node * block_temp = bm.getBlockHead(file_temp);//get block node ptr
	//	int recordSize = recordSizeGet(record.table_name);
	//	int type = api->cm->getAttributeType(record.table_name, record.cond[0].col);
	//	switch (record.cond[0].op)
	//	{
	//	case 0: tmp = api->im->searchIndexLeft(Get_table_file(record.table_name), record.cond[0].value, type); address.push_back(tmp[tmp.size() - 1]); break;
	//	case 1: tmp = api->im->searchIndexLeft(Get_table_file(record.table_name), record.cond[0].value, type);
	//		address.assign(tmp.begin(), tmp.end() - 1);
	//		tmp = api->im->searchIndexRight(Get_table_file(record.table_name), record.cond[0].value, type);
	//		address.insert(address.end(), tmp.begin() + 1, tmp.end());
	//		break;
	//	case 2: tmp = api->im->searchIndexLeft(Get_table_file(record.table_name), record.cond[0].value, type); address.assign(tmp.begin(), tmp.end() - 1); break;
	//	case 3: tmp = api->im->searchIndexRight(Get_table_file(record.table_name), record.cond[0].value, type); address.assign(tmp.begin() + 1, tmp.end()); break;
	//	case 4: address = api->im->searchIndexLeft(Get_table_file(record.table_name), record.cond[0].value, type); break;
	//	case 5: address = api->im->searchIndexRight(Get_table_file(record.table_name), record.cond[0].value, type); break;
	//	default:
	//		break;
	//	}
	//	for (int i = 0; i < address.size(); i++)
	//	{
	//		int blocknum = address[i] / BlockSize;
	//		int offset = address[i] - blocknum * BlockSize;
	//		block_temp = bm.getBlockHead(file_temp);
	//		for (int j = 0; j < blocknum; j++)
	//		{
	//			block_temp = bm.getNextBlock(file_temp, block_temp);
	//		}
	//		record_save(record.table_name, block_temp->get_Address() + offset, recordSize);
	//	}

	//}


}

int RecordManager::block_select_record(Select record, int blockOffset)
{
	File_Node * file_temp = bm.get_File(Get_table_file(record.table_name).c_str());//get file node ptr
	Block_Node * block_ptr = bm.getBlockByOffset(file_temp, blockOffset);//get block ptr by offset
	if (block_ptr == NULL)
	{
		return -1;
	}
	else
	{
		return block_select(record, block_ptr);//search in the block
	}
}

int RecordManager::block_select(Select record, Block_Node * block)
{
	int count = 0;

	char * rec_begin = block->get_Content();//move the ptr to content begin place
	vector<Attribute> attributeVector;

	int recordSize = recordSizeGet(record.table_name);//get select record size
	attributeVector = attributeGet(record.table_name);//get select attribute imformatiom

	char * block_begin = block->get_Address();
	size_t usingSize = block->get_UsedSize();
	//cout << (int)block_begin << endl;
	while (rec_begin < block_begin + usingSize)
	{
		if (record_match(rec_begin, recordSize, &attributeVector, &record.cond))
		{
			count++;//if the record is matched than count the matched record 
			record_save(record.table_name, rec_begin, recordSize);//save the matched record
		}

		rec_begin += recordSize;//move the begin ptr to the next record place
	}

	return count;
}

int RecordManager::delete_record(Delete record)
{
	vector<Attribute> attributeVector;
	attributeVector = attributeGet(record.table_name);//get select attribute imformatiom
	int states = 0;

	if (Table_Exist(record.table_name) == true)//check the table existence
	{
		vector<string>values;
		vector<int>types;
		for (int i = 0; i < record.cond.size(); i++)
		{
			values.push_back(record.cond[i].col);
			types.push_back(record.cond[i].type);

		}
		if (checkattributetype(record.table_name, values, types, states) == true)//check the condition attribute
		{
			File_Node * file_temp = bm.get_File(Get_table_file(record.table_name).c_str());
			Block_Node * block_temp = bm.getBlockHead(file_temp);

			int count = 0;
			while (true)
			{
				if (block_temp == NULL)
				{
					return -1;
				}
				if (block_temp->get_IfEnd())
				{
					int recordBlockNum = block_delete(record, block_temp);
					count += recordBlockNum;
					return 70;
				}
				else
				{
					int recordBlockNum = block_delete(record, block_temp);
					count += recordBlockNum;
					block_temp = bm.getNextBlock(file_temp, block_temp);
				}
			}
			return 70; //success
		}
		else
		{
			return 72;
			//type of column doesn;t match
		}

	}
	else
	{
		return 71;//can't find table
	}


}
int RecordManager::delete_block_record(Delete record, int blockOffset)
{
	File_Node * file_temp = bm.get_File(Get_table_file(record.table_name).c_str());
	Block_Node * block_ptr = bm.getBlockByOffset(file_temp, blockOffset);
	if (block_ptr == NULL)
	{
		return -1;
	}
	else
	{
		return block_delete(record, block_ptr);
	}
}

int RecordManager::block_delete(Delete record, Block_Node * block)
{
	int count = 0;
	string filePath = Get_table_file(record.table_name);
	char * rec_begin = block->get_Content();
	char * rec_head = rec_begin;
	vector<Attribute> attributeVector;
	string key = "";
	int recordSize = recordSizeGet(record.table_name);//get from api//////
	attributeVector = attributeGet(record.table_name);//get from api//////

	while (rec_begin - block->get_Content() < block->get_UsedSize() - (block->get_Content() - block->get_Address()))
	{
		//if the recordBegin point to a record

		if (record_match(rec_begin, recordSize, &attributeVector, &record.cond))
		{
			count++;
			rec_head = rec_begin;
			//index delete
			//api->recordIndexDelete(rec_begin, recordSize, &attributeVector, block->get_OffNum());//get from api/////
			for (int i = 0; i < attributeVector.size(); i++)
			{
				char * tmp = NULL;
				tmp = (char *)malloc(typeSizeGet(attributeVector[i].AttributeType));
				memcpy(tmp, rec_head, typeSizeGet(attributeVector[i].AttributeType));
				key = tmp;
				rec_head += typeSizeGet(attributeVector[i].AttributeType);

				if (attributeVector[i].withindex == true)
				{
					vector<IndexInfo> index_temp = api->cm->List_Table_AllIndex(record.table_name);
					for (int j = 0; j < index_temp.size(); j++)
					{
						if (attributeVector[i].AttributeName == index_temp[j].Attribute)
							filePath = index_temp[j].tableName + "_" + index_temp[j].indexName;
					}

					//api->im->deleteIndex(filePath, key, attributeVector[i].AttributeType);
				}
			}
			int i = 0;
			for (i = 0; i + recordSize + rec_begin - block->get_Content() < block->get_UsedSize() - (block->get_Content() - block->get_Address()); i++)//if the block is not completely delete
			{
				rec_begin[i] = rec_begin[i + recordSize];
			}
			memset(rec_begin + i, 0, recordSize);//adter delete set the last place 0
			block->set_UsedSize(block->get_UsedSize() - recordSize);//update used size
			block->set_Dirty(true);//mark dirty for change
		}
		else
		{
			rec_begin += recordSize;
		}
	}
	return count;
}



string RecordManager::Get_table_file(string tableName)
{
	string tmp = "";
	return "TABLE_FILE_" + tableName;
}

bool RecordManager::record_match(char * recordBegin, int recordSize, vector<Attribute> * attributeVector, vector<Condition> * conditionVector)
{
	if (conditionVector == NULL)
	{
		return true;
	}
	int type;
	string attributeName;
	int typeSize;
	char content[255];

	char * contentBegin = recordBegin;
	for (int i = 0; i < attributeVector->size(); i++)
	{
		type = (*attributeVector)[i].AttributeType;
		attributeName = (*attributeVector)[i].AttributeName;
		typeSize = typeSizeGet(type);//get from api/////

		//initial content (when content is string , we can get a string easily)
		memset(content, 0, 255);
		memcpy(content, contentBegin, typeSize);
		for (int j = 0; j < (*conditionVector).size(); j++)
		{
			if ((*conditionVector)[j].col == attributeName)
			{
				//if this attribute need to deal about the condition
				if (!content_match(content, type, &(*conditionVector)[j]))
				{
					//if this record is not fit the conditon
					return false;
				}
			}
		}

		contentBegin += typeSize;
	}
	return true;
}

bool RecordManager::content_match(char * content, int type, Condition * condition)
{
	if (type == -1)
	{
		//if the content is a int
		int temp = *((int *)content); //get content value by point
		return condition->judge(temp);
	}
	else if (type == 0)
	{
		//if the content is a float
		float tmp = *((float *)content); //get content value by point
		return condition->judge(tmp);
	}
	else
	{
		//if the content is a string
		return condition->judge(content);
	}
	return true;
}

void  RecordManager::record_save(string table_name, char * recordBegin, int recordSize)
{
	if (res.num_tuple == 0)//initial 
	{
		vector<Attribute>tmp = attributeGet(table_name);
		//Attribute att_temp;
		//att_temp.AttributeName = "stuid";
		//att_temp.AttributeType = 10;
		//att_temp.primarykey = true;
		//att_temp.Unique = false;
		//att_temp.withindex = false;
		//tmp.push_back(att_temp);
		//att_temp.AttributeName = "name";
		//att_temp.AttributeType = 15;
		//att_temp.primarykey = false;
		//att_temp.Unique = true;
		//att_temp.withindex = false;
		//tmp.push_back(att_temp);
		//att_temp.AttributeName = "age";
		//att_temp.AttributeType = -1;
		//att_temp.primarykey = false;
		//att_temp.Unique = false;
		//att_temp.withindex = false;
		//tmp.push_back(att_temp);
		res.num_attr = tmp.size();
		res.attr = tmp;
	}
	res.num_tuple++;
	char * rec_begin = recordBegin;
	vector<string>tuple;
	for (int i = 0; i < res.num_attr; i++)
	{
		string element = "";
		char * temp = NULL;
		temp = (char *)malloc(typeSizeGet(res.attr[i].AttributeType) + 1);
		memcpy(temp, rec_begin, typeSizeGet(res.attr[i].AttributeType));

		if (res.attr[i].AttributeType > 0)
		{
			temp[typeSizeGet(res.attr[i].AttributeType)] = '\0';
			element = temp;
			tuple.push_back(element);
		}
		else if (res.attr[i].AttributeType == -1)
		{
			int num;
			num = *(int *)temp;
			stringstream ss;
			ss << num;
			ss >> element;
			tuple.push_back(element);
		}
		else if (res.attr[i].AttributeType == 0)
		{
			float num;
			num = *(float *)temp;
			stringstream ss;
			ss << num;
			element = ss.str();
			tuple.push_back(element);
		}

		rec_begin += typeSizeGet(res.attr[i].AttributeType);
	}//get tuple string vec
	res.res.push_back(tuple);//push into the group
}