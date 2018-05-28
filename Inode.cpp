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
 * @comment �Ե�ǰInode���ж��룬�����IO������Context����
 */
void Inode::ReadI() {

	int lbn;								//�ļ����߼����
	int bn;									//lbn��Ӧ��������
	int offset;								//��ǰ�����ƫ����
	int nbytes;								//������ֽ���
	void* Buffer = (void*)cxt.c_ReadBuffer;	//��������ݻ�����

	if (!Buffer)
	{
		cout << "��������Ч" << endl;
		return;
	}
	FILE* MyDisk = cxt.GetMyDisk();

	if (cxt.GetIOParameter()->m_Count <= 0)
	{
		return;
	}

	while (cxt.GetIOParameter()->m_Count > 0)
	{
		lbn = cxt.GetIOParameter()->m_Offset / Inode::BLOCK_SIZE;			//�����߼����
		offset= cxt.GetIOParameter()->m_Offset % Inode::BLOCK_SIZE;			//�������ƫ����

		nbytes = Inode::BLOCK_SIZE - offset;								//nbytes��2�������
																			//1.������ֽ���û�г�����ǰ���ĩβ
																			//2.������ֽ����������һ���̿�ı߽�
		if (nbytes > cxt.GetIOParameter()->m_Count)
			nbytes = cxt.GetIOParameter()->m_Count;

		int remain = this->i_size - cxt.GetIOParameter()->m_Offset;

		if (nbytes > remain)
			nbytes = remain;

		if ((bn = this->Bmap(lbn)) == 0)									//��ȡ�߼��̿�lbn��Ӧ�������̿�bn
		{
			return;
		}

		int addr = (FileSystem::DATA_ZONE_START_SECTOR + (bn - 1))*Inode::BLOCK_SIZE + offset;	//�����blkno���̿����ʼλ��
		fseek(MyDisk, addr, SEEK_SET);
		if (!(fread(Buffer, nbytes, 1, MyDisk)))						//��ȡ��blkno���̿��s_nfree
		{
			cout <<"��"<<bn<< "���̿��ȡʧ��" << endl;
			return;
		}
		
		Buffer = (void*)((char*)Buffer + nbytes);						//Bufferָ��Ļ�����Ҳ����offset

		cxt.GetIOParameter()->m_Base += nbytes;
		cxt.GetIOParameter()->m_Offset += nbytes;
		cxt.GetIOParameter()->m_Count -= nbytes;

	}
    return;
}

/**
 * @return void
 * @comment �Ե�ǰInode����д�룬д���IO������Context����
 */
void Inode::WriteI() {

	int lbn;								//�ļ����߼����
	int bn;									//lbn��Ӧ��������
	int offset;								//��ǰ�����ƫ����
	int nbytes;								//������ֽ���
	void* Buffer = (void*)cxt.c_ReadBuffer;	//��������ݻ�����

	if (!Buffer)
	{
		cout << "��������Ч" << endl;
		return;
	}
	FILE* MyDisk = cxt.GetMyDisk();
	if (cxt.GetIOParameter()->m_Count <= 0)	//û�ж���д��
	{
		return;
	}

	while (cxt.GetIOParameter()->m_Count > 0)
	{
		lbn = cxt.GetIOParameter()->m_Offset / Inode::BLOCK_SIZE;			//�����߼����
		offset = cxt.GetIOParameter()->m_Offset % Inode::BLOCK_SIZE;			//�������ƫ����

		nbytes = Inode::BLOCK_SIZE - offset;								//nbytes��2�������
																			//1.д����ֽ���û�г�����ǰ���ĩβ
																			//2.д����ֽ����������һ���̿�ı߽�
		if (nbytes > cxt.GetIOParameter()->m_Count)
			nbytes = cxt.GetIOParameter()->m_Count;

		

		if ((bn = this->Bmap(lbn)) == 0)									//��ȡ�߼��̿�lbn��Ӧ�������̿�bn�����а����µ��̿������
		{
			return;
		}

		int addr = (FileSystem::DATA_ZONE_START_SECTOR + (bn - 1))*Inode::BLOCK_SIZE + offset;	//�����blkno���̿����ʼλ��
		fseek(MyDisk, addr, SEEK_SET);
		if (!(fwrite(Buffer, nbytes, 1, MyDisk)))						//��ȡ��blkno���̿��s_nfree
		{
			cout << "��" << bn << "���̿�д��ʧ��" << endl;
			return;
		}

		Buffer = (void*)((char*)Buffer + nbytes);							//Bufferָ��Ļ�����Ҳ����offset

		cxt.GetIOParameter()->m_Base += nbytes;
		cxt.GetIOParameter()->m_Offset += nbytes;
		cxt.GetIOParameter()->m_Count -= nbytes;

		if (this->i_size < cxt.GetIOParameter()->m_Offset)					//���д�뷶Χ�����ļ��Ĵ�С���������ļ���С
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

	FILE* MyDisk = cxt.GetMyDisk();
	int ino = this->i_number;

	if (ino < 0 || ino >= FileSystem::INODE_NUMBER_PER_SECTOR*FileSystem::INODE_ZONE_SIZE)	//�ж�ino�Ƿ�Ϸ�
	{
		cout << "Icopy():���ָ��Inode��Ų��Ϸ�" << endl;
		return;
	}

	fseek(MyDisk, FileSystem::INODE_ZONE_START_SECTOR*Inode::BLOCK_SIZE + (Inode::BLOCK_SIZE / FileSystem::INODE_NUMBER_PER_SECTOR)*ino, SEEK_SET);	//Ѱַ��ino�Ŀ�ʼλ��
	DiskInode DInode;
	void* p = (void*)&DInode;
	if (!(fread(p, 1 * sizeof(DiskInode), 1, MyDisk)))
	{
		cout << "��ȡ��" << ino << "λ�õ�DiskInodeʧ��" << endl;
		return;
	}

	this->FromDiskInode(DInode);
    return;
}

/**
* @comment �������ڴ�Inode�ַ�������Ϣ���������Inode��
*
*/
void Inode::IUpdate() {

	FILE* MyDisk = cxt.GetMyDisk();
	int ino = this->i_number;

	if (ino < 0 || ino >= FileSystem::INODE_NUMBER_PER_SECTOR*FileSystem::INODE_ZONE_SIZE)	//�ж�ino�Ƿ�Ϸ�
	{
		cout << "IUpdate():���ָ��Inode��Ų��Ϸ�" << endl;
		return;
	}
	DiskInode DInode = this->ToDiskInode();

	fseek(MyDisk, FileSystem::INODE_ZONE_START_SECTOR*Inode::BLOCK_SIZE + (Inode::BLOCK_SIZE / FileSystem::INODE_NUMBER_PER_SECTOR)*ino, SEEK_SET);	//Ѱַ��ino�Ŀ�ʼλ��
	void* p = (void*)&DInode;
	if (!(fwrite(p, 1 * sizeof(DiskInode), 1, MyDisk)))
	{
		cout << "д���" << ino << "λ�õ�DiskInodeʧ��" << endl;
		return;
	}

	
	return;
}
/**
 * @param lbn
 * @return int
 * @comment ���߼��̿�ӳ�䵽�����̿�
 */
int Inode::Bmap(int lbn) {

	int FirstBlkno;
	int SecondBlkno;
	FileSystem* fileSys = cxt.GetFileSystem();
	FILE* MyDisk = cxt.GetMyDisk();

	int phyBlkno;										//ת���õ��������̿��
	int iTable[Inode::ADDRESS_PER_INDEX_BLOCK] = {0};	//���ڷ��������̿���һ�μ�ӡ����μ�������� 
	int index;											//Ϊ�����̿���ֱ��������ı�����ڴ����ļ��;����ļ����У�

	if (lbn < 6)										//С���ļ����̿�ֱ�������������
	{
		phyBlkno = this->i_addr[lbn];

		if (phyBlkno == 0 && (FirstBlkno = fileSys->Alloc()) != 0)		//�����Ӧ��������û�з����̿�ţ����ܷ��䵽�µ��̿�
		{
			this->i_addr[lbn] = FirstBlkno;
			phyBlkno = FirstBlkno;
		}

		return phyBlkno;
	}
	else
	{
		if (lbn < Inode::LARGE_FILE_BLOCK)		//�����ļ�����7��8������Ϊһ���������
		{
			index = (lbn - Inode::SMALL_FILE_BLOCK) / Inode::ADDRESS_PER_INDEX_BLOCK + 6;
		}
		else                                   //�����ļ�����9��10������Ϊ�����������
		{
			index = (lbn - Inode::LARGE_FILE_BLOCK) / (Inode::ADDRESS_PER_INDEX_BLOCK * Inode::ADDRESS_PER_INDEX_BLOCK) + 8;
		}

		phyBlkno = this->i_addr[index];
		FirstBlkno = phyBlkno;


		if (phyBlkno == 0)						//phyBlkno��Ӧ����û�з���
		{
			if ((FirstBlkno = fileSys->Alloc()) == 0)	//����һ�������̿��phyBlkno��Ӧ�ı���
			{
				return 0;
			}
			this->i_addr[index] = FirstBlkno;
			

		}
		else
		{
			int addr = (FileSystem::DATA_ZONE_START_SECTOR + (FirstBlkno - 1))*Inode::BLOCK_SIZE;	//�����phyBlkno���̿����ʼλ��
			fseek(MyDisk, addr, SEEK_SET);
			if (!(fread(iTable, Inode::ADDRESS_PER_INDEX_BLOCK*sizeof(int), 1, MyDisk)))			
			{
				cout << "��" << phyBlkno << "���̿����ʧ��" << endl;
				return 0;
			}
		}

		if (index >= 8)								//�����ļ�����ǰ��iTable������ֱ����������Ҫ�ٶ���һ��
		{
			index = ((lbn - Inode::LARGE_FILE_BLOCK) / Inode::ADDRESS_PER_INDEX_BLOCK) % Inode::ADDRESS_PER_INDEX_BLOCK;
			phyBlkno = iTable[index];

			if (phyBlkno == 0)						//��������������Ӧ�ı���û�з����̿�
			{
				if ((SecondBlkno = fileSys->Alloc()) == 0)	//����һ�������̿��phyBlkno��Ӧ�ı���
				{
					return 0;
				}
				
				iTable[index] = SecondBlkno;

				int addr = (FileSystem::DATA_ZONE_START_SECTOR + (FirstBlkno - 1))*Inode::BLOCK_SIZE;	
				fseek(MyDisk, addr, SEEK_SET);
				if (!(fwrite(iTable, Inode::ADDRESS_PER_INDEX_BLOCK * sizeof(int), 1, MyDisk)))			//��iTableд�뵽��ӵ�һ����������
				{
					cout << "��" << SecondBlkno << "���̿�д��ʧ��" << endl;
					return 0;
				}

				for (int i = 0; i < Inode::ADDRESS_PER_INDEX_BLOCK; i++)			//����ȫ��0����Ϊ������ı����Ӧ�Ķ����������̿�Ϊ��
				{
					iTable[i] = 0;
				}
			}
			else
			{
				SecondBlkno = phyBlkno;
				int addr = (FileSystem::DATA_ZONE_START_SECTOR + (SecondBlkno - 1))*Inode::BLOCK_SIZE;	//�����phyBlkno���̿����ʼλ��
				fseek(MyDisk, addr, SEEK_SET);
				if (!(fread(iTable, Inode::ADDRESS_PER_INDEX_BLOCK * sizeof(int), 1, MyDisk)))
				{
					cout << "��" << phyBlkno << "���̿����ʧ��" << endl;
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


			int addr = (FileSystem::DATA_ZONE_START_SECTOR + (FirstBlkno - 1))*Inode::BLOCK_SIZE;	//�����phyBlkno���̿����ʼλ��
			fseek(MyDisk, addr, SEEK_SET);
			if (!(fread(iTable, Inode::ADDRESS_PER_INDEX_BLOCK * sizeof(int), 1, MyDisk)))
			{
				cout << "��" << FirstBlkno << "���̿����ʧ��" << endl;
				return 0;
			}
		}
		return phyBlkno;

	}
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
/**
* @comment ��Inode��ֵ����DiskInode
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

