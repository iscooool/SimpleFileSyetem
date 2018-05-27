/**
 * Project Untitled
 */


#include "FileManager.h"
#include "Context.h"

/**
 * FileManager implementation
 */

extern Context cxt;
FileManager::FileManager() {

}
FileManager::~FileManager() {

}
/**
 * @return void
 */
void FileManager::Initialize() {
    return;
}

/**
 * @return void
 */
void FileManager::fformat() {

	//SuperBlock* New_spb = new SuperBlock();
	SuperBlock* New_spb = cxt.GetSuperBlock();
	FILE* MyDisk = cxt.GetMyDisk();

	New_spb->s_isize = FileSystem::INODE_ZONE_SIZE;		//外存Inode区占用的盘块数
	New_spb->s_fsize = FileSystem::DATA_ZONE_SIZE + FileSystem::INODE_ZONE_SIZE + 2;

	New_spb->s_nfree = 1;								//初始的直接管理空闲盘块数,指向s_nfree -1的位置为最上面的数据，
	New_spb->s_free[0] = 0;
	

	for (int i = FileSystem::DATA_ZONE_SIZE - 1; i >= 1; i--)//这里初始化空闲盘块索引表,注：盘块号从1开始计数
	{
		if (New_spb->s_nfree >= 100)
		{
			int addr = (FileSystem::DATA_ZONE_START_SECTOR + (i - 1))*Inode::BLOCK_SIZE;	//计算第i个盘块的起始位置

			fseek(MyDisk, addr, SEEK_SET);
			void* p = (void*)&New_spb->s_nfree;
			if (!(fwrite(p, 1*sizeof(int), 1, MyDisk)))						//New_spb->s_nfree的数据写入到第i盘块
			{
				cout << "New_spb->s_nfree写入MyDisk失败" << endl;
				return;
			}

			p = (void*)New_spb->s_free;												//指向New_spb->s_free的void指针
								
			if (!(fwrite(p, 100*sizeof(int), 1, MyDisk)))					//New_spb->s_free数组的数据写入到第i盘块
			{
				cout << "New_spb->s_free数组写入MyDisk失败" << endl;
				return;
			}
			New_spb->s_nfree = 0;
		}
		New_spb->s_free[New_spb->s_nfree++] = i;

		
	}

	New_spb->s_ninode = 100;
	for (int i = 0; i < 100; i++)
	{
		New_spb->s_inode[i] = i;

		
	}


	fseek(MyDisk, FileSystem::INODE_ZONE_START_SECTOR*Inode::BLOCK_SIZE, SEEK_SET);	//寻址到Inode区的开始位置
	unsigned int zero = 0;
	void* p = (void*)&zero;
	int inc = (Inode::BLOCK_SIZE/FileSystem::INODE_NUMBER_PER_SECTOR) - sizeof(unsigned int);
	for (int i = 0; i < FileSystem::INODE_ZONE_SIZE; i++)						//初始化Inode区每一个Inode的d_mode为0，表示空闲
	{
		
		
		if (!(fwrite(p, 1 * sizeof(unsigned int), 1, MyDisk)))						
		{
			cout << "初始化d_mode到MyDisk失败" << endl;
			return;
		}

		fseek(MyDisk, inc, SEEK_CUR);

	}
	New_spb->s_flock = 0;
	New_spb->s_ilock = 0;
	New_spb->s_fmod = 0;
	New_spb->s_time = 0;

	//cxt.c_spb = New_spb;
	cxt.c_FileSystem.f_SuperBlock = cxt.c_spb;
	cxt.c_FileSystem.Update();
    return;
}
/**
 * @param fName
 * @param mode
 * @return int
 */
int FileManager::Fcreat(char* fName, int mode) {
    return 0;
}

/**
 * @param fName
 * @param mode
 * @return int
 */
int FileManager::Fopen(char* fName, int mode) {
    return 0;
}

/**
 * @param fd
 * @return void
 */
void FileManager::Fclose(int fd) {
    return;
}

/**
 * @param fd
 * @param buffer
 * @param length
 * @return int
 */
int FileManager::Fread(int fd, char* buffer, int length) {
    return 0;
}

/**
 * @param fd
 * @param buffer
 * @param length
 * @return int
 */
int FileManager::Fwrite(int fd, char* buffer, int length) {
    return 0;
}

/**
 * @param fd
 * @param buffer
 * @param length
 * @param mode
 * @return int
 */
int FileManager::Rdwr(int fd, char* buffer, int length, enum File::FileFlags mode) {
    return 0;
}

/**
 * @param fd
 * @param position
 * @return int
 */
int FileManager::Flseek(int fd, int position) {
    return 0;
}

/**
 * @param fName
 * @return int
 */
int FileManager::Fdelete(char* fName) {
    return 0;
}

