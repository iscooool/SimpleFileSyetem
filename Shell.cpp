
#include "Shell.h"
#include "Context.h"
#include <string>
#include <iomanip>
extern Context cxt;
/**
 *@ comment 构造函数
 */
Shell::Shell() {
	cxt.GetFileManager()->Initialize();
	cxt.c_pwd = cxt.GetrootDirInode();
	cxt.c_pwd_path += "/";

	strcpy(Shell::CodeTable[0], "ls");
	strcpy(Shell::CodeTable[1], "touch");
	strcpy(Shell::CodeTable[2], "mkdir");
	strcpy(Shell::CodeTable[3], "cd");
	strcpy(Shell::CodeTable[4], "rm");
	strcpy(Shell::CodeTable[5], "open");
	strcpy(Shell::CodeTable[6], "read");
	strcpy(Shell::CodeTable[7], "write");
	strcpy(Shell::CodeTable[8], "close");
	strcpy(Shell::CodeTable[9], "lsof");
	strcpy(Shell::CodeTable[10], "seek");
	strcpy(Shell::CodeTable[11], "format");
	strcpy(Shell::CodeTable[12], "shutdown");


}
/**
*@ comment 析构函数
*/
Shell::~Shell() {

}
/**
*@ comment 运行Shell
*/
void Shell::RunShell() {
	
	char* cli = new char[CNUM*CLENGTH];

	while (true)
	{
		cout <<*(cxt.GetPwdPath())<<"# ";
		cin.getline(cli, CNUM*CLENGTH);
		if (!this->CmdEntrance(cli))
		{
			return;
		}
	}
	return;
}
/**
*@ comment 将指令转换为相应的code
*/
int Shell::Cmd2Code(char* cmd) {

	for (int i = 0; i < CMD_COUNT; i++)
	{
		if (!strcmp(cmd, Shell::CodeTable[i]))
		{
			return i;
		}
	}
	return -1;
}

/**
*@ comment 解析一行命令成一个数组
*/
char* Shell::ParseCmd(char* cli) {

	int i = 0;
	int CIndex = 0;				//CTable的下标
	int CCount = 0;				//当前处理第CCount个参数
	
	int CFlag = 0;				//用来判定是否已经完全读入一个词
	char* CTable = new char[CNUM*CLENGTH];

	if (cli == NULL)
	{
		cout << "无效字符串" << endl;
		return NULL;
	}

	for (; cli[i] != '\0'&& CCount < CNUM; i++)
	{
		if (cli[i] == ' ')		//跳过空格
		{
			if (CFlag == 1)
			{
				CTable[CLENGTH*CCount + CIndex] = '\0';
				CCount++;
				CIndex = 0;
				CFlag = 0;
			}
			continue;
		}
		else
		{
			CTable[CLENGTH*CCount + CIndex] = cli[i];		//CLENGTH*CCount为第CCount+1个参数的起始地址，CIndex为行内偏移量
			CIndex++;
			if (CFlag == 0)
			{
				CFlag = 1;
			}
			
		}

		if (CIndex >= CLENGTH)
		{
			cout << "参数长度过长" << endl;
			return NULL;
		}
	}
	CTable[CLENGTH*CCount + CIndex] = '\0';

	return CTable;
}
/**
*@ comment 处理单个指令
*/
int Shell::CmdEntrance(char* cli) {
	
	char* CTable;
	FileManager* fm = cxt.GetFileManager();

	CTable = ParseCmd(cli);

	switch (Cmd2Code(CTable))
	{
	case Shell::LS: {
		this->ls();
		break;
	}
	case Shell::TOUCH: {

		fm->Fcreat(CTable + CLENGTH,Inode::IALLOC);
		break;
	}
	case Shell::MKDIR: {
		this->mkdir(CTable + CLENGTH);
		break;

	}
	case Shell::CD: {
		this->cd(CTable + CLENGTH);
		break;
	}
	case Shell::RM: {
		fm->Fdelete(CTable + CLENGTH);
		break;
	}
	case Shell::OPEN: {
		this->open(CTable + CLENGTH);
		break;
	}
	case Shell::READ: {
		int fd = 0;
		int length = 0;
		for (int i = 1; i < CNUM - 1; i++)				//粗略处理，最后一个参数不检查，防止越界
		{												//这一部分用来获取参数
			if (!strcmp(CTable + CLENGTH*i, "-fd"))
			{
				fd = atoi(CTable + CLENGTH*(i + 1));
				continue;
			}
			if (!strcmp(CTable + CLENGTH*i, "-l"))
			{
				length = atoi(CTable + CLENGTH*(i + 1));
				continue;
			}
			if (!strcmp(CTable + CLENGTH*i, "-all"))
			{
				cxt.GetOpenFiles()->GetF(fd)->f_offset = 0;
				length = cxt.GetOpenFiles()->GetF(fd)->f_inode->i_size;
				continue;
			}

		}
		this->read(fd, length);
		break;
	}
	case Shell::WRITE: {
		int fd = 0;
		int length = 0;
		int FromFile = 0;
		char Path[CLENGTH];
		FileManager* fm = cxt.GetFileManager();
		
		for (int i = 1; i < CNUM - 1; i++)				//粗略处理，最后一个参数不检查，防止越界
		{												//这一部分用来获取参数
			if (!strcmp(CTable + CLENGTH*i, "-fd"))
			{
				fd = atoi(CTable + CLENGTH*(i + 1));
				continue;
			}
			if (!strcmp(CTable + CLENGTH*i, "-l"))
			{
				length = atoi(CTable + CLENGTH*(i + 1));
				continue;
			}
			if (!strcmp(CTable + CLENGTH*i, "-p"))
			{
				strcpy(Path, CTable + CLENGTH*(i + 1));
				FromFile = 1;
				continue;
			}
		}

		if (FromFile)
		{
			int Buffer = 0;
			FILE* InFile = fopen(Path, "rb+");
			if (!InFile)
			{
				cout << "文件打开失败" << endl;
				break;
			}
			int count = 0;
			while (Buffer != EOF)
			{
				Buffer = getc(InFile);
				char ch = Buffer;
				if (Buffer == EOF)
					break;
				fm->Fwrite(fd, &ch, 1);
				count++;
			}
			cout << "输入的字符数量为：" << count << endl;
			fclose(InFile);

		}
		else
		{
			char* Buffer = new char[length + 1];
			cin.getline(Buffer, length);
			fm->Fwrite(fd, Buffer, length);
			delete Buffer;
		}
		
		
		break;
	}
	case Shell::CLOSE: {
		int fd = 0;
	
		for (int i = 1; i < CNUM - 1; i++)				//粗略处理，最后一个参数不检查，防止越界
		{												//这一部分用来获取参数
			if (!strcmp(CTable + CLENGTH*i, "-fd"))
			{
				fd = atoi(CTable + CLENGTH*(i + 1));
				continue;
			}
			
		}
		this->close(fd);
		break;
	}
	case Shell::LSOF: {
		this->lsof();
		break;
	}
	case Shell::SEEK: {
		int fd = 0;
		int position = 0;
		for (int i = 1; i < CNUM - 1; i++)				//粗略处理，最后一个参数不检查，防止越界
		{												//这一部分用来获取参数
			if (!strcmp(CTable + CLENGTH*i, "-fd"))
			{
				fd = atoi(CTable + CLENGTH*(i + 1));
				continue;
			}
			if (!strcmp(CTable + CLENGTH*i, "-offset"))
			{
				position = atoi(CTable + CLENGTH*(i + 1));
				continue;
			}
		}
		this->seek(fd, position);
		break;
	}
	case Shell::FORMAT: {
		FileManager* fm = cxt.GetFileManager();

		fm->fformat();
		break;
	}
	case Shell::SHUTDOWN: {
		return 0;
	}
	default:
		cout << "无效命令" << endl;
		break;
	}
	return 1;
}
/**
*@ comment 列出当前目录的所有文件或者目录名
*/
void Shell::ls() {

	Inode* pwd = cxt.GetPwd();
	DirectoryEntry* DirEntry = new DirectoryEntry();
	int DirNo = pwd->i_size / sizeof(DirectoryEntry);		//计算当前目录的表项数量

	cxt.GetIOParameter()->m_Base = 0;
	cxt.GetIOParameter()->m_Offset = 0;
	cxt.GetIOParameter()->m_Count = sizeof(DirectoryEntry);
	cxt.c_ReadBuffer = (char*)DirEntry;
	for (int i = 0; i < DirNo; i++)								//这一部分是一个读入当前目录的每一项，对比当前目录有没有名字相同的表项
	{

		pwd->ReadI();
		cout<< setiosflags(ios::left) <<setw(20)<< DirEntry->m_name;
		if (i>0&&i % 5 == 0)
			cout << endl;

		//cxt.GetIOParameter()->m_Base += sizeof(DirectoryEntry);
		cxt.GetIOParameter()->m_Offset = sizeof(DirectoryEntry)*(i+1);			
		cxt.c_ReadBuffer = (char*)DirEntry;
		cxt.GetIOParameter()->m_Count = sizeof(DirectoryEntry);
		
	}
	cout << endl;
}

/**
*@ comment 创建目录
*/
void Shell::mkdir(char* fName) {
	Inode* pwd = cxt.GetPwd();
	FileManager* fm = cxt.GetFileManager();
	Inode* DirInode = new Inode();
	DirectoryEntry* DirEntry = new DirectoryEntry();

	if ((DirInode->i_number = fm->Fcreat(fName, Inode::IALLOC | Inode::IFDIR)) == NULL)
	{
		return;
	}
	DirInode->ICopy();

	DirEntry->m_ino = DirInode->i_number;							//将当前目录 "." 写入目录表
	strcpy(DirEntry->m_name, ".");
	cxt.GetIOParameter()->m_Base = 0;
	cxt.GetIOParameter()->m_Offset = 0;
	cxt.GetIOParameter()->m_Count = sizeof(DirectoryEntry);
	cxt.c_ReadBuffer = (char*)DirEntry;
	DirInode->WriteI();

	DirEntry->m_ino = pwd->i_number;								//将上级目录 ".." 写入目录表
	strcpy(DirEntry->m_name, "..");
	cxt.GetIOParameter()->m_Base = 0;
	cxt.GetIOParameter()->m_Offset = sizeof(DirectoryEntry);
	cxt.GetIOParameter()->m_Count = sizeof(DirectoryEntry);
	cxt.c_ReadBuffer = (char*)DirEntry;
	DirInode->WriteI();



	delete DirInode;
}

/**
*@ comment 改变当前目录,只支持相对路径寻址
*/
void Shell::cd(char* Path) {
	DirectoryEntry* DirEntry;
	FileManager* fm = cxt.GetFileManager();
	Inode* pwd = cxt.GetPwd();
	string* PwdPath = cxt.GetPwdPath();
	if (Path == NULL)
	{
		return;
	}

	if (!strcmp(Path, "."))							//cd到当前目录，什么事都不用做
	{
		return;
	}

	if (!strcmp(Path, ".."))						//cd到前一目录
	{
		if ((DirEntry = fm->FindDir("..", pwd)) == NULL)
		{
			cout << "文件或目录不存在" << endl;
			return;
		}

		

		pwd->i_number = DirEntry->m_ino;					// 改变当前路径pwd的Inode
		pwd->ICopy();

		
		
		int SlashCount = 0;
		for (int i = PwdPath->length() - 1; i >= 0; i--)	//修改当前路径字符串
		{
			if (PwdPath->compare(i,1,"/") == 0 )
			{
				//PwdPath[i] = '\0';
				SlashCount++;
			}
			if (SlashCount == 2)
			{
				*PwdPath = PwdPath->substr(0, i + 1);
				
				break;
			}
		}
		return;
	}

	
	if ((DirEntry = fm->FindDir(Path, pwd)) == NULL)
	{
		cout << "文件或目录不存在" << endl;
		return;
	}
	int TempIno = pwd->i_number;

	pwd->i_number = DirEntry->m_ino;					// 改变当前路径pwd的Inode
	pwd->ICopy();

	if ((pwd->i_mode & Inode::IFDIR) == 0)
	{
		pwd->i_number = TempIno;
		pwd->ICopy();
		cout << "不是目录" << endl;
		return;
	}

	

	PwdPath->append(Path);
	PwdPath->append("/");
	
}
/**
*@ comment 打开文件，修改打开文件列表
*/
void Shell::open(char* fName) {
	FileManager* fm = cxt.GetFileManager();
	fm->Fopen(fName, File::FREAD | File::FWRITE);	
	return;
}
/**
*@ comment 读文件
*/
void Shell::read(int fd, int length) {
	FileManager* fm = cxt.GetFileManager();
	File* OpenFile = cxt.GetOpenFiles()->GetF(fd);
	if ((length + OpenFile->f_offset) > OpenFile->f_inode->i_size)		//如果读取的长度length加上偏移量大于文件的大小
	{
		length = OpenFile->f_inode->i_size - OpenFile->f_offset;
	}
	char* Buffer = new char[2];
	for (int i = 0; i < length; i++)
	{
		fm->Fread(fd, Buffer, 1);
		Buffer[1] = '\0';
		cout << Buffer;
	}
	cout << endl;
	//fm->Fread(fd, Buffer, length);
	//Buffer[length] = '\0';
	//cout << Buffer << endl;							//输出Buffer内容
	delete Buffer;
	return;
}
/**
*@ comment 写文件
*/
void Shell::write(int fd, int length) {
	
	return;
}
/**
*@ comment 关闭文件
*/
void Shell::close(int fd) {
	FileManager* fm = cxt.GetFileManager();
	fm->Fclose(fd);
	return;
}
/**
*@ comment 输出当前已经打开的文件列表
*/
void Shell::lsof() {
	OpenFiles* FilesTable = cxt.GetOpenFiles();

	for (int i = 0; i < OpenFiles::NOFILES; i++)
	{

		File* pFile;
		pFile = FilesTable->ProccessOpenFileTable[i];

		if (pFile == NULL)
			continue;

		cout << "fd:" << setiosflags(ios::left) << setw(5) << i << FilesTable->FileName[i] << endl;
	}
}
/**
 *@ comment 改变读写指针的offset
 */
void Shell::seek(int fd, int position) {
	FileManager* fm = cxt.GetFileManager();
	fm->Flseek(fd, position);
	return;
}
