#include "stdafx.h"
#include "Lock.h"
CLock::CLock()
{
	InitializeCriticalSection(&cs);//初始化临界区 
}


CLock::~CLock()
{
	DeleteCriticalSection(&cs);//删除临界区
}


void CLock::Lock()
{
	EnterCriticalSection(&cs);//进入临界区 
}


void CLock::UnLock()
{
	LeaveCriticalSection(&cs);//离开临界区 
}
