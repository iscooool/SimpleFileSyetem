/**
 * Project Untitled
 */


#include "Inode.h"
#include "Context.h"

/**
 * Inode implementation
 */

extern Context cxt;
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

	int lbn;								//文件的逻辑块号
	int bn;									//lbn对应的物理块号
	int offset;								//当前读入的偏移量
	int nbytes;								//读入的字节数
	void* Buffer = (void*)cxt.c_ReadBuffer;	//读入的数据缓冲区

	if (!Buffer)
	{
		cout << "缓存区无效" << endl;
		return;
	}
	FILE* MyDisk = cxt.GetMyDisk();

	if (cxt.GetIOParameter()->m_Count <= 0)
	{
		return;
	}

	while (cxt.GetIOParameter()->m_Count > 0)
	{
		lbn = cxt.GetIOParameter()->m_Offset / Inode::BLOCK_SIZE;			//计算逻辑块号
		offset= cxt.GetIOParameter()->m_Offset % Inode::BLOCK_SIZE;			//计算块内偏移量

		nbytes = Inode::BLOCK_SIZE - offset;								//nbytes有2种情况：
																			//1.读入的字节数没有超过当前块的末尾
																			//2.读入的字节数横跨至少一个盘块的边界
		if (nbytes > cxt.GetIOParameter()->m_Count)
			nbytes = cxt.GetIOParameter()->m_Count;

		int remain = this->i_size - cxt.GetIOParameter()->m_Offset;

		if (nbytes > remain)
			nbytes = remain;

		if ((bn = this->Bmap(lbn)) == 0)									//获取逻辑盘块lbn对应的物理盘块bn
		{
			return;
		}

		int addr = (FileSystem::DATA_ZONE_START_SECTOR + (bn - 1))*Inode::BLOCK_SIZE + offset;	//计算第blkno个盘块的起始位置
		fseek(MyDisk, addr, SEEK_SET);
		if (!(fread(Buffer, nbytes, 1, MyDisk)))						//读取第blkno个盘块的s_nfree
		{
			cout <<"第"<<bn<< "块盘块读取失败" << endl;
			return;
		}
		
		Buffer = (void*)((char*)Buffer + nbytes);						//Buffer指向的缓冲区也加上offset

		cxt.GetIOParameter()->m_Base += nbytes;
		cxt.GetIOParameter()->m_Offset += nbytes;
		cxt.GetIOParameter()->m_Count -= nbytes;

	}
    return;
}

/**
 * @return void
 * @comment 对当前Inode进行写入，写入的IO参数在Context当中
 */
void Inode::WriteI() {

	int lbn;								//文件的逻辑块号
	int bn;									//lbn对应的物理块号
	int offset;								//当前读入的偏移量
	int nbytes;								//读入的字节数
	void* Buffer = (void*)cxt.c_ReadBuffer;	//读入的数据缓冲区

	if (!Buffer)
	{
		cout << "缓存区无效" << endl;
		return;
	}
	FILE* MyDisk = cxt.GetMyDisk();
	if (cxt.GetIOParameter()->m_Count <= 0)	//没有东西写入
	{
		return;
	}

	while (cxt.GetIOParameter()->m_Count > 0)
	{
		lbn = cxt.GetIOParameter()->m_Offset / Inode::BLOCK_SIZE;			//计算逻辑块号
		offset = cxt.GetIOParameter()->m_Offset % Inode::BLOCK_SIZE;			//计算块内偏移量

		nbytes = Inode::BLOCK_SIZE - offset;								//nbytes有2种情况：
																			//1.写入的字节数没有超过当前块的末尾
																			//2.写入的字节数横跨至少一个盘块的边界
		if (nbytes > cxt.GetIOParameter()->m_Count)
			nbytes = cxt.GetIOParameter()->m_Count;

		

		if ((bn = this->Bmap(lbn)) == 0)									//获取逻辑盘块lbn对应的物理盘块bn，其中包含新的盘块的申请
		{
			return;
		}

		int addr = (FileSystem::DATA_ZONE_START_SECTOR + (bn - 1))*Inode::BLOCK_SIZE + offset;	//计算第blkno个盘块的起始位置
		fseek(MyDisk, addr, SEEK_SET);
		if (!(fwrite(Buffer, nbytes, 1, MyDisk)))						//读取第blkno个盘块的s_nfree
		{
			cout << "第" << bn << "块盘块写入失败" << endl;
			return;
		}

		Buffer = (void*)((char*)Buffer + nbytes);							//Buffer指向的缓冲区也加上offset

		cxt.GetIOParameter()->m_Base += nbytes;
		cxt.GetIOParameter()->m_Offset += nbytes;
		cxt.GetIOParameter()->m_Count -= nbytes;

		if (this->i_size < cxt.GetIOParameter()->m_Offset)					//如果写入范围大于文件的大小，则增加文件大小
		{
			this->i_size = cxt.GetIOParameter()->m_Offset;
		}

	}

	cxt.GetFileSystem()->Update();
	this->IUpdate();
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

	FILE* MyDisk = cxt.GetMyDisk();
	int ino = this->i_number;

	if (ino < 0 || ino >= FileSystem::INODE_NUMBER_PER_SECTOR*FileSystem::INODE_ZONE_SIZE)	//判断ino是否合法
	{
		cout << "Icopy():结点指向Inode编号不合法" << endl;
		return;
	}

	fseek(MyDisk, FileSystem::INODE_ZONE_START_SECTOR*Inode::BLOCK_SIZE + (Inode::BLOCK_SIZE / FileSystem::INODE_NUMBER_PER_SECTOR)*ino, SEEK_SET);	//寻址到ino的开始位置
	DiskInode DInode;
	void* p = (void*)&DInode;
	if (!(fread(p, 1 * sizeof(DiskInode), 1, MyDisk)))
	{
		cout << "读取第" << ino << "位置的DiskInode失败" << endl;
		return;
	}

	this->FromDiskInode(DInode);
    return;
}

/**
* @comment 将包含内存Inode字符块中信息拷贝到外存Inode中
*
*/
void Inode::IUpdate() {

	FILE* MyDisk = cxt.GetMyDisk();
	int ino = this->i_number;

	if (ino < 0 || ino >= FileSystem::INODE_NUMBER_PER_SECTOR*FileSystem::INODE_ZONE_SIZE)	//判断ino是否合法
	{
		cout << "IUpdate():结点指向Inode编号不合法" << endl;
		return;
	}
	DiskInode DInode = this->ToDiskInode();

	fseek(MyDisk, FileSystem::INODE_ZONE_START_SECTOR*Inode::BLOCK_SIZE + (Inode::BLOCK_SIZE / FileSystem::INODE_NUMBER_PER_SECTOR)*ino, SEEK_SET);	//寻址到ino的开始位置
	void* p = (void*)&DInode;
	if (!(fwrite(p, 1 * sizeof(DiskInode), 1, MyDisk)))
	{
		cout << "写入第" << ino << "位置的DiskInode失败" << endl;
		return;
	}

	
	return;
}
/**
 * @param lbn
 * @return int
 * @comment 将逻辑盘块映射到物理盘块
 */
int Inode::Bmap(int lbn) {

	int FirstBlkno;
	int SecondBlkno;
	FileSystem* fileSys = cxt.GetFileSystem();
	FILE* MyDisk = cxt.GetMyDisk();

	int phyBlkno;										//转换得到的物理盘块号
	int iTable[Inode::ADDRESS_PER_INDEX_BLOCK] = {0};	//用于访问索引盘块中一次间接、两次间接索引表 
	int index;											//为物理盘块在直接索引表的表项（用于大型文件和巨型文件当中）

	if (lbn < 6)										//小型文件，盘块直接由索引表管理
	{
		phyBlkno = this->i_addr[lbn];

		if (phyBlkno == 0 && (FirstBlkno = fileSys->Alloc()) != 0)		//如果对应索引表项没有分配盘块号，且能分配到新的盘块
		{
			this->i_addr[lbn] = FirstBlkno;
			phyBlkno = FirstBlkno;
		}

		return phyBlkno;
	}
	else
	{
		if (lbn < Inode::LARGE_FILE_BLOCK)		//大型文件，第7、8个表项为一级间接索引
		{
			index = (lbn - Inode::SMALL_FILE_BLOCK) / Inode::ADDRESS_PER_INDEX_BLOCK + 6;
		}
		else                                   //巨型文件，第9、10个表项为二级间接索引
		{
			index = (lbn - Inode::LARGE_FILE_BLOCK) / (Inode::ADDRESS_PER_INDEX_BLOCK * Inode::ADDRESS_PER_INDEX_BLOCK) + 8;
		}

		phyBlkno = this->i_addr[index];
		FirstBlkno = phyBlkno;


		if (phyBlkno == 0)						//phyBlkno对应表项没有分配
		{
			if ((FirstBlkno = fileSys->Alloc()) == 0)	//申请一个空闲盘块给phyBlkno对应的表项
			{
				return 0;
			}
			this->i_addr[index] = FirstBlkno;
			

		}
		else
		{
			int addr = (FileSystem::DATA_ZONE_START_SECTOR + (FirstBlkno - 1))*Inode::BLOCK_SIZE;	//计算第phyBlkno个盘块的起始位置
			fseek(MyDisk, addr, SEEK_SET);
			if (!(fread(iTable, Inode::ADDRESS_PER_INDEX_BLOCK*sizeof(int), 1, MyDisk)))			
			{
				cout << "第" << phyBlkno << "块盘块读入失败" << endl;
				return 0;
			}
		}

		if (index >= 8)								//巨型文件，当前的iTable还不是直接索引表，需要再读入一次
		{
			index = ((lbn - Inode::LARGE_FILE_BLOCK) / Inode::ADDRESS_PER_INDEX_BLOCK) % Inode::ADDRESS_PER_INDEX_BLOCK;
			phyBlkno = iTable[index];

			if (phyBlkno == 0)						//二级间接索引表对应的表项没有分配盘块
			{
				if ((SecondBlkno = fileSys->Alloc()) == 0)	//申请一个空闲盘块给phyBlkno对应的表项
				{
					return 0;
				}
				
				iTable[index] = SecondBlkno;

				int addr = (FileSystem::DATA_ZONE_START_SECTOR + (FirstBlkno - 1))*Inode::BLOCK_SIZE;	
				fseek(MyDisk, addr, SEEK_SET);
				if (!(fwrite(iTable, Inode::ADDRESS_PER_INDEX_BLOCK * sizeof(int), 1, MyDisk)))			//将iTable写入到间接第一级索引表中
				{
					cout << "第" << SecondBlkno << "块盘块写入失败" << endl;
					return 0;
				}

				for (int i = 0; i < Inode::ADDRESS_PER_INDEX_BLOCK; i++)			//这里全置0是因为新申请的表项对应的二级索引表盘块为空
				{
					iTable[i] = 0;
				}
			}
			else
			{
				SecondBlkno = phyBlkno;
				int addr = (FileSystem::DATA_ZONE_START_SECTOR + (SecondBlkno - 1))*Inode::BLOCK_SIZE;	//计算第phyBlkno个盘块的起始位置
				fseek(MyDisk, addr, SEEK_SET);
				if (!(fread(iTable, Inode::ADDRESS_PER_INDEX_BLOCK * sizeof(int), 1, MyDisk)))
				{
					cout << "第" << phyBlkno << "块盘块读入失败" << endl;
					return 0;
				}
			}
			FirstBlkno = SecondBlkno;

		}

		if (lbn < Inode::LARGE_FILE_BLOCK)
		{
			index = (lbn - Inode::SMALL_FILE_BLOCK) % Inode::ADDRESS_PER_INDEX_BLOCK;
		}
		else
		{
			index = (lbn - Inode::LARGE_FILE_BLOCK) % Inode::ADDRESS_PER_INDEX_BLOCK;
		}

		if ((phyBlkno = iTable[index]) == 0 && (SecondBlkno = fileSys->Alloc()) != 0)
		{
			phyBlkno = SecondBlkno;
			iTable[index] = phyBlkno;


			int addr = (FileSystem::DATA_ZONE_START_SECTOR + (FirstBlkno - 1))*Inode::BLOCK_SIZE;	//计算第phyBlkno个盘块的起始位置
			fseek(MyDisk, addr, SEEK_SET);
			if (!(fread(iTable, Inode::ADDRESS_PER_INDEX_BLOCK * sizeof(int), 1, MyDisk)))
			{
				cout << "第" << FirstBlkno << "块盘块读入失败" << endl;
				return 0;
			}
		}
		return phyBlkno;

	}
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
/**
* @comment 将Inode的值赋给DiskInode
*
*/
DiskInode Inode::ToDiskInode() {
	DiskInode DInode;
	DInode.d_mode = this->i_mode;
	DInode.d_uid = this->i_uid;
	DInode.d_gid = this->i_gid;
	DInode.d_size = this->i_size;

	for (int i = 0; i < 10; i++)
	{
		DInode.d_addr[i] = this->i_addr[i];
	}
	return DInode;
}

