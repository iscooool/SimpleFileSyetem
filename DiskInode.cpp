/**
 * Project Untitled
 */


#include "DiskInode.h"

/**
 *@comment ¹¹Ôìº¯Êý 
 */


DiskInode::DiskInode() {
	this->d_mode = 0;
	this->d_nlink = 0;
	this->d_uid = -1;
	this->d_gid = -1;
	this->d_size = 0;
	for (int i = 0; i < 10; i++)
	{
		this->d_addr[i] = 0;
	}
	this->d_atime = 0;
	this->d_mtime = 0;
}

DiskInode::~DiskInode() {

}