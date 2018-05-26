/**
 * Project Untitled
 */


#include "Context.h"

/**
 * Context implementation
 */

Context cxt;

Context::Context() {

	this->c_spb = new SuperBlock();
	this->MyDisk = fopen("MyDisk.img", "rb+");
	
}

Context::~Context() {
	delete this->c_spb;
	fclose(this->MyDisk);
}

/**
 * @return FileManager*
 */
FileManager* Context::GetFileManager() {
    return &(this->c_FileManager);
}

/**
 * @return inode*
 */
Inode* Context::GetPwd() {
    return this->c_pwd;
}

/**
 * @return char*
 */
char* Context::GetPwdPath() {
	return this->c_pwd_path;
}

/**
 * @return IOParameter*
 */
IOParameter* Context::GetIOParameter() {
	return &(this->c_IOParameter);
}

/**
 * @return FileSystem*
 */
FileSystem* Context::GetFileSystem() {
    return &(this->c_FileSystem);
}

/**
* @return FILE*
*/
FILE* Context::GetMyDisk() {
	return this->MyDisk;
}

/**
 * @return SuperBlock*
 */
SuperBlock* Context::GetSuperBlock() {
    return this->c_spb;
}



