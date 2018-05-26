/**
 * Project Untitled
 */


#ifndef _DISKINODE_H
#define _DISKINODE_H

class DiskInode {
public: 
    unsigned int d_mode;
    int d_nlink;
    short d_uid;
    short d_gid;
    int d_size;
    int d_addr[10];
    int d_atime;
    int d_mtime;

public:
    DiskInode();

    
    ~DiskInode();
};

#endif //_DISKINODE_H