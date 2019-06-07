#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H
#include "DBFile.h"
#include<stdio.h>
static int replaced_block=-1;
class BufferManager
{
    private:
        File_Node *filehead;
        File_Node file_pool[MAX_FILE_NUM];
        Block_Node block_pool[MAX_BLOCK_NUM];
        int num_usedblock;
        int num_usedfile;

        Block_Node *getBlock(File_Node *Fnode,Block_Node *bnodpos,bool pin = false);
        void WriteAllBack();
    public:
        BufferManager();
        ~BufferManager();
        void free_FileNode(const char *filename);
        // Get File
        File_Node *get_File(const char *filename,bool pin=false);
        // Get Block from File
        Block_Node *getNextBlock(File_Node *fnode,Block_Node *bnode);
        Block_Node *getBlockHead(File_Node *fnode);
        Block_Node *getBlockByOffset(File_Node *fnode,int offset);
}

