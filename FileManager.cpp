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
	this->m_FileSystem = cxt.GetFileSystem();
	this->OpenFiles = cxt.GetOpenFiles();
	this->rootDirInode = cxt.GetrootDirInode();
    return;
}
/**
* @comment 在pwd当中找出fName对应的目录项
*/
DirectoryEntry* FileManager::FindDir(char* fName, Inode* pwd) {
	DirectoryEntry* DirEntry = new DirectoryEntry();
	int DirNo = pwd->i_size / sizeof(DirectoryEntry);		//计算当前目录的表项数量
	
	

	cxt.GetIOParameter()->m_Base = 0;
	cxt.GetIOParameter()->m_Offset = 0;
	cxt.GetIOParameter()->m_Count = sizeof(DirectoryEntry);
	cxt.c_ReadBuffer = (char*)DirEntry;
	for (int i = 0; i < DirNo; i++)								//这一部分是一个读入当前目录的每一项，对比当前目录有没有名字相同的表项
	{

		pwd->ReadI();

		if (!strcmp(fName, DirEntry->m_name))
		{
			
			return DirEntry;
		}
		//cxt.GetIOParameter()->m_Base += sizeof(DirectoryEntry);
		cxt.GetIOParameter()->m_Offset = sizeof(DirectoryEntry)*(i+1);
		cxt.GetIOParameter()->m_Count = sizeof(DirectoryEntry);
		cxt.c_ReadBuffer = (char*)DirEntry;

	}
	return NULL;
}

/**
 * @return void
 * @comment 格式化cxt当中的MyDisk磁盘
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

		New_spb->s_inode[i] = i + 1;				//没有第0个Inode是因为第0个将要被作为跟结点

	}
								
	Inode* rootDirInode = cxt.GetrootDirInode();			//初始化根节点
	rootDirInode->Clean();
	rootDirInode->i_mode = Inode::IALLOC | Inode::IFDIR;
	rootDirInode->i_number = 0;
	
	
	rootDirInode->IUpdate();



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
	Inode* pwd = cxt.GetPwd();
	DirectoryEntry DirEntry;
	int DirNo = pwd->i_size / sizeof(DirectoryEntry);		//计算当前目录的表项数量
	Inode* NewInode;
	FileSystem* fs = cxt.GetFileSystem();

	if (strlen(fName) > DirectoryEntry::DIRSIZ)
	{
		cout << "文件名超出范围(" << DirectoryEntry::DIRSIZ << ")" << endl;
		return -1;
	}

	if (this->FindDir(fName, pwd) != NULL)
	{
		cout << "文件或目录已经存在" << endl;
		return -1;
	}
	
	if ((NewInode = fs->IAlloc()) == NULL)					//申请新的Inode结点
	{
		cout << "Inode结点申请失败" << endl;
		return -1;
	}

	NewInode->i_mode = mode;								//初始化i_mode
	NewInode->IUpdate();

	strcpy(DirEntry.m_name, fName);
	DirEntry.m_ino = NewInode->i_number;
	cxt.c_ReadBuffer = (char*)(&DirEntry);
	cxt.GetIOParameter()->m_Base = 0;
	cxt.GetIOParameter()->m_Offset = sizeof(DirectoryEntry)*DirNo;
	cxt.GetIOParameter()->m_Count = sizeof(DirectoryEntry);
	pwd->WriteI();

	fs->Update();
    return NewInode->i_number;
}

/**
 * @param fName
 * @param mode
 * @return int
 */
int FileManager::Fopen(char* fName, int mode) {
	Inode* pwd = cxt.GetPwd();
	DirectoryEntry* DirEntry;
	Inode* OpenInode = new Inode();
	File* OpenFile = new File();
	int fd = -1;

	if ((DirEntry = this->FindDir(fName, pwd)) == NULL)
	{
		cout << "文件不存在" << endl;
		return 0;
	}

	OpenInode->i_number = DirEntry->m_ino;
	OpenInode->ICopy();									//打开目录表项对应的Inode

	OpenFile->f_flag = mode;							//构建对的File结构
	OpenFile->f_inode = OpenInode;
	OpenFile->f_offset = 0;

	if ((fd = this->OpenFiles->AllocFreeSlot()) == -1)
	{
		cout << "无法分配更多的文件句柄了" << endl;
		return 0;
	}

	this->OpenFiles->SetF(fd, OpenFile);
	strcpy(cxt.GetOpenFiles()->FileName[fd], fName);
	return 1;
}

/**
 * @param fd
 * @return void
 */
void FileManager::Fclose(int fd) {

	File* pFile = this->OpenFiles->GetF(fd);

	if (pFile == NULL)
	{
		cout << "文件没有处于打开状态，关闭失败" << endl;
		return;
	}

	this->OpenFiles->SetF(fd, NULL);

	delete pFile->f_inode;
	delete pFile;

    return;
}

/**
 * @param fd
 * @param buffer
 * @param length
 * @return int
 */
int FileManager::Fread(int fd, char* buffer, int length) {

	this->Rdwr(fd, buffer, length, File::FREAD);
    return 1;
}

/**
 * @param fd
 * @param buffer
 * @param length
 * @return int
 */
int FileManager::Fwrite(int fd, char* buffer, int length) {
	this->Rdwr(fd, buffer, length, File::FWRITE);
	return 1;
    
}

/**
 * @param fd
 * @param buffer
 * @param length
 * @param mode
 * @return int
 */
int FileManager::Rdwr(int fd, char* buffer, int length, enum File::FileFlags mode) {
	File* pFile = this->OpenFiles->GetF(fd);

	if (pFile == NULL)
	{
		return 0;
	}

	if ((pFile->f_flag & mode) == 0)
	{
		cout << "读写操作与文件打开模式不符" << endl;
		return 0;
	}

	cxt.GetIOParameter()->m_Base = 0;
	cxt.GetIOParameter()->m_Offset = pFile->f_offset;
	cxt.GetIOParameter()->m_Count = length;
	cxt.c_ReadBuffer = buffer;
	if (mode == File::FREAD)
	{
		pFile->f_inode->ReadI();
	}
	else
	{
		pFile->f_inode->WriteI();
	}

	pFile->f_offset += length - cxt.GetIOParameter()->m_Count;

    return 1;
}

/**
 * @param fd
 * @param position
 * @return int
 */
int FileManager::Flseek(int fd, int position) {
	File* pFile = this->OpenFiles->GetF(fd);

	if (pFile == NULL)
	{
		cout << "没有对应文件句柄" << endl;
		return 0;
	}

	if (position > pFile->f_inode->i_size)
	{
		cout << "超出文件范围" << endl;
		return 0;
	}

	pFile->f_offset = position;						//只实现了从头开始计算position

    return 1;
}

/**
 * @param fName
 * @return int
 */
int FileManager::Fdelete(char* fName) {
	Inode* pwd = cxt.GetPwd();
	FileSystem* fs = cxt.GetFileSystem();
	DirectoryEntry* DirEntry = new DirectoryEntry();
	Inode* OpenInode = new Inode();
	int BlkCount = 0;
	int fd = -1;

	int DirNo = pwd->i_size / sizeof(DirectoryEntry);		//计算当前目录的表项数量

	if (this->FindDir(fName, pwd) == NULL)
	{
		cout << "文件或目录不存在" << endl;
		return 0;
	}
	cxt.GetIOParameter()->m_Base = 0;
	cxt.GetIOParameter()->m_Offset = 0;
	cxt.GetIOParameter()->m_Count = sizeof(DirectoryEntry);
	cxt.c_ReadBuffer = (char*)DirEntry;
	for (int i = 0; i < DirNo; i++)								//这一部分是一个读入当前目录的每一项，对比当前目录有没有名字相同的表项
	{

		pwd->ReadI();

		if (!strcmp(fName, DirEntry->m_name))					//这一部分使删除的表项的后面的所以表项向前移动一个位置
		{
			Inode DeleteInode;
			DeleteInode.i_number = DirEntry->m_ino;
			DeleteInode.ICopy();
			if (((DeleteInode.i_mode & Inode::IFDIR) != 0) && DeleteInode.i_size != sizeof(DirectoryEntry)*2)	//如果是目录且不为为空
			{
				cout << "不支持删除非空目录" << endl;
				return 0;
			}


			int RemainNum = DirNo - i - 1;
			char* Buffer = new char[sizeof(DirectoryEntry)*RemainNum + 1];
			cxt.GetIOParameter()->m_Offset = sizeof(DirectoryEntry)*(i + 1);
			cxt.GetIOParameter()->m_Count = sizeof(DirectoryEntry)*RemainNum;
			cxt.c_ReadBuffer = (char*)Buffer;

			pwd->ReadI();

			cxt.GetIOParameter()->m_Offset = sizeof(DirectoryEntry)*i;
			cxt.GetIOParameter()->m_Count = sizeof(DirectoryEntry)*RemainNum;
			cxt.c_ReadBuffer = (char*)Buffer;
			pwd->WriteI();

			delete Buffer;
			
		}
		//cxt.GetIOParameter()->m_Base += sizeof(DirectoryEntry);
		cxt.GetIOParameter()->m_Offset = sizeof(DirectoryEntry)*(i+1);
		cxt.GetIOParameter()->m_Count = sizeof(DirectoryEntry);
		cxt.c_ReadBuffer = (char*)DirEntry;
		//if (i == DirNo - 1)										//文件不存在，无法删除
		//{
		//	cout << "文件不存在" << endl;
		//	return 0;
		//}

	}

	pwd->i_size -= sizeof(DirectoryEntry);					//pwd的大小减小一个表项

	OpenInode->i_number = DirEntry->m_ino;
	OpenInode->ICopy();									//打开目录表项对应的Inode

	BlkCount = OpenInode->i_size / Inode::BLOCK_SIZE;	//计算文件所占用的盘块的数量
	if (OpenInode->i_size % Inode::BLOCK_SIZE > 0)
	{
		BlkCount++;
	}

	for (int i = 0; i < BlkCount; i++)					//释放Inode所占用的盘块
	{
		int bn = OpenInode->Bmap(i);					//获取逻辑盘块对应的物理盘块
		fs->Free(bn);
	}

	fs->IFree(OpenInode->i_number);
	fs->Update();

    return 1;
}

