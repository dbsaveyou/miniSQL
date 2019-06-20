#ifndef DBFILE_H
#define DBFILE_H

#define MAX_FILE_NUM 40
#define MAX_BLOCK_NUM 300
#define MAX_FILE_NAME 100
#include<iostream>
#include<stdlib.h>
#include<string>
#include<cstring>
#include<fstream>

using namespace std;
class Block_Node
{
private:
	static const int BLOCK_SIZE = 4096;
	char *address; // the content address
	Block_Node *preblock;
	Block_Node *nextblock;

	bool refer; // the LRU repacement flag
	bool dirty; // dirty bit
	size_t used_size;

	int offsetnum; // in real block list
	bool pin;
	bool ifendnode;
	char *filename;
public:
	Block_Node(); // allocate space
	~Block_Node();// free space
	//<---
	char *get_Address();
	void set_Pblock(Block_Node *bnode);
	Block_Node *get_Pblock();
	void set_Nblock(Block_Node *nnode);
	Block_Node *get_Nblock();
	void set_Refer(bool reference);
	bool get_Refer();
	void set_Dirty(bool dirtybit);
	bool get_Dirty();
	void set_UsedSize(size_t usedsize);
	size_t get_UsedSize();
	void set_OffNum(int offnum);
	int get_OffNum();
	void set_Pin(bool pinbit);
	bool get_Pin();
	void set_IfEnd(bool ifend);
	bool get_IfEnd();
	void set_FileName(char *filename);
	char *get_FileName();
	//--->
	void Set_Zero();
	void Init();
	void WriteBack();
	char *get_Content();
	int get_RemainedSize();
	friend class File_Node;
	friend class BufferManager;
};
class File_Node
{
private:
	char *filename;
	bool pin;
	Block_Node *blockhead;
	File_Node *prefile;
	File_Node *nextfile;
public:
	File_Node();
	~File_Node();
	char *get_FileName();
	void set_Bhead(Block_Node *bhead);
	Block_Node *get_Bhead();
	void set_Pfile(File_Node *pfile);
	File_Node *get_Pfile();
	void set_Nfile(File_Node *nfile);
	File_Node *get_Nfile();
	void set_Pin(bool _pin);
	bool get_Pin();
	void Init();
	friend class BufferManager;
};
#endif
