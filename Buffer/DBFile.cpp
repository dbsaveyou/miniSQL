
#define _CRT_SECURE_NO_WARNINGS
#include "DBFile.h"
Block_Node::Block_Node()
{
	address = new char[BLOCK_SIZE];
	if (NULL == address)
	{
		cout << "Failed to malloc space for Block." << endl;
		exit(1);
	}
	filename = new char[MAX_FILE_NAME];
	if (NULL == filename)
	{
		cout << "Failed to malloc space for Block_FileName." << endl;
		exit(1);
	}
	Init();
}
Block_Node::~Block_Node()
{
	WriteBack();
	delete[] filename;
	delete[] address;
}
char *Block_Node::get_Address()
{
	return address;
}
void Block_Node::set_Pblock(Block_Node *bnode)
{
	this->preblock = bnode;
}
Block_Node *Block_Node::get_Pblock()
{
	return preblock;
}
void Block_Node::set_Nblock(Block_Node *nnode)
{
	this->nextblock = nnode;
}
Block_Node *Block_Node::get_Nblock()
{
	return nextblock;
}
void Block_Node::set_Refer(bool reference)
{
	this->refer = reference;
}
bool Block_Node::get_Refer()
{
	return refer;
}
void Block_Node::set_Dirty(bool dirtybit)
{
	this->dirty = dirtybit;
}
bool Block_Node::get_Dirty()
{
	return this->dirty;
}
void Block_Node::set_UsedSize(size_t usedsize)
{
	used_size = usedsize;
}
size_t Block_Node::get_UsedSize()
{
	return used_size;
}
void Block_Node::set_OffNum(int offnum)
{
	this->offsetnum = offnum;
}
int Block_Node::get_OffNum()
{
	return this->offsetnum;
}
void Block_Node::set_Pin(bool pinbit)
{
	this->pin = pinbit;
}
bool Block_Node::get_Pin()
{
	return this->pin;
}
void Block_Node::set_IfEnd(bool ifend)
{
	this->ifendnode = ifend;
}
bool Block_Node::get_IfEnd()
{
	return this->ifendnode;
}
void Block_Node::set_FileName(char *filename)
{
	this->filename = filename;
}
char *Block_Node::get_FileName()
{
	return this->filename;
}
void Block_Node::Init()
{
	memset(address, 0, BLOCK_SIZE);
	size_t init_usage = 0;
	memcpy(address, (char*)&init_usage, sizeof(size_t));
	used_size = sizeof(size_t);
	dirty = false;
	nextblock = NULL;
	preblock = NULL;
	offsetnum = -1;
	pin = false;
	refer = false;
	ifendnode = false;
	memset(filename, 0, MAX_FILE_NAME);
}
void Block_Node::WriteBack()
{
	if (!dirty)
		return;
	else
	{
		FILE *fileHandle = NULL;
        if((fileHandle = fopen(filename, "rb+")) != NULL)
        {
            if(fseek(fileHandle, this->offsetnum*BLOCK_SIZE, 0) == 0)
            {
                if(fwrite(this->address, this->used_size, 1, fileHandle) != 1)
                {
                    printf("Problem writing the file %s in writtenBackToDisking",filename);
                    exit(1);
                }
            }
            else
            {
                printf("Problem seeking the file %s in writtenBackToDisking",filename);
                exit(1);
            }
            fclose(fileHandle);
        }
        else
        {
            printf("Problem opening the file %s in writtenBackToDisking",filename);
            exit(1);
        }
	}
}
	char *Block_Node::get_Content()
	{
		return address + sizeof(size_t);
	}
	int Block_Node::get_RemainedSize()
	{
		return BLOCK_SIZE - used_size;
	}



	File_Node::File_Node()
	{
		filename = new char[MAX_FILE_NAME];
		if (NULL == filename)
		{
			cout << "Failed to allocate memory for filenode." << endl;
			exit(1);
		}
		Init();
	}
	File_Node::~File_Node()
	{
		delete[] filename;
	}
	char *File_Node::get_FileName()
	{
		return filename;
	}
	void File_Node::set_Bhead(Block_Node *bhead)
	{
		blockhead = bhead;
	}
	Block_Node *File_Node::get_Bhead()
	{
		return blockhead;
	}
	void File_Node::set_Pfile(File_Node *pfile)
	{
		prefile = pfile;
	}
	File_Node *File_Node::get_Pfile()
	{
		return prefile;
	}
	void File_Node::set_Nfile(File_Node *nfile)
	{
		nextfile = nfile;
	}
	File_Node *File_Node::get_Nfile()
	{
		return nextfile;
	}
	void File_Node::set_Pin(bool _pin)
	{
		pin = _pin;
	}
	bool File_Node::get_Pin()
	{
		return pin;
	}
	void File_Node::Init()
	{
		nextfile = NULL;
		prefile = NULL;
		blockhead = NULL;
		pin = false;
		memset(filename, 0, MAX_FILE_NAME);
	}

