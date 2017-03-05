/****************************************************************************************
*版权所有:
*文件名:ComImple.h
*描述:串口通信接口类
*日期:2016.9
******************************************************************************************/

#ifndef COM_IMPL_H
#define COM_IMPL_H

#include <windows.h>
#include <deque>

using std::deque;

#define WM_COMM_BREAK_DETECTED		WM_USER+1	// 输入中发生中断
#define WM_COMM_CTS_DETECTED		WM_USER+2	// CTS信号状态发生变化
#define WM_COMM_DSR_DETECTED		WM_USER+3	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+4	// 发生线路状态错误 
#define WM_COMM_RING_DETECTED		WM_USER+5	// 检测到振铃指示
#define WM_COMM_RLSD_DETECTED		WM_USER+6	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		WM_USER+8	// 接收到事件字符，并置于输入缓冲区中	 
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+9	// The last character in the output buffer was sent.  

//备用
#define MaxSerialPortNum 20   ///有效的串口总个数，不是串口的号

#define  MAX_THREAD_WAIT_TIME  (0xFFFFFFFF)  //线程等待退出的最大等待时间

#define WRITE_BUFFER_SIZE			2048		//写缓冲区大小

#define COMIPLE		CComImple::CreateInstance()

////串口数据接收队列定义
//class CComDataQueue
//{
//public:
//	CComDataQueue(void);
//	~CComDataQueue(void);
//public:
//
//	//接收队列元素结构体定义
//	typedef struct _comData
//	{
//		//接收的数据
//		char* pData;
//		//接收数据的长度
//		int nDataLength;
//
//		// 构造函数
//		_comData()
//		{
//			pData = NULL;
//			nDataLength = 0;
//		}
//
//		//拷贝构造
//		_comData(const _comData& copyData)
//		{
//			nDataLength = copyData.nDataLength;
//			pData = new char[nDataLength];
//			memcpy(pData,copyData.pData,nDataLength);
//		}
//
//		//赋值函数
//		_comData& operator= (const _comData& copyData)
//		{
//			//防止自赋值
//			if (this == &copyData)
//			{
//				return *this;
//			}
//
//			if (NULL != pData)
//			{
//				delete []pData;
//				pData = NULL;
//			}
//
//			nDataLength = copyData.nDataLength;
//			pData = new char[nDataLength];
//			memcpy(pData,copyData.pData,nDataLength);
//		}
//
//		//析构函数
//		~_comData()
//		{
//			if (NULL != pData)
//			{
//				delete []pData;
//				pData = NULL;
//			}
//			nDataLength = 0;
//		}
//
//	}COMDATA,*LPCOMDATA;
//
//public:
//
//	//判断队列是否为空
//	bool IsQueueEmpty(void);
//	//获取队列中元素个数
//	int GetQueueElementNum(void);
//	//放入元素到队列
//	void PushElement(const char* pBuffer,int nLength );
//	//从队列中弹出元素
//	void PopElement(void);
//	//获取队列的头的元素
//	LPCOMDATA GetHeadElement(void);
//	//释放信号量
//	void ReleaseLock(void);
//private:
//	//释放内存的接口
//	void FreeQueueMemory(void);
//	//接收队列
//	deque<LPCOMDATA> m_Queue;
//	//操作队列的临界区
//	CRITICAL_SECTION m_cs;
//	//信号量句柄
//	HANDLE m_SemphoreHandle;
//};


//串口通信
class CSerialPort
{
public:
	CSerialPort(void);
	virtual ~CSerialPort(void);

	//串口初始化
	BOOL		InitPort(CWnd* pPortOwner, //串口句柄
		UINT portnr = 1, //端口号
		UINT baud = 9600, //波特率
		char parity = 'N', //奇偶校验
		UINT databits = 8, //数据位
		UINT stopbits = 1, //停止位
		DWORD dwCommEvents = EV_RXCHAR, //消息类型
		UINT writebuffersize = WRITE_BUFFER_SIZE//写缓存
		);

	//控制串口监视线程
	BOOL		 StartMonitoring();//开始监听
	BOOL		 RestartMonitoring();//重新监听
	BOOL		 StopMonitoring();//停止监听

	//获取写缓冲大小
	DWORD		 GetWriteBufferSize();
	//获取事件
	DWORD		 GetCommEvents();
	//获取DCB
	DCB			 GetDCB();
	//获取超时
	COMMTIMEOUTS GetCommTimeOuts();
	//设置超时
	BOOL         SetCommTimeOuts(COMMTIMEOUTS * lpTimeOuts);

	//写数据到串口
	void		WriteToPort(char* string);
	void		WriteToPort(char* string,int n);
	void		WriteToPort(LPCTSTR string,int n);
	void        WriteToPort(BYTE* Buffer, int n);

	//关闭串口
	void		ClosePort();


	//串口发送函数（备用）
	BOOL        RecvData(LPTSTR lpszData, const int nSize);
	//串口接收函数（备用）
	void        SendData(LPCTSTR lpszData, const int nLength);

protected:
	//保护的成员函数

	//错误信息输出
	void		ProcessErrorMessage(char* ErrorText);

	//线程函数
	static UINT	CommThread(LPVOID pParam);
	//接收字符
	static void	ReceiveChar(CSerialPort* pCom, COMSTAT comstat);
	//写字符
	static void	WriteChar(CSerialPort* pCom);


	//线程
	//监视线程指针
	CWinThread*			m_Thread;

	//监视线程是否挂起
	BOOL                m_bIsSuspened;
	//临界资源
	CRITICAL_SECTION	m_csCommunicationSync;
	//监视线程运行标志
	BOOL				m_bThreadAlive;

	//写事件句柄
	HANDLE				m_hWriteEvent;
	//串口句柄
	HANDLE				m_hComm;
	//关闭事件句柄
	HANDLE				m_hShutdownEvent;

	//事件数组，包括一个写事件，接收事件，关闭事件
	//一个元素用于一个事件。有两个事件线程处理端口。
	//写事件和接收字符事件位于overlapped结构体（m_ov.hEvent）中
	//当端口关闭时，有一个通用的关闭。
	HANDLE				m_hEventArray[3];

	//结构体

	//异步I/O
	OVERLAPPED			m_ov;
	//超时设置
	COMMTIMEOUTS		m_CommTimeouts;
	//设备控制块
	DCB					m_dcb;

	//父窗口指针
	CWnd*				m_pOwner;

	//缓冲区
	UINT				m_nPortNr;
	//写缓冲区
	char*				m_szWriteBuffer;
	DWORD				m_dwCommEvents;
	//写缓冲区大小
	DWORD				m_nWriteBufferSize;
	//写入字节数
	int m_nWriteSize;

	//接收缓冲区
	char m_recvBuffer[WRITE_BUFFER_SIZE];
	//处理缓冲区
	char m_procBuffer[WRITE_BUFFER_SIZE];
	//写指针
	int m_nWritePos;
	//读指针
	int m_nReadPos;

private:
	//解析串口数据线程句柄
	HANDLE m_HandleDecode;

	//解析串口数据线程
	static DWORD WINAPI DecodeComDataProc(LPVOID lpParam);

	//线程的循环标识
	static bool s_bloop;
};


class CComImple
{
public:
	static CComImple& CreateInstance();

private:
	CComImple(void);
	~CComImple(void);

public:
	
	//开启串口通信
	void StartCom(CWnd* pWnd);
	//关闭串口通信
	void StopCom(void);
	//向串口发送数据
	void SendComData(BYTE* Buffer, int nLength);

private:
	//串口通信实例
	CSerialPort m_SerialPort;
};

#endif