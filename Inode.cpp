/**
 * Project Untitled
 */


#include "Inode.h"

/**
 * Inode implementation
 */


Inode::Inode() {
	this->i_mode = 0;
	this->i_number = -1;
	this->i_uid = -1;
	this->i_gid = -1;
	this->i_size = 0;
	for (int i = 0; i < 10; i++)
	{
		this->i_addr[i] = 0;
	}
}
Inode::~Inode() {

}

/**
 * @return void
 * @comment �Ե�ǰInode���ж��룬�����IO������Context����
 */
void Inode::ReadI() {
    return;
}

/**
 * @return void
 * @comment �Ե�ǰInode����д�룬д���IO������Context����
 */
void Inode::WriteI() {
    return;
}

/**
 * @return void
 * @comment ����Inode�Ĳ���
 */
void Inode::Clean() {
	this->i_mode = 0;
	this->i_uid = -1;
	this->i_gid = -1;
	this->i_size = 0;
	for (int i = 0; i < 10; i++)
	{
		this->i_addr[i] = 0;
	}
    return;
}

/**
 * @return void
 * @comment ���������Inode�ַ�������Ϣ�������ڴ�Inode��
 */
void Inode::ICopy() {
    return;
}

/**
 * @param lbn
 * @return int
 * @comment ���߼��̿�ӳ�䵽�����̿�
 */
int Inode::Bmap(int lbn) {
    return 0;
}
/**
* @comment ��DiskInode��ֵ����Inode
*
*/
void Inode::FromDiskInode(DiskInode DInode) {
	this->i_mode = DInode.d_mode;
	this->i_uid = DInode.d_uid;
	this->i_gid = DInode.d_gid;
	this->i_size = DInode.d_size;

	for (int i = 0; i < 10; i++)
	{
		this->i_addr[i] = DInode.d_addr[i];
	}

}

