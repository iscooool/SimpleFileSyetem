/**
 * Project Untitled
 */


#include "FileSystem.h"
#include "Context.h"
/**
 * FileSystem implementation
 */
Context cxt;
FileSystem::FileSystem() {
	
}

FileSystem::~FileSystem(){

}

/**
 * @return void
 * @comment 初始化
 */
void FileSystem::Initialize() {
    return;
}

/**
 * @comment 从MyDisk的offset读取nbytes字节到buffer当中
 */
void FileSystem::ReadFromDisk(int offset, char* buffer, int nbytes) {
	FILE* MyDisk = cxt.GetMyDisk();

	if (MyDisk && buffer&& offset >= 0 && nbytes >= 0) 
	{

	}
}

/**
 * @comment 从offset写入nbytes字节的buffer内容到MyDisk当中
 */
void FileSystem::WriteToDisk(int offset, char* buffer, int nbytes) {

}

/**
 * @return void
 * @comment 加载SuperBlock到内存当中
 */
void FileSystem::LoadSuperBlock() {

	SuperBlock* g_spb = cxt.GetSuperBlock();
	FILE* MyDisk = cxt.GetMyDisk();

	void* p = (void*)g_spb;								//指向g_spb的void指针

	fseek(MyDisk, 0, SEEK_SET);							//定位到MyDisk开头位置
	if (!(fread(p, Inode::BLOCK_SIZE * 2, 1, MyDisk)))	//从My读取两个BLOCK_SIZE大小的
	{
		cout << "LoadSuperBlock失败" << endl;
		return;
	}

	this->f_SuperBlock = g_spb;
	g_spb->s_flock = 0;
	g_spb->s_ilock = 0;
	return;
}

/**
 * @return SuperBlock*
 * @comment 返回当前的SuperBlock
 */
SuperBlock* FileSystem::GetFS() {
    return this->f_SuperBlock;
}

/**
 * @return void
 * @comment 更新SuperBlock到硬盘当中
 */
void FileSystem::Update() {

	SuperBlock* g_spb = this->f_SuperBlock;
	FILE* MyDisk = cxt.GetMyDisk();

	void* p = (void*)g_spb;								//指向g_spb的void指针

	fseek(MyDisk, 0, SEEK_SET);							//定位到MyDisk开头位置
	if (!(fwrite(p, Inode::BLOCK_SIZE * 2, 1, MyDisk)))	//从My读取两个BLOCK_SIZE大小的
	{
		cout << "Update失败" << endl;
		return;
	}

	this->f_SuperBlock = g_spb;
	g_spb->s_flock = 0;
	g_spb->s_ilock = 0;
	return;
    return;
}

/**
 * @return Inode*
 * @comment 分配Inode结点
 */
Inode* FileSystem::IAlloc() {
    return NULL;
}

/**
 * @param number
 * @return void
 * @comment 释放Inode结点
 */
void FileSystem::IFree(int number) {
    return;
}

/**
 * @return int
 * @comment 分配DIskBlock
 */
int FileSystem::Alloc() {
    return 0;
}

/**
 * @param blkno
 * @return void
 * @comment 释放DiskBlock
 */
void FileSystem::Free(int blkno) {
    return;
}


