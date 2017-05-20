// BaseLib.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SerialPort.h"

//串口默认的接受缓冲区大小
#define  COM_IN_SIZE       (1024 * 1024)
//串口默认的发送缓冲区大小
#define  COM_OUT_SIZE   (1024 * 1024)
//不使用流控制
# define   NO_USE_FLOW_CONTROL            (0)
//使用硬件流控制(RTS/CTS方式)
#define    USE_HARDWARE_FLOW_CONTROL          (1)
//操作COM口类的实现
CHandleCom::CHandleCom(void):
m_handle(INVALID_HANDLE_VALUE)
{
}

CHandleCom::~CHandleCom(void)
{

}

/*****************************************************************************************************************************************
函数名称:Open
功能描述:打开COM口
输入参数:szName:要打开的串口名称
输出参数:无
返回值:true:成功 false:失败
*****************************************************************************************************************************************/
bool CHandleCom::Open(const char* szName)
{
	if (NULL == szName)
	{
		return false;
	}
	//首先判断串口是否打开
	if (IsOpen())
	{
		return true;
	}
	//同步打开串口
	m_handle = CreateFileA(szName,GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);

	if (INVALID_HANDLE_VALUE == m_handle)
	{
		return false;
	}
	//设置串口的接受缓冲和发送缓冲
	SetupComm(m_handle,COM_IN_SIZE,COM_OUT_SIZE);
	//清空串口的接收缓冲和发送缓冲
	PurgeComm(m_handle,PURGE_RXCLEAR | PURGE_TXCLEAR);
	return true;

}

/*****************************************************************************************************************************************
函数名称:Close
功能描述:关闭COM口
输入参数:无
输出参数:无
返回值:true:成功 false:失败
*****************************************************************************************************************************************/
void CHandleCom::Close(void)
{
	if (INVALID_HANDLE_VALUE != m_handle)
	{
		CloseHandle(m_handle);
		m_handle = INVALID_HANDLE_VALUE;
	}
}

/*****************************************************************************************************************************************
函数名称:Write
功能描述:向串口写入数据
输入参数:pData:要写入的内容 nDataLen:写入内容的长度 
输出参数:uWriteLen:实际写入的内容的长度
返回值:true:成功 false:失败
*****************************************************************************************************************************************/
bool CHandleCom::Write(void* pData,unsigned int nDataLen,unsigned int& uWriteLen)
{
	if (NULL == pData || 0 == nDataLen || nDataLen >= COM_OUT_SIZE)
	{
		return false;
	}
	uWriteLen = 0;
	//判断串口是否已经打开
	if (IsOpen())
	{
		//使用了硬件流驱动
		if (USE_HARDWARE_FLOW_CONTROL == m_comproperty.nFlowControl)
		{
			//查看CTS的电平高低
			while (1)
			{
				DWORD dwStatus = 0;
				if (GetCommModemStatus(m_handle,&dwStatus))
				{
					//CTS为高电平
					if (dwStatus == MS_CTS_ON)
					{
						break;
					}
					else
					{
						Sleep(100);
						continue;
					}
				}
				//获取失败
				else
				{
					return false;
				}
			}
		}

		//实际写入的字节数
		DWORD dwWriteByte = 0;
		if (WriteFile(m_handle,pData,nDataLen,&dwWriteByte,NULL))
		{
			uWriteLen = (unsigned int)dwWriteByte;
			return true;
		}
		//如果写入失败，返回false
		return false;
	}
	//如果尚未打开串口
	return false;
}

/*****************************************************************************************************************************************
函数名称:Read
功能描述:从串口读取数据
输入参数:pData:存放读取内容的缓冲区 nDataLen:要读取的字节数
输出参数:uReadLen:实际读取的内容的长度
返回值:true:成功 false:失败
*****************************************************************************************************************************************/
bool CHandleCom::Read(void* pData,unsigned int nDataLen,unsigned int& uReadLen)
{
	if (NULL == pData || 0 == nDataLen)
	{
		return false;
	}
	uReadLen = 0;
	//判断串口是否已经打开
	if (!IsOpen())
	{
		return false;
	}

#ifdef _WIN32

	//标志可以接受
#if 0
	DWORD dwStatus = SETRTS;
	if (!EscapeCommFunction(m_handle,dwStatus))
	{
		return false;
	}
#endif
	//实际读入的字节数
	DWORD dwReadByte = 0;
	if (ReadFile(m_handle,pData,nDataLen,&dwReadByte,NULL))
	{
		uReadLen = (unsigned int)dwReadByte;
		//PurgeComm(m_handle,PURGE_RXCLEAR | PURGE_TXCLEAR);
		return true;
	}
#else
	//记录实际读取的字节数
	unsigned int nReadByte = 0;
	//进行读的操作
	nReadByte = read(m_handle,pData,nDataLen);
	//读取成功
	if (nReadByte != -1)
	{
		uReadLen = nReadByte;
		return true;
	}
#endif
	return false;
}

/*****************************************************************************************************************************************
函数名称:IsOpen
功能描述:判断COM是否已经打开
输入参数:无
输出参数:无
返回值:true:已经打开 false:没有打开
*****************************************************************************************************************************************/
bool CHandleCom::IsOpen(void)
{
	if (INVALID_HANDLE_VALUE == m_handle)
	{
		return false;
	}
	return true;
}

/*****************************************************************************************************************************************
函数名称:GetComProperty
功能描述:获取打开串口的当前属性
输入参数:无
输出参数:pProperty:保存串口的属性
返回值:true:获取成功 false:获取失败
*****************************************************************************************************************************************/
bool CHandleCom::GetComProperty(LPCOMPROPERTY pProperty)
{
	if (NULL == pProperty)
	{
		return false;
	}

	*pProperty = m_comproperty;
	return true;

}

/****************************************************************************************************************************************
函数名称:SetComProperty
功能描述:设置打开串口的当前属性
输入参数:pProperty:设置串口的属性
输出参数:无
返回值:true:设置成功 false:设置失败
*****************************************************************************************************************************************/
bool CHandleCom::SetComProperty(LPCOMPROPERTY pProperty)
{
	if (NULL == pProperty)
	{
		return false;
	}

	//如果串口还没有打开
	if (!IsOpen())
	{
		return false;
	}
#ifdef _WIN32
	DCB ComDcb;
	//获取当前串口的配置
	if (!GetCommState(m_handle,&ComDcb))
	{
		return false;
	}

	switch (pProperty->nFlowControl)
	{
		//不使用流控
	case NO_USE_FLOW_CONTROL:
		{
			ComDcb.fOutxCtsFlow = 0;
			ComDcb.fOutxDsrFlow = 0;
			ComDcb.fDsrSensitivity = 0;
			ComDcb.fRtsControl = 0;
			ComDcb.fDtrControl = 0;

			ComDcb.fInX = 0; 
			ComDcb.fOutX = 0;
			ComDcb.XoffChar = 0;
			ComDcb.XonChar = 0;
			ComDcb.XoffLim = 0;
			ComDcb.XonLim = 0;
		}

		break;
		//使用硬件流控RTS/CTS方式
	case USE_HARDWARE_FLOW_CONTROL:
		{
			ComDcb.fOutxCtsFlow = 1;
			ComDcb.fOutxDsrFlow = 0;
			ComDcb.fDsrSensitivity = 0;
			ComDcb.fRtsControl = RTS_CONTROL_ENABLE;
			ComDcb.fDtrControl = 0;

			ComDcb.fInX = 0; 
			ComDcb.fOutX = 0;
			ComDcb.XoffChar = 0;
			ComDcb.XonChar = 0;
			ComDcb.XoffLim = 0;
			ComDcb.XonLim = 0;
		}
		break;
	default:
		break;
	}
	//设置波特率
	ComDcb.BaudRate = pProperty->nBaudRate;
	//设置数据位
	ComDcb.ByteSize = pProperty->nDataBits;
	//设置停止位
	ComDcb.StopBits = pProperty->nStopBits;
	//设置校验位
	ComDcb.Parity = pProperty->nPairty;


	//设置串口
	if (!SetCommState(m_handle,&ComDcb))
	{
		return false;
	}
#else
	//获取串口当前设置
	struct termios newtio;
	//struct termios oldtio;
	memset(&newtio,0,sizeof(newtio));
	//memset(&oldtio,0,sizeof(oldtio));
	//获取失败
	if (tcgetattr(m_handle,&newtio) != 0)
	{
		return false;
	}
	newtio.c_cflag |= (CLOCAL | CREAD);
	newtio.c_cflag &= ~CSIZE;
	//数据位
	switch (pProperty->nDataBits)
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	default:
		break;
	}
	//校验位
	switch (pProperty->nPairty)
	{
		//没有校验位
	case 0:
		newtio.c_cflag &= ~PARENB;
		newtio.c_iflag &= ~INPCK;
		break;
		//奇校验
	case 1:
		newtio.c_cflag |= (PARODD | PARENB);
		newtio.c_iflag |= INPCK;
		break;
		//偶校验
	case 2:
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		newtio.c_iflag |= INPCK;
		break;
		//空格校验
	case 4:
		newtio.c_cflag &= ~PARENB;
		newtio.c_cflag &= ~CSTOPB;
		newtio.c_iflag |= INPCK;
		break;
	default:
		break;
	}
	//停止位
	switch (pProperty->nPairty)
	{
		//1个停止位
	case 0:
		newtio.c_cflag &= ~CSTOPB;
		break;
		//2个停止位
	case 2:
		newtio.c_cflag |= CSTOPB;
		break;
	default:
		break;
	}

	//设置波特率
	cfsetispeed(&newtio,pProperty->nBaudRate);
	cfsetospeed(&newtio,pProperty->nBaudRate);
	//设置串口
	if (tcsetattr(m_handle,TCSANOW,&newtio) != 0)
	{
		return false;
	}
#endif
	m_comproperty = *pProperty;
	return true;
}

/****************************************************************************************************************************************
函数名称:SetTimeout
功能描述:设置串口的读写操作时间
输入参数:nMillSeconds:超时时间，单位是毫秒
输出参数:无
返回值:true:设置成功 false:设置失败
*****************************************************************************************************************************************/
bool CHandleCom::SetTimeout(unsigned int nMillSeconds)
{  
	//判断串口是否已经打开
	if (!IsOpen())
	{
		return false;
	}

#ifdef _WIN32
	COMMTIMEOUTS Timeouts;
	ZeroMemory(&Timeouts,sizeof(COMMTIMEOUTS));
	Timeouts.ReadTotalTimeoutConstant = nMillSeconds;
	Timeouts.WriteTotalTimeoutConstant = nMillSeconds;

	//设置失败
	if (!SetCommTimeouts(m_handle,&Timeouts))
	{
		return false;
	}
#else
	//获取串口当前设置
	struct termios newtio;
	memset(&newtio,0,sizeof(newtio));
	//获取失败
	if (tcgetattr(m_handle,&newtio) != 0)
	{
		return false;
	}

	newtio.[VTIME] = nMillSeconds / 100;
	newtio.c_cc[VMIN] = 0;

	if (tcsetattr(m_handle,TCSANOW,&newtio) != 0)
	{
		return false;
	}
#endif
	return true;
}

void CHandleCom::ResetSendBuffer( void )
{
	if (IsOpen())
	{
#ifdef _WIN32
		PurgeComm(m_handle,PURGE_TXCLEAR);
#else
		tcflush(m_handle,TCOFLUSH);
#endif
	}

}

void CHandleCom::ResetRecvBuffer( void )
{
	if (IsOpen())
	{
#ifdef _WIN32
		PurgeComm(m_handle,PURGE_RXCLEAR);
#else
		tcflush(m_handle,TCIFLUSH);
#endif
	}

}
