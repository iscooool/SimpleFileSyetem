/**
 * Project Untitled
 */


#ifndef _SUPERBLOCK_H
#define _SUPERBLOCK_H

class SuperBlock {
public: 
    int s_isize;
    int s_fsize;
    int s_nfree;
    int s_free[100];
    int s_ninode;
    int s_inode[100];
    int s_flock;
    int s_ilock;
    int s_fmod;
    int s_time;
    int padding[48];
    
public:
	SuperBlock();
	~SuperBlock();
};

#endif //_SUPERBLOCK_H