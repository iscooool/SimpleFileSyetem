/**
 * Project Untitled
 */


#ifndef _DIRECTORYENTRY_H
#define _DIRECTORYENTRY_H

class DirectoryEntry {
public: 
    int m_ino;
    static const int DIRSIZ = 28;
	char m_name[DIRSIZ];
    
    
public:
	DirectoryEntry();
    
    ~DirectoryEntry();
};

#endif //_DIRECTORYENTRY_H