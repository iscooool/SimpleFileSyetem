/**
 * Project Untitled
 */


#ifndef _FILEMANAGER_H
#define _FILEMANAGER_H
#include"Inode.h"
#include"OpenFiles.h"
#include"FileSystem.h"
#include"File.h"
class FileManager {
public: 
    Inode* rootDirInode;
    OpenFiles* OpenFiles;
    FileSystem* m_FileSystem;
    
   
    
    void Initialize();
    
	/**
	 * @comment ��ʽ��Ӳ��
	 */
    void fformat();
    
    /**
     * @param fName
     * @param mode
	 * @comment �����ļ�
     */
    int fcreat(char* fName, int mode);
    
    /**
     * @param fName
     * @param mode
	 * @comment ���ļ�
     */
    int fopen(char* fName, int mode);
    
    /**
     * @param fd
	 * @comment �ر��ļ�
     */
    void fclose(int fd);
    
    /**
     * @param fd
     * @param buffer
     * @param length
	 * @comment ��ȡlength���ȵ����ݵ�buffer����
     */
    int fread(int fd, char* buffer, int length);
    
    /**
     * @param fd
     * @param buffer
     * @param length
	 * @��length��С������д�뵽fd����
     */
    int fwrite(int fd, char* buffer, int length);
    
    /**
     * @param fd
     * @param buffer
     * @param length
     * @param mode
	 * @comment �ú�����fread��fwrite����
     */
    int Rdwr(int fd, char* buffer, int length, enum File::FileFlags mode);
    
    /**
     * @param fd
     * @param position
     * @comment ���ļ�ָ��ָ��position
     */
    int flseek(int fd, int position);
    
    /**
     * @param fName
     * @comment ɾ���ļ�
     */
    int fdelete(char* fName);
public:
	FileManager();
	~FileManager();
};

#endif //_FILEMANAGER_H
