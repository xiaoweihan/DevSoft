#pragma once
#include <windows.h>  
#include <PROCESS.H> 
class CLock
{
public:
	CLock();
	~CLock();
	void Lock();
	void UnLock();
protected:
	CRITICAL_SECTION cs;
};

