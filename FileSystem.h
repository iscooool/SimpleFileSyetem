/**
 * Project Untitled
 */


#ifndef _FILESYSTEM_H
#define _FILESYSTEM_H
#include<iostream>
#include "SuperBlock.h"

#include "Inode.h"
using namespace::std;
class FileSystem {
public: 
    
    static const int SUPER_BLOCK_SECTOR_NUMBER = 0;
    static const int ROOTINO = 0;
    static const int INODE_NUMBER_PER_SECTOR = 8;
    static const int INODE_ZONE_START_SECTOR = 2;
    static const int INODE_ZONE_SIZE = 1024 - 2;
    static const int DATA_ZONE_START_SECTOR = 1024;
    static const int DATA_ZONE_END_SECTOR = 8000 - 1;
    static const int DATA_ZONE_SIZE = 8000 - DATA_ZONE_START_SECTOR;
    SuperBlock* f_SuperBlock;
    
   /**
    * @comment 初始化
    *
    */
    void Initialize();

	/*
	 *@comment 从MyDisk的offset读取nbytes字节到buffer当中
	 */
	void ReadFromDisk(int offset, char* buffer, int nbytes);

	/*
	 *@comment 从offset写入nbytes字节的buffer内容到MyDisk当中
	 */
	void WriteToDisk(int offset, char* buffer, int nbytes);


   /*
    *@comment 加载SuperBlock到内存当中
    *
    */ 
    void LoadSuperBlock();

    /*
     * @comment 返回当前的SuperBlock
     *
     */
    
    SuperBlock* GetFS();
   /*
    * @comment 更新SuperBlock到硬盘当中
    *
    */
    void Update();
   /*
    * @comment 分配Inode结点
    *
    */
    Inode* IAlloc();
    
    /**
     * @param number
     * @comment 释放Inode结点
     */
    void IFree(int number);
   /*
    * @comment 分配DIskBlock
    *
    */
    int Alloc();
    
    /**
     * @param blkno
     * @comment 释放DiskBlock
     */
    void Free(int blkno);
public: 
    
    FileSystem();
    ~FileSystem();
};

#endif //_FILESYSTEM_H
