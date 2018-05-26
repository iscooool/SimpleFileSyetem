/**
 * Project Untitled
 */


#ifndef _IOPARAMETER_H
#define _IOPARAMETER_H

class IOParameter {
public: 
    unsigned char* m_Base;
    int m_Offset;
    int m_Count;
    
public:
	IOParameter();
	~IOParameter();
};

#endif //_IOPARAMETER_H