/**
 * Project Untitled
 */


#ifndef _DIRECTORYENTRY_H
#define _DIRECTORYENTRY_H

class DirectoryEntry {
public: 
    int m_ino;							//ÿһ��Ŀ¼�����Inode��Ų���
    static const int DIRSIZ = 28;		//Ŀ¼�������ֵ���󳤶�
	char m_name[DIRSIZ];				//ÿһ��Ŀ¼��������ֲ���
    
    
public:
	DirectoryEntry();
    
    ~DirectoryEntry();
};

#endif //_DIRECTORYENTRY_H