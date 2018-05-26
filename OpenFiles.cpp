/**
 * Project Untitled
 */


#include "OpenFiles.h"

/**
 * OpenFiles implementation
 */


OpenFiles::OpenFiles() {

}
OpenFiles::~OpenFiles() {

}

/**
 * @return int
 * @comment 返回空闲的fd下标
 */
int OpenFiles::AllocFreeSlot() {
    return 0;
}

/**
 * @param fd
 * @return File*
 * @comment 根据fd返回对应的File结构
 */
File* OpenFiles::GetF(int fd) {
    return NULL;
}

/**
 * @param fd
 * @param pFile
 * @return void
 * @comment 设置fd对应的指针指向pFile
 */
void OpenFiles::SetF(int fd, File* pFile) {
    return;
}

