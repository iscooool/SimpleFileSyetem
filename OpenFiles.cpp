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
 * @comment ���ؿ��е�fd�±�
 */
int OpenFiles::AllocFreeSlot() {

	for (int i = 0; i < OpenFiles::NOFILES; i++)		//�ҵ���һ�����е��ļ����
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
 * @comment ����fd���ض�Ӧ��File�ṹ
 */
File* OpenFiles::GetF(int fd) {
	if (fd < 0 || fd >= OpenFiles::NOFILES)
	{
		cout << "�ļ�����Ƿ�" << endl;
		return NULL;
	}
    return this->ProccessOpenFileTable[fd];
}

/**
 * @param fd
 * @param pFile
 * @return void
 * @comment ����fd��Ӧ��ָ��ָ��pFile
 */
void OpenFiles::SetF(int fd, File* pFile) {
	if (fd < 0 || fd >= OpenFiles::NOFILES)
	{
		cout << "�ļ�����Ƿ�" << endl;
		return;
	}
	this->ProccessOpenFileTable[fd] = pFile;
    return;
}

