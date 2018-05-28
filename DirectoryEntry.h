/**
 * Project Untitled
 */


#ifndef _DIRECTORYENTRY_H
#define _DIRECTORYENTRY_H

class DirectoryEntry {
public: 
    int m_ino;							//每一个目录表项的Inode编号部分
    static const int DIRSIZ = 28;		//目录表项名字的最大长度
	char m_name[DIRSIZ];				//每一个目录表项的名字部分
    
    
public:
	DirectoryEntry();
    
    ~DirectoryEntry();
};

#endif //_DIRECTORYENTRY_H