/**
 * Project Untitled
 */


#ifndef _CONTEXT_H
#define _CONTEXT_H
#include "FileManager.h"
#include "Inode.h"
#include "IOParameter.h"
#include "FileSystem.h"



class Context {
public: 
    FileManager c_FileManager;
    Inode* c_pwd;
    char* c_pwd_path;
    IOParameter c_IOParameter;
    char* c_Buffer;
	FileSystem c_FileSystem;
    SuperBlock* c_spb;
    FILE* MyDisk;
    char* c_ReadBuffer;
    
  
    
    FileManager* GetFileManager();
    
    Inode* GetPwd();
    
    char* GetPwdPath();
    
    IOParameter* GetIOParameter();
    
    FileSystem* GetFileSystem();
    
    SuperBlock* GetSuperBlock();

	FILE* GetMyDisk();

	void DwordCopy(int *src, int *dst, int count);

public:
	Context();
	~Context();
};

#endif //_CONTEXT_H