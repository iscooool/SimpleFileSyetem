/**
 * Project Untitled
 */


#ifndef _INODE_H
#define _INODE_H

class Inode {
public: 
    enum InodeFlag;
    static const unsigned int IFMY = 0x6000;
    static const unsigned int IFDIR = 0x4000;
    static const unsigned int ILARG = 0x1000;
    static const unsigned int IREAD = 0x100;
    static const unsigned int IWRITE = 0x80;
    static const unsigned int IEXEC = 0x40;
    static const unsigned int IRWXU = (IREAD|IWRITE|IEXEC);
    static const unsigned int IRWXG = ((IRWXU) >> 3);
    static const unsigned int IRWXO = ((IRWXU) >> 6);
    static const int BLOCK_SIZE = 512;
    static const int ADDRESS_PER_INDEX_BLOCK = BLOCK_SIZE / sizeof(int);
    static const int SMALL_FILE_BLOCK = 6;
    static const int LARGE_FILE_BLOCK = 128 * 2 + 6;
    static const int HUGE_FILE_BLOCK = 128 * 128 * 2 + 128 * 2 + 6;


	unsigned int i_mode;	/* �ļ�������ʽ��Ϣ */
	int	i_number;			/* ���inode���еı�� */
    short i_uid;			/* �ļ������ߵ��û���ʶ�� */
    short i_gid;			/* �ļ������ߵ����ʶ�� */
    int i_size;				/* �ļ���С���ֽ�Ϊ��λ */
    int i_addr[10];			/* �����ļ��߼���ú�������ת���Ļ��������� */
    
    
    /*
	 * @comment �Ե�ǰInode���ж��룬�����IO������Context����
	 *
	 */
    void ReadI();

	/*
	* @comment �Ե�ǰInode����д�룬д���IO������Context����
	*
	*/
    void WriteI();
    
	/*
	* @comment ����Inode�Ĳ���
	*
	*/
    void Clean();
    
	/*
	* @comment ���������Inode�ַ�������Ϣ�������ڴ�Inode��
	*
	*/
    void ICopy();
    
    /**
     * @param lbn
	 * @comment ���߼��̿�ӳ�䵽�����̿�
     */
    int Bmap(int lbn);
public:
	Inode();
	~Inode();
};

#endif //_INODE_H