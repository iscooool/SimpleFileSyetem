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
 * @comment ���ؿ��е�fd�±�
 */
int OpenFiles::AllocFreeSlot() {
    return 0;
}

/**
 * @param fd
 * @return File*
 * @comment ����fd���ض�Ӧ��File�ṹ
 */
File* OpenFiles::GetF(int fd) {
    return NULL;
}

/**
 * @param fd
 * @param pFile
 * @return void
 * @comment ����fd��Ӧ��ָ��ָ��pFile
 */
void OpenFiles::SetF(int fd, File* pFile) {
    return;
}

