/**
 * Project Untitled
 */


#ifndef _INODE_H
#define _INODE_H
#include "DiskInode.h"
class Inode {
public: 
    enum InodeFlag;
    static const unsigned int IALLOC = 0x6000;						// 文件被使用 
    static const unsigned int IFDIR = 0x4000;						// 文件类型：目录文件 
    static const unsigned int ILARG = 0x1000;						// 文件长度类型：大型或巨型文件 
    static const unsigned int IREAD = 0x100;						// 对文件的读权限 
    static const unsigned int IWRITE = 0x80;						// 对文件的写权限 
    static const unsigned int IEXEC = 0x40;							// 对文件的执行权限 
    static const unsigned int IRWXU = (IREAD|IWRITE|IEXEC);			// 文件主对文件的读、写、执行权限 
    static const unsigned int IRWXG = ((IRWXU) >> 3);				// 文件主同组用户对文件的读、写、执行权限 
    static const unsigned int IRWXO = ((IRWXU) >> 6);				// 其他用户对文件的读、写、执行权限 
    static const int BLOCK_SIZE = 512;								// 每一个盘块所占的字节数
    static const int ADDRESS_PER_INDEX_BLOCK = BLOCK_SIZE / sizeof(int);
    static const int SMALL_FILE_BLOCK = 6;
    static const int LARGE_FILE_BLOCK = 128 * 2 + 6;
    static const int HUGE_FILE_BLOCK = 128 * 128 * 2 + 128 * 2 + 6;


	unsigned int i_mode;	/* 文件工作方式信息 */
	int	i_number;			/* 外存inode区中的编号 */
    short i_uid;			/* 文件所有者的用户标识数 */
    short i_gid;			/* 文件所有者的组标识数 */
    int i_size;				/* 文件大小，字节为单位 */
    int i_addr[10];			/* 用于文件逻辑块好和物理块好转换的基本索引表 */
	    
    /*
	 * @comment 对当前Inode进行读入，读入的IO参数在Context当中
	 *
	 */
    void ReadI();

	/*
	* @comment 对当前Inode进行写入，写入的IO参数在Context当中
	*
	*/
    void WriteI();
    
	/*
	* @comment 清理Inode的参数
	*
	*/
    void Clean();
    
	/*
	* @comment 将包含外存Inode字符块中信息拷贝到内存Inode中
	*
	*/
    void ICopy();
	/**
	 * @comment 将包含内存Inode字符块中信息拷贝到外存Inode中
	 *
	 */
	void IUpdate();
    /**
     * @param lbn
	 * @comment 将逻辑盘块映射到物理盘块
     */
    int Bmap(int lbn);
	/**
	 * @comment 将DiskInode的值赋给Inode
	 *
	 */
	void FromDiskInode(DiskInode DInode);
	/**
	 * @comment 将Inode的值赋给DiskInode
	 *
	 */
	DiskInode ToDiskInode();
public:
	Inode();
	~Inode();
};

#endif //_INODE_H