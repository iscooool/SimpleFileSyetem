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
 * @comment ��ʼ��
 */
void FileSystem::Initialize() {
    return;
}

/**
 * @comment ��MyDisk��offset��ȡnbytes�ֽڵ�buffer����
 */
void FileSystem::ReadFromDisk(int offset, char* buffer, int nbytes) {
	FILE* MyDisk = cxt.GetMyDisk();

	if (MyDisk && buffer&& offset >= 0 && nbytes >= 0) 
	{

	}
}

/**
 * @comment ��offsetд��nbytes�ֽڵ�buffer���ݵ�MyDisk����
 */
void FileSystem::WriteToDisk(int offset, char* buffer, int nbytes) {

}

/**
 * @return void
 * @comment ����SuperBlock���ڴ浱��
 */
void FileSystem::LoadSuperBlock() {

	SuperBlock* g_spb = cxt.GetSuperBlock();
	FILE* MyDisk = cxt.GetMyDisk();

	void* p = (void*)g_spb;								//ָ��g_spb��voidָ��

	fseek(MyDisk, 0, SEEK_SET);							//��λ��MyDisk��ͷλ��
	if (!(fread(p, Inode::BLOCK_SIZE * 2, 1, MyDisk)))	//��My��ȡ����BLOCK_SIZE��С��
	{
		cout << "LoadSuperBlockʧ��" << endl;
		return;
	}

	this->f_SuperBlock = g_spb;
	g_spb->s_flock = 0;
	g_spb->s_ilock = 0;
	return;
}

/**
 * @return SuperBlock*
 * @comment ���ص�ǰ��SuperBlock
 */
SuperBlock* FileSystem::GetFS() {
    return this->f_SuperBlock;
}

/**
 * @return void
 * @comment ����SuperBlock��Ӳ�̵���
 */
void FileSystem::Update() {

	SuperBlock* g_spb = this->f_SuperBlock;
	FILE* MyDisk = cxt.GetMyDisk();

	void* p = (void*)g_spb;								//ָ��g_spb��voidָ��

	fseek(MyDisk, 0, SEEK_SET);							//��λ��MyDisk��ͷλ��
	if (!(fwrite(p, Inode::BLOCK_SIZE * 2, 1, MyDisk)))	//��My��ȡ����BLOCK_SIZE��С��
	{
		cout << "Updateʧ��" << endl;
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
 * @comment ����Inode���
 */
Inode* FileSystem::IAlloc() {
    return NULL;
}

/**
 * @param number
 * @return void
 * @comment �ͷ�Inode���
 */
void FileSystem::IFree(int number) {
    return;
}

/**
 * @return int
 * @comment ����DIskBlock
 */
int FileSystem::Alloc() {
    return 0;
}

/**
 * @param blkno
 * @return void
 * @comment �ͷ�DiskBlock
 */
void FileSystem::Free(int blkno) {
    return;
}


