#include<iostream>
#include "Context.h"
#include "FileManager.h"
#include "FileSystem.h"
#include "Shell.h"
using namespace std;

extern Context cxt;
int main() {

	FileSystem* fs = cxt.GetFileSystem();

	FileManager* fm = cxt.GetFileManager();
	
	//fm->fformat();

	//fs->LoadSuperBlock();

	Shell sh;
	sh.RunShell();

	/*Inode* pNode = fs->IAlloc();
	pNode->i_mode |= Inode::IALLOC;
	cxt.GetIOParameter()->m_Base = 0;
	cxt.GetIOParameter()->m_Offset = 0;
	cxt.GetIOParameter()->m_Count = 10;
	char Buffer[11] = "MyDisk";

	cxt.c_ReadBuffer = Buffer;
	pNode->WriteI();
	pNode->IUpdate();

	cout << "Inode编号：" << pNode->i_number << endl;*/

	/*Inode* RInode = new Inode();
	RInode->i_number = 98;
	RInode->ICopy();

	cxt.GetIOParameter()->m_Base = 0;
	cxt.GetIOParameter()->m_Offset = 0;
	cxt.GetIOParameter()->m_Count = 10;
	char Buffer[11];

	cxt.c_ReadBuffer = Buffer;
	RInode->ReadI();
	Buffer[10] = '\0';
	cout << "读入内容为：" << Buffer << endl;*/


	//int blkno = 0;
	//fs->Free(6666);
	//for (int i = 0; i < 150; i++)
	//{
	//	//blkno = fs->Alloc();
	//}
	//blkno = fs->Alloc();
	////cout <<"分配的Inode："<< pNode->i_number << endl;
	//cout << "分配的blkno：" << blkno << endl;
	return 0;
}