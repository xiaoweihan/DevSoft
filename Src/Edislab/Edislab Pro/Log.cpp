/**************************************************************************************************************************************************
版权所有:
文件名称:log.cpp
功能描述:完成打印日志的功能
作者:hanxiaowei
完成日期:2013-07-29
**************************************************************************************************************************************************/
#include "stdafx.h"
#include "log.h"
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#ifdef _WIN32
#pragma warning(disable:4996)
#pragma warning(disable:4251)
#pragma warning(disable:4554)
#endif

//缓冲区的最大长度
#define  BUFFER_MAX_SIZE          (1024 * 10)

//日志文件的最大容量
#define  LOG_MAX_SIZE                 (100 * 1024 * 1024)

//用于同步的类
static boost::mutex s_Lock;

//记录文件达到日志的等级的次数
static unsigned int s_nTimes = 1;

/********************************************************************************************************************************************************************/
//操作文件类的实现
/********************************************************************************************************************************************************************/
CHandleFile::CHandleFile( void ):
m_pFile(NULL)
{

}

CHandleFile::~CHandleFile( void )
{
	if (NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

}

/*****************************************************************************************************************************************
函数名称:Open
功能描述:打开文件
输入参数:szName:要打开的文件名称
输出参数:无
返回值:true:成功 false:失败
*****************************************************************************************************************************************/
bool CHandleFile::Open( const char* szName )
{
	if (NULL == szName)
	{
		return false;
	}

	//如果已经打开
	if (IsOpen())
	{
		return true;
	}

	m_pFile = fopen(szName,"a+");

	if (NULL == m_pFile)
	{
		return false;
	}

	return true;


}

/*****************************************************************************************************************************************
函数名称:Close
功能描述:关闭文件
输入参数:无
输出参数:无
返回值:true:成功 false:失败
*****************************************************************************************************************************************/
void CHandleFile::Close( void )
{
	if (NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}

}

/*****************************************************************************************************************************************
函数名称:Write
功能描述:向文件中写入数据
输入参数:pData:要写入的内容 nDataLen:写入内容的长度 
输出参数:uWriteLen:实际写入的内容的长度
返回值:true:成功 false:失败
*****************************************************************************************************************************************/
bool CHandleFile::Write( void* pData,unsigned int nDataLen,unsigned int& uWriteLen )
{
	uWriteLen = 0;
	//如果文件尚未打开
	if (!IsOpen())
	{
		return false;
	}

	int nAlreadyWriteByte = 0;

	nAlreadyWriteByte = (int)fwrite(pData,1,nDataLen,m_pFile);

	//写入失败
	if (nAlreadyWriteByte <= 0)
	{
		return false;
	}
	uWriteLen = nAlreadyWriteByte;
	return true;
}

/*****************************************************************************************************************************************
函数名称:Read
功能描述:从文件中读取数据
输入参数:pData:存放读取内容的缓冲区 nDataLen:要读取的字节数
输出参数:uReadLen:实际读取的内容的长度
返回值:true:成功 false:失败
*****************************************************************************************************************************************/
bool CHandleFile::Read( void* pData,unsigned int nDataLen,unsigned int& uReadLen )
{
	uReadLen = 0;

	if (!IsOpen())
	{
		return false;
	}

	int nReadByte = 0;

	nReadByte = (int)fread(pData,1,nDataLen,m_pFile);

	//读取失败
	if (nReadByte <= 0)
	{
		return false;
	}

	uReadLen = nReadByte;

	return true;
}

/*****************************************************************************************************************************************
函数名称:IsOpen
功能描述:判断文件是否已经打开
输入参数:无
输出参数:无
返回值:true:已经打开 false:没有打开
*****************************************************************************************************************************************/
bool CHandleFile::IsOpen( void )
{
	if (NULL == m_pFile)
	{
		return false;
	}
	return true;
}

/*****************************************************************************************************************************************
函数名称:SeekFilePos
功能描述:设置文件指针位置
输入参数:Pos:设置的相对位置 loffset:相对设置的偏移量 正数向后移，负数向前移
输出参数:无
返回值:true:设置成功 false:设置失败
*****************************************************************************************************************************************/
bool CHandleFile::SeekFilePos( FILEPOS Pos,long loffset )
{
	if (!IsOpen())
	{
		return false;
	}

	int nResult = 0;
	switch (Pos)
	{
	case POS_CURRENT:
		nResult = fseek(m_pFile,loffset,SEEK_CUR);
		break;
	case POS_HEAD:
		nResult = fseek(m_pFile,loffset,SEEK_SET);
		break;
	case POS_TAIL:
		nResult = fseek(m_pFile,loffset,SEEK_END);
		break;
	default:
		break;
	}

	//调用失败
	if (0 != nResult)
	{
		return false;
	}
	return true;
}

/*****************************************************************************************************************************************
函数名称:GetFileSize
功能描述:获取文件的大小
输入参数:无
输出参数:无
返回值:文件的大小
*****************************************************************************************************************************************/
unsigned long CHandleFile::GetFileSize( void )
{
	if (!IsOpen())
	{
		return -1;
	}

	//设置到文件尾
	if (!SeekFilePos(POS_TAIL,0))
	{
		return -1;
	}

	long lSize = ftell(m_pFile);
	return lSize;
}


/********************************************************************************************************************************************************************/

//日志类的实现
/********************************************************************************************************************************************************************/
/*****************************************************************************************************************************************
函数名称:CreateInstance
功能描述:创建日志类的唯一实例
输入参数:无
输出参数:无
返回值:唯一实例的指针
*****************************************************************************************************************************************/
CLog& CLog::CreateInstance( void )
{
	//唯一的一个日志实例类
	return s_Log;
}

/*****************************************************************************************************************************************
函数名称:SetLogPath
功能描述:设置日志文件的存放目录
输入参数:szLogDir:存放日志的目录
输出参数:无
返回值:无
*****************************************************************************************************************************************/
void CLog::SetLogPath( const char* szLogDir )
{
	if (NULL == szLogDir)
	{
		return;
	}
	m_strLogDir = szLogDir;
}

/*****************************************************************************************************************************************
函数名称:SetLogLevel
功能描述:设置日志级别
输入参数:loglevel:日志的等级
输出参数:无
返回值:无
*****************************************************************************************************************************************/
void CLog::SetLogLevel( LOGLEVEL loglevel )
{
	m_loglevel = loglevel;
}

/*****************************************************************************************************************************************
函数名称:WriteLog
功能描述:打印日志信息
输入参数:loglevel:日志的等级 szFormat:打印的格式  ...：变参
输出参数:无
返回值:无
*****************************************************************************************************************************************/
void CLog::WriteLog( LOGLEVEL loglevel,const char* szFormat,... )
{
	if (NULL == szFormat)
	{
		return;
	}
	//查看打印等级是否需要打印
	if (loglevel < m_loglevel)
	{
		return;
	}
	char szLogContent[BUFFER_MAX_SIZE] = {0};
	va_list varg;
	va_start(varg,szFormat);
	vsprintf(szLogContent,szFormat,varg);
	va_end(varg);

	//得到系统时间
	time_t logtime;
	time(&logtime);
	struct tm *plogtime = NULL; 
	plogtime = localtime(&logtime);
	char szTime[MAX_PATH] = {0};
	char szDayTime[MAX_PATH] = {0};
	//把时间格式化为字符串
	sprintf(szTime,"%02d-%02d-%02d %02d:%02d:%02d",plogtime->tm_year + 1900,plogtime->tm_mon + 1,plogtime->tm_mday,plogtime->tm_hour,plogtime->tm_min,plogtime->tm_sec);
	//判断前缀
	if (m_strLogPrefix.empty())
	{
		sprintf(szDayTime,"%s_%02d_%02d_%02d","log",plogtime->tm_year + 1900,plogtime->tm_mon + 1,plogtime->tm_mday);
	}
	else
	{
		sprintf(szDayTime,"%s_%02d_%02d_%02d",m_strLogPrefix.c_str(),plogtime->tm_year + 1900,plogtime->tm_mon + 1,plogtime->tm_mday);
	}
	char szLogLevel[MAX_PATH] = {0};
	switch (loglevel)
	{
	case LOG_DEBUG:
		strncpy(szLogLevel," DEBUG ",strlen(" DEBUG "));
		break;
	case LOG_INFO:
		strncpy(szLogLevel," INFO ",strlen(" INFO "));
		break;
	case LOG_WARN:
		strncpy(szLogLevel," WARN ",strlen(" WARN "));
		break;
	case LOG_ERROR:
		strncpy(szLogLevel," ERROR ",strlen(" ERROR "));
		break;
	case LOG_FATAL:
		strncpy(szLogLevel," FATAL ",strlen(" FATAL "));
		break;
	case LOG_NECESSARY:
		strncpy(szLogLevel," NECESSARY ",strlen(" NECESSARY "));
		break;
	default:
		break;
	}
	//查看是否存在
	char szLogName[MAX_PATH] = {0};
	//进入同步
	boost::lock_guard<boost::mutex> Lock(s_Lock);
	while (1)
	{
		memset(szLogName,0,sizeof(szLogName));
#ifdef _WIN32
		sprintf(szLogName,"%s\\%s_%d.dat",m_strLogDir.c_str(),szDayTime,s_nTimes);
		if (0 == _access(szLogName,0))
#else
		sprintf(szLogName,"%s/%s_%d.dat",m_strLogDir.c_str(),szDayTime,s_nTimes);
		if (0 == access(szLogName,0))
#endif
		{
			//查看文件的大小，是否超过100M
			CHandleFile file;
			if (file.Open(szLogName))
			{
				unsigned long lSize = file.GetFileSize();
				file.Close();
				//如果大于100M
				if (lSize >= LOG_MAX_SIZE)
				{
					s_nTimes++;
					continue;
					//查找前面写了多少个日志文件了
				}
				//如果不大于100M
				else
				{
					break;
				}
			}	
		}
		//如果不存在这个文件，则退出循环
		else
		{
			break;
		}
	}
	//开始写文件
	string strContent = string(szTime) + string(szLogLevel) + string(szLogContent) + string("\n");

	char szContent[BUFFER_MAX_SIZE] = {0};
	strncpy(szContent,strContent.c_str(),strContent.length());

	CHandleFile Writefile;

	if (Writefile.Open(szLogName))
	{
		unsigned int nWriteLength = 0;
		Writefile.Write(szContent,(unsigned int)strContent.length(),nWriteLength);
		Writefile.Close();
	}
}

CLog::CLog( void )
{
	m_loglevel = LOG_DEBUG;
}

CLog::~CLog( void )
{

}

void CLog::SetLogNamePrefix( const char* szPrefix )
{
	if (NULL == szPrefix)
	{
		return;
	}

	m_strLogPrefix = szPrefix;

}

CLog CLog::s_Log;

/********************************************************************************************************************************************************************/