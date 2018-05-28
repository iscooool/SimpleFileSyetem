/**
 * Project Untitled
 */


#include "OpenFiles.h"

/**
 * OpenFiles implementation
 */


OpenFiles::OpenFiles() {
	for (int i = 0; i < OpenFiles::NOFILES; i++)
	{
		this->ProccessOpenFileTable[i] = NULL;
	}
}
OpenFiles::~OpenFiles() {

}

/**
 * @return int
 * @comment 返回空闲的fd下标
 */
int OpenFiles::AllocFreeSlot() {

	for (int i = 0; i < OpenFiles::NOFILES; i++)		//找到第一个空闲的文件句柄
	{
		if (this->ProccessOpenFileTable[i] == NULL)
		{
			return i;
		}
	}
    return -1;
}

/**
 * @param fd
 * @return File*
 * @comment 根据fd返回对应的File结构
 */
File* OpenFiles::GetF(int fd) {
	if (fd < 0 || fd >= OpenFiles::NOFILES)
	{
		cout << "文件句柄非法" << endl;
		return NULL;
	}
    return this->ProccessOpenFileTable[fd];
}

/**
 * @param fd
 * @param pFile
 * @return void
 * @comment 设置fd对应的指针指向pFile
 */
void OpenFiles::SetF(int fd, File* pFile) {
	if (fd < 0 || fd >= OpenFiles::NOFILES)
	{
		cout << "文件句柄非法" << endl;
		return;
	}
	this->ProccessOpenFileTable[fd] = pFile;
    return;
}

