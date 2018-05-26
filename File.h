/**
 * Project Untitled
 */


#ifndef _FILE_H
#define _FILE_H
#include"Inode.h"

class File {
public:
    enum FileFlags
    {
        FREAD = 0x1,            /* 读请求类型 */
        FWRITE = 0x2,           /* 写请求类型 */
    };
    unsigned int f_flag;
    Inode* f_inode;
    int f_offset;
    

public:
	File();
    ~File();
};

#endif //_FILE_H
