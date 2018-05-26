/**
 * Project Untitled
 */


#include "FileManager.h"

/**
 * FileManager implementation
 */


FileManager::FileManager() {

}
FileManager::~FileManager() {

}
/**
 * @return void
 */
void FileManager::Initialize() {
    return;
}

/**
 * @return void
 */
void FileManager::fformat() {
    return;
}

/**
 * @param fName
 * @param mode
 * @return int
 */
int FileManager::fcreat(char* fName, int mode) {
    return 0;
}

/**
 * @param fName
 * @param mode
 * @return int
 */
int FileManager::fopen(char* fName, int mode) {
    return 0;
}

/**
 * @param fd
 * @return void
 */
void FileManager::fclose(int fd) {
    return;
}

/**
 * @param fd
 * @param buffer
 * @param length
 * @return int
 */
int FileManager::fread(int fd, char* buffer, int length) {
    return 0;
}

/**
 * @param fd
 * @param buffer
 * @param length
 * @return int
 */
int FileManager::fwrite(int fd, char* buffer, int length) {
    return 0;
}

/**
 * @param fd
 * @param buffer
 * @param length
 * @param mode
 * @return int
 */
int FileManager::Rdwr(int fd, char* buffer, int length, enum FileFlags mode) {
    return 0;
}

/**
 * @param fd
 * @param position
 * @return int
 */
int FileManager::flseek(int fd, int position) {
    return 0;
}

/**
 * @param fName
 * @return int
 */
int FileManager::fdelete(char* fName) {
    return 0;
}

void FileManager::FileManager() {

}