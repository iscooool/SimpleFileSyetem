/**
 * Project Untitled
 */

#include<iostream>
#include "File.h"


/**
 * File implementation
 */


File::File() {
	
	this->f_flag = 0;
	this->f_offset = 0;
	this->f_inode = NULL;
}

File::~File() {

}