#pragma once

//头文件
/**************************************************************************************************************************************************/
#ifdef _WIN32
#include <stdio.h>
#include <Windows.h>
#include <map>
#include <string>
#include <stdarg.h>
#include <time.h>
#include <io.h>
//包含Linux操作串口的头文件
#else
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <pthread.h>
#include <map>
#include <string>
#include <stdarg.h>
#include <string.h>
#endif

//宏定义
/************************************************************************************************************************************************/
#ifdef _WIN32
//锁的操作
typedef CRITICAL_SECTION    Mutex;
#define MUTEX_INIT(x)   InitializeCriticalSection(&x)
#define LOCK(x)             EnterCriticalSection(&x)
#define UNLOCK(x)         LeaveCriticalSection(&x)
#define MUTEX_DEL(x)    DeleteCriticalSection(&x)
#else
typedef int HANDLE;
#define INVALID_HANDLE_VALUE  (-1)
//#define NULL                                        (0)
//锁的操作
typedef pthread_mutex_t  Mutex;
#define MUTEX_INIT(x)   x = PTHREAD_MUTEX_INITIALIZER
#define LOCK(x)             pthread_mutex_lock(&x)
#define UNLOCK(x)         pthread_mutex_unlock(&x)
#define MUTEX_DEL(x)    pthread_mutex_destroy(&x)
#define MAX_PATH          (260)
#endif

//临界区
class CLogLock
{
public:
	CLogLock(void);
	~CLogLock(void);

	void Lock(void);
	void UnLock(void);
private:
	Mutex m_Lock;
};

