/**
 * Project Untitled
 */


#include "FileSystem.h"
#include "DiskInode.h"
#include "Context.h"
/**
 * FileSystem implementation
 */
extern Context cxt;
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
	//SuperBlock* g_spb = new SuperBlock();
	FILE* MyDisk = cxt.GetMyDisk();

	void* p = (void*)g_spb;								//指向g_spb的void指针

	fseek(MyDisk, 0, SEEK_SET);							//定位到MyDisk开头位置
	if (!(fread(p, Inode::BLOCK_SIZE * 2, 1, MyDisk)))	//从My读取两个BLOCK_SIZE大小的数据
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
	if (!(fwrite(p, Inode::BLOCK_SIZE * 2, 1, MyDisk)))	//从My读取两个BLOCK_SIZE大小数据
	{
		cout << "Update失败" << endl;
		return;
	}

	
	return;
    
}

/**
 * @return Inode*
 * @comment 分配Inode结点
 */
Inode* FileSystem::IAlloc() {
	Inode* pNode = new Inode();
	int ino;
	SuperBlock* sb = this->f_SuperBlock;
	FILE* MyDisk = cxt.GetMyDisk();

	if (sb->s_ninode == 0)
	{
		int ino = -1;

		fseek(MyDisk, FileSystem::INODE_ZONE_START_SECTOR*Inode::BLOCK_SIZE, SEEK_SET);	//寻址到Inode区的开始位置
		unsigned int d_mode;
		void* p = (void*)&d_mode;
		int inc = (Inode::BLOCK_SIZE / FileSystem::INODE_NUMBER_PER_SECTOR) - sizeof(unsigned int);		//两个相邻Inode结点的d_mode的间隔减去unsigned int 的大小
		
		for (int i = 0; i < FileSystem::INODE_ZONE_SIZE; i++)						//读取每个Inode的d_mode的数据
		{


			if (!(fread(p, 1 * sizeof(unsigned int), 1, MyDisk)))
			{
				cout << "读取第"<<i<<"位置的d_mode失败" << endl;
				return NULL;
			}

			ino++;

			if (d_mode != 0)													//该Inode已经被占用
			{
				continue;
			}

			sb->s_inode[sb->s_ninode++] = ino;									//将空闲的Inode存入Inode索引表当中

			fseek(MyDisk, inc, SEEK_CUR);

			if (sb->s_ninode >= 100)								//说明读取的Inode已经填满Inode索引表
			{
				break;
			}

		}

		if (sb->s_ninode <= 0)
		{
			cout << "磁盘已经没有空闲的Inode" << endl;
			return NULL;
		}
	}

	while (sb->s_ninode)		//因为在IFree过程当中可能会释放已经存在于Inode索引表的表项（当错误地使用IFree时），可能会造成同一个Inode多次分配，所以要循环查找来找到合法的空闲Inode。
	{
		ino = sb->s_inode[--sb->s_ninode];								//取sb->s_inode栈顶的元素作为分配的Inode

		fseek(MyDisk, FileSystem::INODE_ZONE_START_SECTOR*Inode::BLOCK_SIZE + (Inode::BLOCK_SIZE / FileSystem::INODE_NUMBER_PER_SECTOR)*ino, SEEK_SET);	//寻址到ino的开始位置
		DiskInode DInode;
		void* p = (void*)&DInode;
		if (!(fread(p, 1 * sizeof(DiskInode), 1, MyDisk)))
		{
			cout << "读取第" << ino << "位置的DiskInode失败" << endl;
			return NULL;
		}

		pNode->FromDiskInode(DInode);
		pNode->i_number = ino;
		if (pNode->i_mode == 0)
		{
			pNode->Clean();
			this->Update();							//更新SuperBlock
			return pNode;
		}
	}
    return NULL;
}

/**
 * @param number
 * @return void
 * @comment 释放Inode结点
 */
void FileSystem::IFree(int number) {
	SuperBlock* sb = this->f_SuperBlock;
	FILE* MyDisk = cxt.GetMyDisk();

	if (number < 0 || number >= FileSystem::INODE_NUMBER_PER_SECTOR*FileSystem::INODE_ZONE_SIZE)	//判断number是否合法
	{
		return;
	}

	unsigned int d_mode = 0;
	void* p = (void*)&d_mode;
	fseek(MyDisk, FileSystem::INODE_ZONE_START_SECTOR*Inode::BLOCK_SIZE + (Inode::BLOCK_SIZE / FileSystem::INODE_NUMBER_PER_SECTOR)*number, SEEK_SET);	//寻址到number的开始位置

	if (!(fwrite(p, 1 * sizeof(unsigned int), 1, MyDisk)))
	{
		cout << "写第" << number << "位置的d_mode失败" << endl;
		return;
	}
	if (sb->s_ninode >= 100)
	{
		return;
	}

	sb->s_inode[sb->s_ninode++] = number;
	this->Update();							//更新SuperBlock
    return;
}

/**
 * @return int
 * @comment 分配DIskBlock
 */
int FileSystem::Alloc() {
	int blkno;
	SuperBlock* sb = this->f_SuperBlock;
	FILE* MyDisk = cxt.GetMyDisk();

	
	blkno = sb->s_free[--sb->s_nfree];						//从空闲块索引表当中读取blkno

	if (blkno == 0)											//说明已经没有空闲的盘块了
	{
		sb->s_nfree = 0;
		cout << "Alloc失败，已经没有空闲的盘块了" << endl;
		return 0;

	}

	if (sb->s_nfree <= 0)									//已经没有直接管理的盘块了，需要将间接索引的盘块读取进来
	{
		int addr = (FileSystem::DATA_ZONE_START_SECTOR + (blkno - 1))*Inode::BLOCK_SIZE;	//计算第blkno个盘块的起始位置

		fseek(MyDisk, addr, SEEK_SET);
		void* p = (void*)&sb->s_nfree;

		if (!(fread(p, 1 * sizeof(int), 1, MyDisk)))						//读取第blkno个盘块的s_nfree
		{
			cout << "sb->s_nfree读取失败" << endl;
			return 0;
		}

		p = (void*)sb->s_free;												//指向New_spb->s_free的void指针

		if (!(fread(p, 100 * sizeof(int), 1, MyDisk)))					//读取第blkno个盘块的s_free数组
		{
			cout << "sb->s_free数组读取失败" << endl;
			return 0;
		}
	}

	this->Update();							//更新SuperBlock
    return blkno;
}

/**
 * @param blkno
 * @return void
 * @comment 释放DiskBlock
 */
void FileSystem::Free(int blkno) {
	SuperBlock* sb = this->f_SuperBlock;
	FILE* MyDisk = cxt.GetMyDisk();

	if (sb->s_nfree <= 0)					//如果当前已经没有空闲的盘块了
	{
		sb->s_nfree = 1;
		sb->s_free[0] = 0;	
	}
	if (sb->s_nfree >= 100)									//直接索引盘块已经超过100，需要将当前的索引表的内容写入到间接索引表当中
	{
		int addr = (FileSystem::DATA_ZONE_START_SECTOR + (blkno - 1))*Inode::BLOCK_SIZE;	//计算第blkno个盘块的起始位置

		fseek(MyDisk, addr, SEEK_SET);
		void* p = (void*)&sb->s_nfree;

		if (!(fwrite(p, 1 * sizeof(int), 1, MyDisk)))						//写入第blkno个盘块的s_nfree
		{
			cout << "sb->s_nfree写入失败" << endl;
			return;
		}

		p = (void*)sb->s_free;												//指向New_spb->s_free的void指针

		if (!(fwrite(p, 100 * sizeof(int), 1, MyDisk)))						//写入第blkno个盘块的s_free数组
		{
			cout << "sb->s_free数组写入失败" << endl;
			return;
		}
	}
	sb->s_free[sb->s_nfree++] = blkno;		//将blkno放置到空闲盘块索引表的栈顶当中

	this->Update();							//更新SuperBlock
    return;
}


