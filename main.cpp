#include<iostream>
#include "Context.h"
#include "FileManager.h"
#include "FileSystem.h"

using namespace std;

extern Context cxt;
int main() {

	FileSystem* fs = cxt.GetFileSystem();

	FileManager* fm = cxt.GetFileManager();
	
	//fm->fformat();

	fs->LoadSuperBlock();
	//fs->IFree(11);
	//Inode* pNode = fs->IAlloc();
	int blkno = 0;
	fs->Free(6666);
	for (int i = 0; i < 150; i++)
	{
		//blkno = fs->Alloc();
	}
	blkno = fs->Alloc();
	//cout <<"分配的Inode："<< pNode->i_number << endl;
	cout << "分配的blkno：" << blkno << endl;
	return 0;
}