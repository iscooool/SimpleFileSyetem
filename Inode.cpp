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
 * @comment 对当前Inode进行读入，读入的IO参数在Context当中
 */
void Inode::ReadI() {
    return;
}

/**
 * @return void
 * @comment 对当前Inode进行写入，写入的IO参数在Context当中
 */
void Inode::WriteI() {
    return;
}

/**
 * @return void
 * @comment 清理Inode的参数
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
 * @comment 将包含外存Inode字符块中信息拷贝到内存Inode中
 */
void Inode::ICopy() {
    return;
}

/**
 * @param lbn
 * @return int
 * @comment 将逻辑盘块映射到物理盘块
 */
int Inode::Bmap(int lbn) {
    return 0;
}
/**
* @comment 将DiskInode的值赋给Inode
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

