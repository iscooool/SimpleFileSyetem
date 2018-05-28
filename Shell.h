#ifndef _SHELL_H
#define _SHELL_H

#define CMD_COUNT 20
#define MAX_LENGTH 30

#define CNUM 21
#define CLENGTH 100
class Shell {

public:
	enum CmdCode {
		LS = 0,
		TOUCH = 1,
		MKDIR = 2,
		CD = 3,
		RM = 4,
		OPEN = 5,
		READ = 6,
		WRITE = 7,
		CLOSE = 8,
		LSOF = 9,
		SEEK = 10
	};
	

	char CodeTable[CMD_COUNT][MAX_LENGTH];

	Shell();
	~Shell();

	/**
	 *@ comment 运行Shell
	 */
	void RunShell();						//运行Shell

	/**
	 *@ comment 将指令转换为相应的code
	 */
	int Cmd2Code(char* cmd);				//cmd映射为Code

		

	/**
	 *@ comment 解析一行命令成一个数组 
	 */
	char* ParseCmd(char* cli);				

	/**
	*@ comment 处理单个指令
	*/
	int CmdEntrance(char* cli);						//处理单个指令

	/**
	*@ comment 列出当前目录的所有文件或者目录名
	*/
	void ls();
	/**
	 *@ comment 创建目录
	 */
	void mkdir(char* fName);
	/**
	*@ comment 改变当前目录
	*/
	void cd(char* Path);
	/**
	 *@ comment 打开文件，修改打开文件列表
	 */
	void open(char* fName);
	/**
	 *@ comment 读文件
	 */
	void read(int fd, int length);
	/**
	*@ comment 写文件
	*/
	void write(int fd, int length);
	/**
	 *@ comment 关闭文件
	 */
	void close(int fd);
	/**
	 *@ comment 输出当前已经打开的文件列表
	 */
	void lsof();
	/**
	 *@ comment 改变读写指针的offset
	 */
	void seek(int fd, int position);
};
#endif