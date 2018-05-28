/**
 * Project Untitled
 */


#ifndef _OPENFILES_H
#define _OPENFILES_H
#include "File.h"
#include "DirectoryEntry.h"
#include<iostream>
using namespace std;
class OpenFiles {
public: 
    static const int NOFILES = 15;
    File* ProccessOpenFileTable[NOFILES];
	char FileName[NOFILES][DirectoryEntry::DIRSIZ];
  
	/**
	 * @comment ���ؿ��е�fd�±�
	 */
    int AllocFreeSlot();
    
    /**
     * @param fd
	 * @comment ����fd���ض�Ӧ��File�ṹ
     */
    File* GetF(int fd);
    
    /**
     * @param fd
     * @param pFile
	 * @comment ����fd��Ӧ��ָ��ָ��pFile
     */
    void SetF(int fd, File* pFile);
public:
	OpenFiles();
	~OpenFiles();
};

#endif //_OPENFILES_H