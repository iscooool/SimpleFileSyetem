/**
 * Project Untitled
 */


#ifndef _OPENFILES_H
#define _OPENFILES_H
#include "File.h"
#include<iostream>
class OpenFiles {
public: 
    static const int NOFILES = 15;
    File* ProccessOpenFileTable[NOFILES];
    
  
	/**
	 * @comment 返回空闲的fd下标
	 */
    int AllocFreeSlot();
    
    /**
     * @param fd
	 * @comment 根据fd返回对应的File结构
     */
    File* GetF(int fd);
    
    /**
     * @param fd
     * @param pFile
	 * @comment 设置fd对应的指针指向pFile
     */
    void SetF(int fd, File* pFile);
public:
	OpenFiles();
	~OpenFiles();
};

#endif //_OPENFILES_H