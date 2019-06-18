#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include "BufferManager.h"
#include<stdlib.h>
#include<string>
#include<cstring>
#include<queue>
extern int replaced_block;
BufferManager::BufferManager() :num_usedblock(0), num_usedfile(0), filehead(NULL) {}
BufferManager::~BufferManager() { WriteAllBack(); }
void BufferManager::free_FileNode(const char *filename)
{
	File_Node *ftmp = get_File(filename);
	Block_Node* btmp = ftmp->get_Bhead();
	queue<Block_Node*> blockQ;
	while (true) {
		if (NULL == btmp) return;
		blockQ.push(btmp);
		if (btmp->ifendnode) break;
		btmp = getNextBlock(ftmp, btmp);
	}
	num_usedblock -= blockQ.size();
	while (!blockQ.empty())
	{
		blockQ.back()->Init();
		blockQ.pop();
	}
	if (ftmp->get_Pfile()) (ftmp->get_Pfile())->set_Nfile(ftmp->get_Nfile());
	if (ftmp->get_Nfile()) (ftmp->get_Nfile())->set_Pfile(ftmp->get_Pfile());
	if (filehead == ftmp) filehead = ftmp->get_Nfile();
	ftmp->Init();
	num_usedfile--;
}
// Get File
File_Node *BufferManager::get_File(const char *filename, bool pin)
{
	Block_Node * btmp = NULL;
	File_Node * ftmp = NULL;
	if (filehead != NULL)
	{
		for (ftmp = filehead; ftmp != NULL; ftmp = ftmp->get_Nfile())
		{
			if (!strcmp(filename, ftmp->get_FileName())) //the File_Node is already in the list
			{
				ftmp->set_Pin(pin);
				return ftmp;
			}
		}
	}
	// The File_Node is not in the list
	if (num_usedfile == 0) // No file in the list now
	{
		ftmp = &file_pool[num_usedfile];
		num_usedfile++;
		filehead = ftmp;
	}
	else if (num_usedfile < MAX_FILE_NUM) // There are empty File_Node in the pool
	{
		ftmp = &file_pool[num_usedfile];
		// add this File_Node into the tail of the list
		file_pool[num_usedfile - 1].set_Nfile(ftmp);
		ftmp->set_Pfile(&file_pool[num_usedfile - 1]);
		num_usedfile++;
	}
	else // if num_usedfile >= MAX_FILE_NUM, find one File_Node to replace, write back the block node belonging to the File_Node
	{
		ftmp = filehead;
		while (ftmp->get_Pin())
		{
			if (ftmp->get_Nfile()) ftmp = ftmp->get_Nfile();
			else //no enough file node in the pool
			{
				printf("There are no enough file node in the pool!");
				exit(2);
			}
		}
		for (btmp = ftmp->get_Bhead(); btmp != NULL; btmp = btmp->get_Nblock())
		{
			if (btmp->get_Pblock())
			{
				(btmp->get_Pblock())->Init();
				num_usedblock--;
			}
			btmp->WriteBack();
		}
		ftmp->Init();
	}
	if (strlen(filename) + 1 > MAX_FILE_NAME)
	{
		printf("文件名长度过长\n");
		exit(3);
	}
	strncpy(ftmp->get_FileName(), filename, MAX_FILE_NAME);
	ftmp->set_Pin(pin);
	return ftmp;
}
// Get Block from File
Block_Node *BufferManager::getNextBlock(File_Node *fnode, Block_Node *bnode)
{
	if (bnode->get_Nblock() == NULL)
	{
		if (bnode->get_IfEnd())
			bnode->set_IfEnd(false);
		return getBlock(fnode, bnode);
	}
	else
	{
		if (bnode->get_OffNum() == bnode->get_Nblock()->get_OffNum() - 1)
			return bnode->get_Nblock();
		else
			return getBlock(fnode, bnode);
	}
}
Block_Node *BufferManager::getBlockHead(File_Node *fnode)
{
	Block_Node *btmp = NULL;
	if (NULL != fnode->get_Bhead())
	{
		if (0 == (fnode->get_Bhead())->get_OffNum())
			btmp = fnode->get_Bhead();
		else
			btmp = getBlock(fnode, NULL);
	}
	else
		btmp = getBlock(fnode, NULL);
	return btmp;
}
Block_Node *BufferManager::getBlockByOffset(File_Node *fnode, int offset)
{
	Block_Node* btmp = NULL;
	if (0 == offset)
		return getBlockHead(fnode);
	else
	{
		btmp = getBlockHead(fnode);
		while (offset > -0)
		{
			btmp = getNextBlock(fnode, btmp);
			offset--;
		}
		return btmp;
	}
}
Block_Node *BufferManager::getBlock(File_Node *fnode, Block_Node *bnodpos, bool pin)
{
	const char * filename = fnode->get_FileName();
	Block_Node *btmp = NULL;
	if (0 == num_usedblock)
	{
		btmp = &block_pool[0];
		num_usedblock++;
	}
	else if (num_usedblock < MAX_BLOCK_NUM)
	{
		for (int i = 0; i < MAX_BLOCK_NUM; i++)
		{
			if (-1 == block_pool[i].get_OffNum())
			{
				btmp = &block_pool[i];
				num_usedblock++;
				break;
			}
			else
				continue;
		}
	}
	else
	{
		int i = replaced_block;
		while (true)
		{
			i++;
			if (i >= num_usedblock) i = 0;
			if (!block_pool[i].get_Pin())
			{
				if (true == block_pool[i].get_Refer())
					block_pool[i].set_Refer(false);
				else
				{
					btmp = &block_pool[i];
					if (btmp->get_Nblock()) (btmp->get_Nblock())->set_Pblock(btmp->get_Pblock());
					if (btmp->get_Pblock()) (btmp->get_Pblock())->set_Nblock(btmp->get_Nblock());
					if (btmp == fnode->get_Bhead()) fnode->set_Bhead(btmp->get_Nblock());
					replaced_block = i;
					btmp->WriteBack();
					btmp->Init();
					break;
				}
			}
			else
				continue;
		}
	}
	if (bnodpos != NULL && bnodpos->get_Nblock() == NULL)
	{
		btmp->set_Pblock(bnodpos);
		bnodpos->set_Nblock(btmp);
		btmp->set_OffNum(bnodpos->get_OffNum() + 1);
	}
	else if (bnodpos != NULL && bnodpos->get_Nblock() != NULL)
	{
		btmp->set_Pblock(bnodpos);
		btmp->set_Nblock(bnodpos->get_Nblock());
		(bnodpos->get_Nblock())->set_Pblock(btmp);
		bnodpos->set_Nblock(btmp);
		btmp->set_OffNum(bnodpos->get_OffNum() + 1);
	}
	else
	{
		btmp->set_OffNum(0);
		if (fnode->get_Bhead())
		{
			(fnode->get_Bhead())->set_Pblock(btmp);
			btmp->set_Nblock(fnode->get_Bhead());
		}
		fnode->set_Bhead(btmp);
	}
	btmp->set_Pin(pin);
	if (strlen(filename) + 1 > MAX_FILE_NAME)
	{
		printf("文件名长度过长\n");
		exit(3);
	}
	strncpy(btmp->get_FileName(), filename, MAX_FILE_NAME);

	FILE *fileHandle;
	if ((fileHandle = fopen(filename, "ab+")) != NULL)
	{
		if (fseek(fileHandle, (btmp->get_OffNum() * BLOCK_SIZE), 0) == 0)
		{
			if (fread(btmp->get_Address(), 1, BLOCK_SIZE, fileHandle) == 0)
				btmp->set_IfEnd(true);
			btmp->set_UsedSize(btmp->get_UsedSize());
		}
		else
		{
			printf("Problem seeking the file %s in reading", filename);
			exit(1);
		}
		fclose(fileHandle);
	}
	else
	{
		printf("Problem opening the file %s in reading", filename);
		exit(1);
	}
	return btmp;
}
void BufferManager::WriteAllBack()
{
	Block_Node *btmp = NULL;
	File_Node *ftmp = NULL;
	if (filehead)
	{
		for (ftmp = filehead; ftmp != NULL; ftmp = ftmp->get_Nfile())
		{
			if (ftmp->get_Bhead())
			{
				for (btmp = ftmp->get_Bhead(); btmp != NULL; btmp = btmp->get_Nblock())
				{
					if (btmp->get_Pblock())
						(btmp->get_Pblock())->Init();
					btmp->WriteBack();
				}
			}
		}
	}
}
