/*************************************************************************************************
*版权所有:
*文件名:ComImple.cpp
*描述:串口通信接口类实现
*日期:2016.9
****************************************************************************************************/

#include "stdafx.h"
#include "ComImple.h"
#include "Utility.h"
#include <process.h>
#include "GlobalDataManager.h"
#pragma warning(push)
#pragma warning(disable:4018)
/*************************************************************************************************
函数名称:CreateInstance
功能描述:串口通信接口类实例化
输入参数:无
输出参数:无
返回值:串口通信实例
***************************************************************************************************/
CComImple& CComImple::CreateInstance()
{
	static CComImple s_ComImple;
	return s_ComImple;
}

/*************************************************************************************************
函数名称:CComImple
功能描述:串口通信接口类构造函数
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
CComImple::CComImple(void)
{
	
}

/*************************************************************************************************
函数名称:~CComImple
功能描述:串口通信接口类析构函数
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
CComImple::~CComImple(void)
{
	
}

/*************************************************************************************************
函数名称:StartCom
功能描述:开启串口通信
输入参数:hWnd:窗口句柄，向窗口反馈消息
输出参数:无
返回值:
***************************************************************************************************/
void CComImple::StartCom(CWnd* pWnd)
{
	//判断窗口句柄是否有效
	//if (NULL == pWnd)
	{
	//	return;
	}

	//初始化串口通信
	if(!m_SerialPort.InitPort(pWnd,4))
		return;

	//开启控制串口监视线程
	m_SerialPort.StartMonitoring();
}

/*************************************************************************************************
函数名称:StopCom
功能描述:关闭串口通信
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
void CComImple::StopCom()
{
	//停止监听
	m_SerialPort.StopMonitoring();

	//停止处理线程
	//s_bloop = false;

	//接收队列释放锁
	//m_SerialPort.m_RecvQueue.ReleaseLock();

	//等待解析处理线程结束
	/*if (NULL != m_HandleDecode)
	{
		WaitForSingleObject(m_HandleDecode,MAX_THREAD_WAIT_TIME);
		CloseHandle(m_HandleDecode);
		m_HandleDecode = NULL;
	}*/

	//关闭串口
	m_SerialPort.ClosePort();
}

/*************************************************************************************************
函数名称:SendComData
功能描述:向串口发送数据
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
void CComImple::SendComData(BYTE* Buffer, int nLength)
{
	//判断参数有效性
	if (NULL == Buffer||nLength <= 0)
	{
		return;
	}

	//发送数据
	m_SerialPort.WriteToPort(Buffer,nLength);
}

/*******************************************************************************************************************************
//串口通信类实现
/*******************************************************************************************************************************

/*************************************************************************************************
函数名称:CSerialPort
功能描述:串口通信类构造函数
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
CSerialPort::CSerialPort(void)
{
	//初始化串口句柄
	m_hComm = NULL;

	//初始化异步结构体
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;

	//创建事件
	m_ov.hEvent = NULL;
	m_hWriteEvent = NULL;
	m_hShutdownEvent = NULL;

	m_szWriteBuffer = NULL;

	m_bThreadAlive = FALSE;

	m_nWriteSize=1;
	m_bIsSuspened = FALSE;

	m_nWritePos = 0;
	m_nReadPos = 0;

	//解析线程句柄
	m_HandleDecode = NULL;

	//创建串口数据处理线程
	DWORD dwThreadID = 0;

	s_bloop = true;

	m_HandleDecode = CreateThread(NULL,0,DecodeComDataProc,this,0,&dwThreadID);

	if (NULL == m_HandleDecode)
	{
		return;
	}
}

/*************************************************************************************************
函数名称:~CSerialPort
功能描述:串口通信类析构函数
输入参数:无
输出参数:无
返回值:
***************************************************************************************************/
CSerialPort::~CSerialPort(void)
{
	do
	{
		SetEvent(m_hShutdownEvent);
	} while (m_bThreadAlive);


	// 端口如果打开的，关闭之。
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	//关闭读、写、关闭 
	if(m_hShutdownEvent!=NULL)
		CloseHandle( m_hShutdownEvent); 
	if(m_ov.hEvent!=NULL)
		CloseHandle( m_ov.hEvent ); 
	if(m_hWriteEvent!=NULL)
		CloseHandle( m_hWriteEvent ); 

	//结束线程

	delete [] m_szWriteBuffer;
}

/*************************************************************************************************
函数名称:InitPort
功能描述:初始化串口
输入参数:见函数体
输出参数:无
返回值: TRUE/成功 FALSE/失败
***************************************************************************************************/
BOOL CSerialPort::InitPort(CWnd* pPortOwner,	//父窗口串口句柄（用于接收消息用）
						   UINT  portnr,		// 端口号
						   UINT  baud,			// 波特率
						   char  parity,		// 奇偶校验 
						   UINT  databits,		// 数据位 
						   UINT  stopbits,		// 停止位 
						   DWORD dwCommEvents,	// 事件 EV_RXCHAR, EV_CTS 等
						   UINT  writebuffersize)	// 写缓存大小
{
	//判断端口 （最多 支持20个）
	if(portnr < 0 || portnr > MaxSerialPortNum + 1)
	{
		return FALSE;
	}
	
	//if(pPortOwner == NULL)
	{
		//return FALSE;
	}

	//如果线程存在，则关掉进程
	if (m_bThreadAlive)
	{
		do
		{
			SetEvent(m_hShutdownEvent);
		} while (m_bThreadAlive);
	}

	//创建事件
	//异步I/O
	if (m_ov.hEvent != NULL)
	{
		ResetEvent(m_ov.hEvent);
	}
	else
	{
		m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	//写事件
	if (m_hWriteEvent != NULL)
	{
		ResetEvent(m_hWriteEvent);
	}
	else
	{
		m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	
	//关闭事件
	if (m_hShutdownEvent != NULL)
	{
		ResetEvent(m_hShutdownEvent);
	}
	else
	{
		m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	//事件数组初始化，设定优先级别
	m_hEventArray[0] = m_hShutdownEvent;	//最高位校验
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	//初始化临界资源
	InitializeCriticalSection(&m_csCommunicationSync);
	
	//父窗口
	m_pOwner = pPortOwner;

	if (NULL != m_szWriteBuffer)
	{
		delete [] m_szWriteBuffer;
	}
	
	m_szWriteBuffer = new char[writebuffersize];

	m_nPortNr = portnr;

	m_nWriteBufferSize = writebuffersize;
	m_dwCommEvents = dwCommEvents;

	BOOL bResult = FALSE;
	TCHAR *szPort = new TCHAR[50];
	TCHAR *szBaud = new TCHAR[50];

	//进入临界区
	EnterCriticalSection(&m_csCommunicationSync);

	//串口已打开就关掉
	if (NULL != m_hComm)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	//串口参数
	wsprintf(szPort, _T("COM%d"), portnr);
	wsprintf(szBaud, _T("baud=%d parity=%c data=%d stop=%d"), baud, parity, databits, stopbits);

	//打开串口
	m_hComm = CreateFile((LPCTSTR)szPort,				// 串口名称
					     GENERIC_READ | GENERIC_WRITE,	// 读写方式
					     0,								// 
					     NULL,							// 
					     OPEN_EXISTING,					// 串口必须使用 OPEN_EXISTING
					     FILE_FLAG_OVERLAPPED,			// 异步 I/O
					     0);							// 必须为0

	//如果创建失败
	if (INVALID_HANDLE_VALUE == m_hComm)
	{
		//释放资源
		delete [] szPort;
		delete [] szBaud;

		return FALSE;
	}

	//设置超时
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

	//配置
	//分别调用Windows API设置串口参数
	//设置超时
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))
	{
		
		// 若对端口数据的响应时间要求较严格，可采用事件驱动方式。
		// 本软件使用115200波特率，已经是USB转串口的极限，理论上会存在晶振不同。造成数据错位、覆盖、丢包等问题。
		// 待完善.......
		// Windows定义了9种串口通信事件，较常用的有以下三种：
		//		EV_RXCHAR:接收到一个字节，并放入输入缓冲区；
		//		EV_TXEMPTY:输出缓冲区中的最后一个字符，发送出去；
		//		EV_RXFLAG:接收到事件字符(DCB结构中EvtChar成员)，放入输入缓冲区
		// 在用SetCommMask()指定了有用的事件后，应用程序可调用WaitCommEvent()来等待事件的发生。
		// SetCommMask(hComm,0)可使WaitCommEvent()中止
		
		//设置通信事件
		if (SetCommMask(m_hComm, dwCommEvents))
		{
			//获取当前DCB参数
			if (GetCommState(m_hComm, &m_dcb))
			{
				//设置字件字符
				m_dcb.EvtChar = 'q';
				//设置RTS高位
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;

				//填写DCB结构
				if (BuildCommDCB((LPCWSTR)szBaud, &m_dcb))
				{
					//配置DCB
					if (!SetCommState(m_hComm, &m_dcb))
					{
						ProcessErrorMessage("SetCommState()");
					}
				}
				else
				{
					ProcessErrorMessage("BuildCommDCB()");
				}
			}
			else
			{
				ProcessErrorMessage("GetCommState()");
			}
		}
		else
		{
			ProcessErrorMessage("SetCommMask()");
		}
	}
	else
	{
		ProcessErrorMessage("SetCommTimeouts()");
	}

	//释放资源
	delete [] szPort;
	delete [] szBaud;

	//终止读写并清空接收和发送
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	//释放临界资源
	LeaveCriticalSection(&m_csCommunicationSync);

	return TRUE;
}

/*************************************************************************************************
函数名称:CommThread
功能描述:监视线程
输入参数:pParam : com通信接口类指针
输出参数:无
返回值: 0（可扩展）
备注: 事件驱动核心：检查串口-->进入循环{WaitCommEvent(不阻塞询问)询问事件-->如果有事件来到-->到相应处理(关闭\读\写)}
***************************************************************************************************/
UINT CSerialPort::CommThread(LPVOID pParam)
{
	//com通信接口类指针
	CSerialPort *pSerialPort = (CSerialPort*)pParam;

	if(NULL == pSerialPort)
	{
		//结束线程
		return 0;
	}
	
	//TRUE表示线程正在运行
	pSerialPort->m_bThreadAlive = TRUE;	
		
	DWORD BytesTransfered = 0; 
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL  bResult = TRUE;
		
	//开始时清除串口缓冲
	//检查串口是否打开
	if (pSerialPort->m_hComm)
	{
		PurgeComm(pSerialPort->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);
	}

	//只要线程存在就不断读取数据
	while (TRUE) 
	{ 
		//表示该函数是异步的
		bResult = WaitCommEvent(pSerialPort->m_hComm, &Event, &pSerialPort->m_ov);
		
		if (!bResult)  
		{ 
			//如果WaitCommEvent返回Error为FALSE，则查询错误信息
			switch (dwError = GetLastError()) 
			{ 
			case ERROR_IO_PENDING: 	//正常情况，没有字符可读
				{ 
					break;
				}
			case ERROR_INVALID_PARAMETER://系统错误
				{
					break;
				}
			default://发生其他错误，包括：串口读写中断开串口连接的错误
				{
					pSerialPort->ProcessErrorMessage("WaitCommEvent()");
					break;
				}
			}
		}
		else
		{
			bResult = ClearCommError(pSerialPort->m_hComm, &dwError, &comstat);

			if (comstat.cbInQue == 0)
				continue;
		}

		//主等待函数，会阻塞线程
		//等待3个事件：关断/读/写
		Event = WaitForMultipleObjects(3,	//3个事件
			pSerialPort->m_hEventArray,			//事件数组
			FALSE,							//有一个事件发生就返回
			INFINITE);						//超时时间

		switch (Event)
		{
		case 0:	//关断事件，关闭串口
			{
				CloseHandle(pSerialPort->m_hComm);
				pSerialPort->m_hComm=NULL;
				pSerialPort->m_bThreadAlive = FALSE;
				
				AfxEndThread(100);
				break;
			}
		case 1:	//读事件 将定义的各种消息发送出去
			{
				memset(&comstat, 0, sizeof(COMSTAT)); 
				GetCommMask(pSerialPort->m_hComm, &CommEvent);

				//接收到字符，并置于输入缓冲区中
				if (CommEvent & EV_RXCHAR)
				{
					ReceiveChar(pSerialPort, comstat);
				}

				//CTS信号状态发生变化
				if (CommEvent & EV_CTS)
				{
					//打印日志
				}

				//输入中发生中断
				if (CommEvent & EV_BREAK)
				{
					//打印日志
				}

				//发生线路状态错误，线路状态错误包括CE_FRAME,CE_OVERRUN和CE_RXPARITY 
				if (CommEvent & EV_ERR)
				{
					//打印日志
				}

				//检测到振铃指示	
				if (CommEvent & EV_RING)
				{
					//打印日志
				}

				//接收到事件字符，并置于输入缓冲区中	
				if (CommEvent & EV_RXFLAG)
				{
					//打印日志
				}
				break;
			}  
		case 2: //写事件
			{
				WriteChar(pSerialPort);
				break;
			}
		default:
			{
				//接收错误
				//打印日志
				break;
			}

		}

	}

	return 0;
}

/*************************************************************************************************
函数名称:StartMonitoring
功能描述:开启监视线程
输入参数:无
输出参数:无
返回值: TRUE/成功 FALSE/失败
***************************************************************************************************/
BOOL CSerialPort::StartMonitoring()
{
	//开启监视线程
	if (!(m_Thread = AfxBeginThread(CommThread, this)))
	{
		return FALSE;
	}

	m_bIsSuspened = false;

	return TRUE;	
}

/*************************************************************************************************
函数名称:RestartMonitoring
功能描述:复位监视线程
输入参数:无
输出参数:无
返回值: TRUE/成功 FALSE/失败
***************************************************************************************************/
BOOL CSerialPort::RestartMonitoring()
{
	m_bIsSuspened = false;

	m_Thread->ResumeThread();

	return TRUE;	
}

/*************************************************************************************************
函数名称:StopMonitoring
功能描述:挂起监视线程
输入参数:无
输出参数:无
返回值: TRUE/成功 FALSE/失败
***************************************************************************************************/
BOOL CSerialPort::StopMonitoring()
{
	m_bIsSuspened = true;

	m_Thread->SuspendThread();

	return TRUE;
}

/*************************************************************************************************
函数名称:ProcessErrorMessage
功能描述:错误信息提示
输入参数:ErrorText 错误信息
输出参数:无
返回值: 无
***************************************************************************************************/
void CSerialPort::ProcessErrorMessage(char* ErrorText)
{
	if(NULL == ErrorText)
	{
		return;
	}

	char *Temp = new char[256];
	
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	sprintf(Temp, "警告:  %s 失败可能是以下问题: \n%s\nPort: %d\n", (char*)ErrorText, lpMsgBuf, m_nPortNr);

	//打印日志 or 弹框
	MessageBox(NULL, (LPCWSTR)Temp, _T("Application Error"), MB_ICONSTOP);

	LocalFree(lpMsgBuf);
	delete [] Temp;

	return;
}

/*************************************************************************************************
函数名称:WriteChar
功能描述:写数据
输入参数:pCom com通信接口类指针
输出参数:无
返回值: 无
***************************************************************************************************/
void CSerialPort::WriteChar(CSerialPort* pCom)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;

	DWORD BytesSent = 0;

	//复位写事件句柄
	ResetEvent(pCom->m_hWriteEvent);

	//进入临界区
	EnterCriticalSection(&pCom->m_csCommunicationSync);

	if (bWrite)
	{
		//初始化参数
		pCom->m_ov.Offset = 0;
		pCom->m_ov.OffsetHigh = 0;

		//清空缓存
		PurgeComm(pCom->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		//串口写入
		bResult = WriteFile(pCom->m_hComm,							// 串口句柄
							pCom->m_szWriteBuffer,					// 消息缓存
							pCom->m_nWriteSize,						// 发现消息长度
							&BytesSent,								// 发送字节数
							&pCom->m_ov);

		if (!bResult)  //如果发生错误
		{
			DWORD dwError = GetLastError();
			switch (dwError)
			{
				case ERROR_IO_PENDING:
					{
						BytesSent = 0;
						bWrite = FALSE;
						break;
					}
				default:
					{
						//打印日志 or 错误消息
						pCom->ProcessErrorMessage("WriteFile()");
						break;
					}
			}
		} 
		else
		{
			LeaveCriticalSection(&pCom->m_csCommunicationSync);
		}
	}

	if (!bWrite)
	{
		bWrite = TRUE;
	
		bResult = GetOverlappedResult(pCom->m_hComm,	// 串口句柄
									  &pCom->m_ov,		// Overlapped 结构
									  &BytesSent,		// 记录字节数
									  TRUE); 			// 等待标志

		LeaveCriticalSection(&pCom->m_csCommunicationSync);

		if (!bResult)
		{
			//打印日志 or 错误消息
			pCom->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}	
	}

	if (BytesSent != pCom->m_nWriteSize)
	{
		//实际发送长度不一致
		//打印日志
	}
}

/*************************************************************************************************
函数名称:ReceiveChar
功能描述:读数据
输入参数:pCom com通信接口类指针 comstat com结构
输出参数:无
返回值: 无
***************************************************************************************************/
static int nTime = 0;
void CSerialPort::ReceiveChar(CSerialPort* pCom, COMSTAT comstat)
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	unsigned char RXBuff[WRITE_BUFFER_SIZE];

	while (true) 
	{ 
		//防止死锁
		if(WaitForSingleObject(pCom->m_hShutdownEvent,0) == WAIT_OBJECT_0)
		{
            return;
		} 
		
		EnterCriticalSection(&pCom->m_csCommunicationSync);

		//更新COMSTAT
		bResult = ClearCommError(pCom->m_hComm, &dwError, &comstat);

		LeaveCriticalSection(&pCom->m_csCommunicationSync);
		
		//所有字符均被读出，中断循环
		if (comstat.cbInQue == 0)
		{
			break;
		}
						
		EnterCriticalSection(&pCom->m_csCommunicationSync);

		if (bRead)
		{
			//串口读出，读出缓冲区中字节
			bResult = ReadFile(pCom->m_hComm,		// 串口句柄 
							   &RXBuff,				// 读取缓冲区
							   WRITE_BUFFER_SIZE,	// 读一个字节
							   &BytesRead,			// 读取字节数
							   &pCom->m_ov);

			///若返回错误，错误处理
			if (!bResult)  
			{ 
				switch (dwError = GetLastError()) 
				{ 
					case ERROR_IO_PENDING: 	
						{ 
							//异步IO仍在进行
							bRead = FALSE;
							break;
						}
					default:
						{
							//输出错误 or 打印日志
							pCom->ProcessErrorMessage("ReadFile()");
							break;
						} 
				}
			}
			else
			{
				bRead = TRUE;
			}
		}

		//异步IO操作仍在进行，需要调用GetOverlappedResult查询
		if (!bRead)
		{
			bRead = TRUE;
			bResult = GetOverlappedResult(pCom->m_hComm,
										  &pCom->m_ov,
										  &BytesRead,
										  TRUE);

			if (!bResult)  
			{
				//输出错误信息 or 打印日志
				pCom->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
			}	
		}
				
		LeaveCriticalSection(&pCom->m_csCommunicationSync);

		//接收到串口信息 缓存到RXBuff
		//发送到父窗口处理？
		//::SendMessage((pCom->m_pOwner)->m_hWnd, WM_COMM_RXCHAR, (WPARAM) RXBuff, (LPARAM) port->m_nPortNr);
		//添加到缓冲队列 -> 解析 ->处理？
		//m_RecvQueue.

		/*BYTE sendbuf[10];
		sendbuf[0] = 0x11;
		pCom->WriteToPort(sendbuf,1);*/

		//memcpy(pCom->m_recvBuffer,RXBuff,BytesRead);

		//pCom->m_nWritePos += BytesRead >= WRITE_BUFFER_SIZE? pCom->m_nWritePos - WRITE_BUFFER_SIZE : pCom->m_nWritePos;

		if(BytesRead==6)
		{
			if(RXBuff[0] == 0xAA)
			{
				if(RXBuff[1] == 0x04)//数据
				{
					short nData = 0;
					memcpy(&nData,RXBuff+3,sizeof(short));
					BYTE nSensorChan = 0;
					memcpy(&nSensorChan,RXBuff+2,sizeof(BYTE));

					GLOBAL_DATA.PushData(nSensorChan,nData);
					CString strTemp;
					strTemp.Format(_T("%d"),nTime);
					GLOBAL_DATA.ModifyData(GLOBAL_DATA.FindTimeID(),nTime++,strTemp);
				}
				else if(RXBuff[1] == 0x01)//识别
				{

					nTime = 0;

					short nSensorID = 0;
					memcpy(&nSensorID,RXBuff+3,sizeof(short));
					BYTE nSensorChan = 0;
					memcpy(&nSensorChan,RXBuff+2,sizeof(BYTE));

					//添加数据列
					GLOBAL_DATA.AppendTimeColumn();
					GLOBAL_DATA.AppendColumn(nSensorID,nSensorChan);

					BYTE bHz[6] = {0xAA,0x02,0x01,0x00,0xAA,0xFF};

					pCom->WriteToPort(bHz,6);

					GLOBAL_DATA.SetChanged();
					
					GLOBAL_DATA.Notify((void*)1);
				}
			}
		}
	}

}

/*************************************************************************************************
函数名称:WriteToPort
功能描述:向串口写入字符串
输入参数:string ： 字符串
输出参数:无
返回值: 无
***************************************************************************************************/
void CSerialPort::WriteToPort(char* string)
{
	if(NULL == m_hComm)
	{
		return;
	}
	
	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	strcpy(m_szWriteBuffer, string);
	m_nWriteSize=strlen(string);

	// 设置写事件 写入字符串
	SetEvent(m_hWriteEvent);
}

/*************************************************************************************************
函数名称:WriteToPort
功能描述:向串口写入字符串
输入参数:string ： 字符串 n：字符串长度
输出参数:无
返回值: 无
***************************************************************************************************/
void CSerialPort::WriteToPort(char* string,int n)
{		
	if(NULL == m_hComm)
	{
		return;
	}

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	memcpy(m_szWriteBuffer, string, n);
	m_nWriteSize=n;

	// 设置写事件 写入字符串
	SetEvent(m_hWriteEvent);
}

/*************************************************************************************************
函数名称:WriteToPort
功能描述:向串口写入字符串
输入参数:string ： 字符串 n：字符串长度
输出参数:无
返回值: 无
***************************************************************************************************/
void CSerialPort::WriteToPort(LPCTSTR string,int n)
{		
	if(NULL == m_hComm)
	{
		return;
	}

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	memcpy(m_szWriteBuffer, string, n);
	m_nWriteSize = n;

	// 设置写事件 写入字符串
	SetEvent(m_hWriteEvent);
}

/*************************************************************************************************
函数名称:WriteToPort
功能描述:向串口写入字符串
输入参数:Buffer ： 字符数组 n：字符串长度
输出参数:无
返回值: 无
***************************************************************************************************/
void CSerialPort::WriteToPort(BYTE* Buffer, int n)
{
	if(NULL == m_hComm)
	{
		return;
	}

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	int i;
	for(i=0; i<n; i++)
	{
		m_szWriteBuffer[i] = Buffer[i];
	}
	m_nWriteSize=n;
	
	// 设置写事件 写入字符串
	SetEvent(m_hWriteEvent);
}

/*************************************************************************************************
函数名称:GetDCB
功能描述:取得DCB信息
输入参数:无
输出参数:无
返回值: DCB结构信息
***************************************************************************************************/
DCB CSerialPort::GetDCB()
{
	return m_dcb;
}

/*************************************************************************************************
函数名称:GetCommEvents
功能描述:获取事件
输入参数:无
输出参数:无
返回值: 事件定义
***************************************************************************************************/
DWORD CSerialPort::GetCommEvents()
{
	return m_dwCommEvents;
}

/*************************************************************************************************
函数名称:GetWriteBufferSize
功能描述:获取写缓冲区大小
输入参数:无
输出参数:无
返回值: 缓冲区大小
***************************************************************************************************/
DWORD CSerialPort::GetWriteBufferSize()
{
	return m_nWriteBufferSize;
}

/*************************************************************************************************
函数名称:ClosePort
功能描述:关闭串口
输入参数:无
输出参数:无
返回值: 无
***************************************************************************************************/
void CSerialPort::ClosePort()
{
    if(m_bIsSuspened)
	{
		RestartMonitoring();
	}

    if (m_bThreadAlive)
    {
        MSG message;
        while (m_bThreadAlive)
        {
			//防止死锁
            if(::PeekMessage(&message, m_pOwner->m_hWnd, 0, 0, PM_REMOVE))
            {
                ::TranslateMessage(&message);
                ::DispatchMessage(&message);
            }

            SetEvent(m_hShutdownEvent);
        }
    }

    if(NULL != m_szWriteBuffer)
    {
        delete [] m_szWriteBuffer;
        m_szWriteBuffer = NULL;
    }

    if(NULL != m_hComm)
    {
        CloseHandle(m_hComm);
        m_hComm = NULL;
    }

	//关闭事件
	if(NULL != m_hShutdownEvent)
	{
		ResetEvent(m_hShutdownEvent);
	}

	if(NULL != m_ov.hEvent)
	{
		ResetEvent(m_ov.hEvent);
	}

	if(NULL != m_hWriteEvent)
	{
		ResetEvent(m_hWriteEvent);
	}
}

/*************************************************************************************************
函数名称:SendData
功能描述:发送数据
输入参数:无
输出参数:无
返回值: 无
***************************************************************************************************/
void CSerialPort::SendData(LPCTSTR lpszData, const int nLength)
{
	if(NULL == m_hComm)
	{
		return;
	}

	//如果大于写入缓冲区大小
	if(nLength > WRITE_BUFFER_SIZE)
	{
		return;
	}

	memset(m_szWriteBuffer, 0, nLength);
	memcpy(m_szWriteBuffer, lpszData, nLength);
	m_nWriteSize=nLength;

	// 设置写事件 写入字符串
	SetEvent(m_hWriteEvent);
}

/*************************************************************************************************
函数名称:RecvData
功能描述:接收指定长度数据
输入参数:无
输出参数:无
返回值: 无
***************************************************************************************************/
BOOL CSerialPort::RecvData(LPTSTR lpszData, const int nSize)
{
	if(NULL == m_hComm)
	{
		return FALSE;
	}

    memset(lpszData,0,nSize);

    DWORD mylen  = 0;
    DWORD mylen2 = 0;
    while (mylen < nSize) 
	{
        if(!ReadFile(m_hComm,lpszData,nSize,&mylen2,NULL)) 
            return FALSE;
        mylen += mylen2;   
    }
    
    return TRUE;
}

/*************************************************************************************************
函数名称:GetCommTimeOuts
功能描述:获得超时时间结构
输入参数:无
输出参数:无
返回值: 超时时间结构信息
***************************************************************************************************/
COMMTIMEOUTS CSerialPort:: GetCommTimeOuts()
{
	return m_CommTimeouts;
}

/*************************************************************************************************
函数名称:SetCommTimeOuts
功能描述:设置超时
输入参数:无
输出参数:无
返回值: TRUE
***************************************************************************************************/
BOOL CSerialPort::SetCommTimeOuts(COMMTIMEOUTS *lpTimeOuts)
{
	//设置超时 
    SetCommTimeouts(m_hComm, lpTimeOuts);

	return TRUE;
}


///*******************************************************************************************************************************
////串口数据接收队列类实现
///*******************************************************************************************************************************
//
///*******************************************************************************************************************************
//函数名称:CComDataQueue
//功能描述:发送队列类的构造函数
//输入参数:无
//输出参数:无
//返回值:无
//*******************************************************************************************************************************/
//
//CComDataQueue::CComDataQueue( void ):
//m_SemphoreHandle(NULL)
//{
//	//初始化临界区
//	InitializeCriticalSection(&m_cs);
//
//	//清空队列
//	m_Queue.clear();
//
//	//创建信号量
//	m_SemphoreHandle = CreateSemaphore(NULL,0,1,NULL);
//}
//
///*******************************************************************************************************************************
//函数名称:~CComDataQueue
//功能描述:发送队列类的构造函数
//输入参数:无
//输出参数:无
//返回值:无
//*******************************************************************************************************************************/
//CComDataQueue::~CComDataQueue( void )
//{
//	//清空内存
//	FreeQueueMemory();
//	//删除临界区
//	DeleteCriticalSection(&m_cs);
//	CloseHandle(m_SemphoreHandle);
//	m_SemphoreHandle = NULL;
//}
//
///*******************************************************************************************************************************
//函数名称:IsQueueEmpty
//功能描述:判断发送队列是否为空
//输入参数:无
//输出参数:无
//返回值:true:队列为空  false:队列不为空
//*******************************************************************************************************************************/
//bool CComDataQueue::IsQueueEmpty( void )
//{
//	bool bEmpty = false;
//	EnterCriticalSection(&m_cs);
//
//	//判断队列是否为空
//	bEmpty = m_Queue.empty();
//
//	LeaveCriticalSection(&m_cs);
//
//	return bEmpty;
//}
//
///*******************************************************************************************************************************
//函数名称:GetQueueElementNum
//功能描述:获取队列中元素的个数
//输入参数:无
//输出参数:无
//返回值:返回队列中元素的个数
//*******************************************************************************************************************************/
//int CComDataQueue::GetQueueElementNum( void )
//{
//	int nSize = 0;
//
//	EnterCriticalSection(&m_cs);
//
//	//获取队列的长度
//	nSize = (int)m_Queue.size();
//
//	LeaveCriticalSection(&m_cs);
//
//	return nSize;
//}
//
///*******************************************************************************************************************************
//函数名称:PushElement
//功能描述:向队列中放入元素
//输入参数:pBuffer:数据的内容 nLength:pBuffer的长度
//输出参数:无
//返回值:无
//*******************************************************************************************************************************/
//void CComDataQueue::PushElement( const char* pBuffer,int nLength )
//{
//	if (NULL == pBuffer )
//	{
//		return;
//	}
//
//	//申请内存
//	LPCOMDATA pContentData = new COMDATA;
//
//	//申请内存失败
//	if (NULL == pContentData)
//	{
//		return;
//	}
//
//	//记录发送内容的长度
//	pContentData->nDataLength = nLength;
//
//	//申请内存
//	pContentData->pData = new char[pContentData->nDataLength];
//
//	//申请内存失败
//	if (NULL == pContentData->pData)
//	{
//		//释放内存
//		DELETE_POINTER(pContentData);
//		return;
//	}
//
//	//拷贝内存
//	memcpy(pContentData->pData,pBuffer,nLength);
//
//	EnterCriticalSection(&m_cs);
//
//	//插入队列尾部
//	m_Queue.push_back(pContentData);
//
//	LeaveCriticalSection(&m_cs);
//
//	ReleaseSemaphore(m_SemphoreHandle,1,NULL);
//
//}
//
///*******************************************************************************************************************************
//函数名称:PopElement
//功能描述:向队列中删除一个元素
//输入参数:无
//输出参数:无
//返回值:无
//*******************************************************************************************************************************/
//void CComDataQueue::PopElement( void )
//{
//	//判断是否为空
//	if (IsQueueEmpty())
//	{
//		return;
//	}
//
//	EnterCriticalSection(&m_cs);
//
//	LPCOMDATA p = m_Queue.front();
//	m_Queue.pop_front();
//
//	LeaveCriticalSection(&m_cs);
//
//	DELETE_POINTER(p);
//
//}
//
///*******************************************************************************************************************************
//函数名称:GetHeadElement
//功能描述:返回队列中的头元素指针
//输入参数:无
//输出参数:无
//返回值:LPCOMDATA:指向队列中的头元素
//*******************************************************************************************************************************/
//CComDataQueue::LPCOMDATA CComDataQueue::GetHeadElement( void )
//{
//
//	WaitForSingleObject(m_SemphoreHandle,INFINITE);
//
//	LPCOMDATA pHeadElement = NULL;
//
//	//判断队列是否为空
//	if (IsQueueEmpty())
//	{
//		return pHeadElement;
//	}
//
//	EnterCriticalSection(&m_cs);
//
//	//获取队列头元素
//	pHeadElement = m_Queue.front();
//
//	LeaveCriticalSection(&m_cs);
//
//	return pHeadElement;
//}
//
///*******************************************************************************************************************************
//函数名称:FreeQueueMemory
//功能描述:释放队列中内存
//输入参数:无
//输出参数:无
//返回值:无
//*******************************************************************************************************************************/
//void CComDataQueue::FreeQueueMemory( void )
//{
//	
//	//判断队列是否为空
//	if (IsQueueEmpty())
//	{
//		return;
//	}
//
//	
//	EnterCriticalSection(&m_cs);
//
//	deque<LPCOMDATA>::iterator Iter = m_Queue.begin();
//
//	while (Iter != m_Queue.end())
//	{
//		//释放内存
//		DELETE_POINTER(*Iter);
//		++Iter;
//	}
//
//	m_Queue.clear();
//	LeaveCriticalSection(&m_cs);
//}
//
//void CComDataQueue::ReleaseLock( void )
//{
//	if (NULL != m_SemphoreHandle)
//	{
//		ReleaseSemaphore(m_SemphoreHandle,1,NULL);
//	}
//}

//线程循环标识
bool CSerialPort::s_bloop = true;
/*******************************************************************************************************************************
函数名称:DecodeComDataProc
功能描述:解析串口数据线程
输入参数:lpParam:
输出参数:无
返回值:DWORD
*******************************************************************************************************************************/
DWORD WINAPI CSerialPort::DecodeComDataProc( LPVOID lpParam )
{
	DWORD dwResult = 0;

	CSerialPort* pArg = (CSerialPort*)lpParam;

	if (NULL == pArg)
	{
		return dwResult;
	}

	while (s_bloop)
	{
		//if(pArg->m_nReadPos != 0)
		//{
		//	memcpy(pArg->m_procBuffer +  pArg->m_nReadPos,pArg->m_recvBuffer,pArg->m_nReadPos);

		//	//解析

		//}
		//CComDataQueue::LPCOMDATA pData = pArg->m_RecvQueue.GetHeadElement();
		//如果数据不为空
		//if (NULL != pData)
		{
			//解析数据---->GlobalDataManager
			//pData


			//弹出元素
			//pArg->m_RecvQueue.PopElement();
		}
	}
	//DELETE_POINTER(pArg);
	return dwResult;
}
#pragma warning(pop)