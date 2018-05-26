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
	 * @comment 格式化硬盘
	 */
    void fformat();
    
    /**
     * @param fName
     * @param mode
	 * @comment 创建文件
     */
    int Fcreat(char* fName, int mode);
    
    /**
     * @param fName
     * @param mode
	 * @comment 打开文件
     */
    int Fopen(char* fName, int mode);
    
    /**
     * @param fd
	 * @comment 关闭文件
     */
    void Fclose(int fd);
    
    /**
     * @param fd
     * @param buffer
     * @param length
	 * @comment 读取length长度的数据到buffer当中
     */
    int Fread(int fd, char* buffer, int length);
    
    /**
     * @param fd
     * @param buffer
     * @param length
	 * @将length大小的数据写入到fd当中
     */
    int Fwrite(int fd, char* buffer, int length);
    
    /**
     * @param fd
     * @param buffer
     * @param length
     * @param mode
	 * @comment 该函数有fread和fwrite调用
     */
    int Rdwr(int fd, char* buffer, int length, enum File::FileFlags mode);
    
    /**
     * @param fd
     * @param position
     * @comment 将文件指针指向position
     */
    int Flseek(int fd, int position);
    
    /**
     * @param fName
     * @comment 删除文件
     */
    int Fdelete(char* fName);
public:
	FileManager();
	~FileManager();
};

#endif //_FILEMANAGER_H
