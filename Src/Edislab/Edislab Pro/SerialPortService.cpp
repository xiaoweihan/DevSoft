#include "stdafx.h"
#include "SerialPortService.h"
#include <boost/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "Log.h"
#include "SensorManager.h"
#include "SensorDataManager.h"
#include "SensorData.h"
#include "Utility.h"
#include "Msg.h"
//接收缓冲区的大小(1MB)
const int MAX_BUFFER_SIZE = (1 << 20);
//发送指令的缓冲区大小
const int COMMAND_BUFFER_SIZE = 7;
//存放传感器名称的数组大小
const int MAX_SENSOR_NAME_LENGTH = 100;

CSerialPortService& CSerialPortService::CreateInstance()
{
	return s_obj;
}

CSerialPortService::CSerialPortService():
	m_Work(m_IoService),
	m_SerialPort(m_IoService),
	m_nUseBufferBytes(0),
	m_pRecvBuffer(nullptr)
{

	if (nullptr == m_pRecvBuffer)
	{
		m_pRecvBuffer = new BYTE[MAX_BUFFER_SIZE];
	}

}


CSerialPortService::~CSerialPortService()
{
	if (nullptr != m_pRecvBuffer)
	{
		delete[]m_pRecvBuffer;
		m_pRecvBuffer = nullptr;
	}
	m_nUseBufferBytes = 0;
}

void CSerialPortService::StartSerialPortService(void)
{
	auto ThreadProc = [this]()->void
	{
		//初始化串口
		if (!InitSerialPort())
		{
			ERROR_LOG("InitSerialPort failed.");
			return;
		}
		//开始异步读取数据
		m_SerialPort.async_read_some(boost::asio::buffer(m_pRecvBuffer + m_nUseBufferBytes, MAX_BUFFER_SIZE - m_nUseBufferBytes), boost::bind(&CSerialPortService::ReadHandler, this, boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

		boost::system::error_code ec;
		m_IoService.run(ec);

	};


	if (!m_pRecvThread)
	{
		m_pRecvThread = boost::make_shared<boost::thread>(ThreadProc);
	}

}

void CSerialPortService::StopSerialPortService(void)
{

	if (m_pRecvThread)
	{
		m_IoService.stop();
		m_SerialPort.close();
		m_pRecvThread->join();
	}
	
}


void CSerialPortService::StartSensorCollect( const std::string& strSensorName )
{
	if (strSensorName.empty())
	{
		return;
	}

	int nSensorNameLength = (int)strSensorName.length();
	int nFrameLength =  1 + 1 + 1 + nSensorNameLength;
	int nTotalLength = nFrameLength + 2;
	BYTE* pData = new BYTE[nTotalLength];
	if (nullptr == pData)
	{
		return;
	}
	pData[0] = 0xAB;
	pData[1] = (BYTE)nFrameLength;
	pData[2] = (BYTE)nSensorNameLength;
	memcpy(pData + 3,strSensorName.c_str(),nSensorNameLength);
	pData[nSensorNameLength + 3] = 0x00;
	pData[nSensorNameLength + 4] = Utility::CalCRC8(pData,nTotalLength - 1);

	AsyncWriteData(pData,nTotalLength);
}

void CSerialPortService::StartSensorCollect(int nSensorTypeID,int nSensorSeqID)
{
	BYTE* pData = new BYTE[COMMAND_BUFFER_SIZE];
	if (nullptr == pData)
	{
		return;
	}
	memset(pData,0,sizeof(BYTE) * COMMAND_BUFFER_SIZE);
	pData[0] = 0xAB;
	pData[1] = (BYTE)nSensorTypeID;
	pData[2] = (BYTE)nSensorSeqID;
	pData[COMMAND_BUFFER_SIZE - 1] = 0x00;

	AsyncWriteData(pData,COMMAND_BUFFER_SIZE);
}

void CSerialPortService::StopSensorCollect( const std::string& strSensorName )
{
	if (strSensorName.empty())
	{
		return;
	}
	int nSensorNameLength = (int)strSensorName.length();
	int nFrameLength =  1 + 1 + 1 + nSensorNameLength;
	int nTotalLength = nFrameLength + 2;
	BYTE* pData = new BYTE[nTotalLength];
	if (nullptr == pData)
	{
		return;
	}
	pData[0] = 0xAB;
	pData[1] = (BYTE)nFrameLength;
	pData[2] = (BYTE)nSensorNameLength;
	memcpy(pData + 3,strSensorName.c_str(),nSensorNameLength);
	pData[nSensorNameLength + 3] = 0x01;
	pData[nSensorNameLength + 4] = Utility::CalCRC8(pData,nTotalLength - 1);
	
	AsyncWriteData(pData,nTotalLength);
}

void CSerialPortService::StopSensorCollect(int nSensorTypeID,int nSensorSeqID)
{
	BYTE* pData = new BYTE[COMMAND_BUFFER_SIZE];
	if (nullptr == pData)
	{
		return;
	}
	memset(pData,0,sizeof(BYTE) * COMMAND_BUFFER_SIZE);
	pData[0] = 0xAB;
	pData[1] = (BYTE)nSensorTypeID;
	pData[2] = (BYTE)nSensorSeqID;
	pData[COMMAND_BUFFER_SIZE - 1] = 0x01;

	AsyncWriteData(pData,COMMAND_BUFFER_SIZE);
}

void CSerialPortService::SetSensorFrequence( const std::string& strSensorName,int nMillSecond )
{
	if (strSensorName.empty())
	{
		return;
	}

	//设置采样周期
	int nSensorNameLength = (int)strSensorName.length();
	int nMsgLength = 6 + nSensorNameLength;
	BYTE* pSendBuffer = new BYTE[nMsgLength];
	if (nullptr == pSendBuffer)
	{
		return;
	}

	ZeroMemory(pSendBuffer,nMsgLength);
	pSendBuffer[0] = 0xAF;
	pSendBuffer[1] = nMsgLength - 2;
	pSendBuffer[2] = nSensorNameLength;
	memcpy(pSendBuffer + 3,strSensorName.c_str(),nSensorNameLength);
	pSendBuffer[nMsgLength - 3] = (BYTE)(nMillSecond & 0x00FF);
	pSendBuffer[nMsgLength - 2] = (BYTE)((nMillSecond & 0xFF00) >> 8);
	pSendBuffer[nMsgLength - 1] = Utility::CalCRC8(pSendBuffer,nMsgLength - 1);

	AsyncWriteData(pSendBuffer,nMsgLength);
}

void CSerialPortService::SetSensorFrequence(int nSensorTypeID,int nSensorSeqID,int nMillSecond)
{

	//设置采样周期
	BYTE* pSendBuffer = new BYTE[COMMAND_BUFFER_SIZE];
	if (nullptr == pSendBuffer)
	{
		return;
	}

	memset(pSendBuffer,0,sizeof(BYTE) * COMMAND_BUFFER_SIZE);
	pSendBuffer[0] = 0xAF;
	pSendBuffer[1] = (BYTE)nSensorTypeID;
	pSendBuffer[2] = (BYTE)nSensorSeqID;
	pSendBuffer[COMMAND_BUFFER_SIZE - 2] = (BYTE)((nMillSecond & 0xFF00) >> 8);
	pSendBuffer[COMMAND_BUFFER_SIZE - 1] = (BYTE)(nMillSecond & 0x00FF);

	
	AsyncWriteData(pSendBuffer,COMMAND_BUFFER_SIZE);
}

void CSerialPortService::AsyncWriteData(BYTE* pData, int nDataLength)
{
	if (nullptr == pData || 0 == nDataLength)
	{
		return;
	}

	//boost::this_thread::sleep(boost::posix_time::seconds(1));
	m_SerialPort.async_write_some(boost::asio::buffer(pData,nDataLength),boost::bind(&CSerialPortService::WriteHandler,this,pData,nDataLength, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

//任务通知完成回调
void CSerialPortService::ReadHandler(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	if (ec)
	{
		return;
	}
	//分析是否有整的数据包
	int nRecvBytes = static_cast<int>(bytes_transferred);
	m_nUseBufferBytes += nRecvBytes;
	//处理数据
	int nHandledBytes = HandlerData(m_pRecvBuffer,m_nUseBufferBytes);
	//移动数据块
	memmove(m_pRecvBuffer, m_pRecvBuffer + nHandledBytes, (m_nUseBufferBytes - nHandledBytes));
	m_nUseBufferBytes -= nHandledBytes;
	m_SerialPort.async_read_some(boost::asio::buffer(m_pRecvBuffer + m_nUseBufferBytes, MAX_BUFFER_SIZE - m_nUseBufferBytes), boost::bind(&CSerialPortService::ReadHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

/**************************************************************************
@FunctionName:HandlerData
@FunctionDestription:处理数据
@InputParam:
@OutPutParam:
@ReturnValue:返回已经处理的字节数
**************************************************************************/
int CSerialPortService::HandlerData(BYTE* pData, int nDataLength)
{
	int nHandledBytes = 0;
	if (nullptr == pData || 0 == nDataLength)
	{
		return 0;
	}
	int nIndex = 0;

	while (nIndex < nDataLength)
	{
		//上线通知
		if (pData[nIndex] == 0xBA)
		{
			//判断数组不越界
			if (nIndex + 6 < nDataLength)
			{
				//获取传感器类型编号
				int nSensorTypeID = static_cast<int>(pData[nIndex + 1]);
				//获取同类传感器序号
				int nSensorSeqID = static_cast<int>(pData[nIndex + 2]);

				if (nSensorSeqID >= 0 && nSensorTypeID >= 0)
				{
					LP_SENSOR_TYPE_KEY pSensorInfo = new SENSOR_TYPE_KEY(nSensorTypeID,nSensorSeqID);
		
					//上线
					if (0x01 == pData[nIndex + 6])
					{
						DEBUG_LOG("detect the device online,the SensorType[%d],the SensorSeq[%d].",nSensorTypeID,nSensorSeqID);
						//添加传感器
						CSensorManager::CreateInstance().RegisterSensor(nSensorTypeID,nSensorSeqID);
						//添加对应SensorID的数据
						CSensorDataManager::CreateInstance().AddSensorData(SENSOR_TYPE_KEY(nSensorTypeID,nSensorSeqID));
						//通知主窗口
						::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_NOTIFY_DETECT_DEVICE,(WPARAM)pSensorInfo,1);
					}
					//下线
					else
					{
						DEBUG_LOG("detect the device offline,the SensorType[%d],the SensorSeq[%d].",nSensorTypeID,nSensorSeqID);
						//删除数据
						CSensorDataManager::CreateInstance().DelSensorData(SENSOR_TYPE_KEY(nSensorTypeID,nSensorSeqID));
						//删除传感器
						CSensorManager::CreateInstance().UnRegisterSensor(nSensorTypeID,nSensorSeqID);
						//通知主窗口
						::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_NOTIFY_DETECT_DEVICE,(WPARAM)pSensorInfo,0);
					}

				}
				//处理数据
				nIndex += 7;
				//已经处理的数据递增
				nHandledBytes += 7;
				continue;
			}
		}
		//传感器数据
		else if (0xBD == pData[nIndex])
		{
			//获取传感器类型编号
			int nSensorTypeID = static_cast<int>(pData[nIndex + 1]);
			//获取同类传感器序号
			int nSensorSeqID = static_cast<int>(pData[nIndex + 2]);
			float fValue = 0.0f;
			memcpy(&fValue, &pData[nIndex + 3],4);

			if (nSensorTypeID >= 0 && nSensorSeqID >= 0)
			{
				if (fValue > 0)
				{
					DEBUG_LOG("receive the device data[%.2f],the SensorType[%d],the SensorSeq[%d].",fValue,nSensorTypeID,nSensorSeqID);
					//根据ID获取数据
					CSensorData* pSensorData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(SENSOR_TYPE_KEY(nSensorTypeID,nSensorSeqID));
					if (nullptr != pSensorData)
					{
						pSensorData->AddSensorData(fValue);
					}
				}

			}
			nIndex += 7;
			nHandledBytes += 7;
			continue;
		}
		else
		{
			++nIndex;
		}
	}
	return nHandledBytes;
#if 0
	char szSensorName[MAX_SENSOR_NAME_LENGTH] = { 0 };
	while (nIndex < nDataLength)
	{	
		//传感器上下线通知
		if (pData[nIndex] == 0xBA)
		{
			//确保数组不越界
			if (nIndex + 1 < nDataLength)
			{
				//获取数据长度
				int nFrameLength = (int)pData[nIndex + 1];
				//判断整包长度
				if ((nIndex + nFrameLength + 1) < nDataLength)
				{
					if (Utility::CalCRC8(&pData[nIndex], nFrameLength + 1) == pData[nIndex + nFrameLength + 1])
					{
						int nSensorNameLength = (int)pData[nIndex + 1 + 1];
						memset(szSensorName,0,sizeof(szSensorName));
						//获取传感器名称
						memcpy(szSensorName, &pData[nIndex + 1 + 1 + 1], nSensorNameLength);
						//传感器上线
						if (pData[nIndex + 1 + 1 + 1 + nSensorNameLength] == 0x01)
						{
							DEBUG_LOG("the device [%s] is on.",szSensorName);

							//判断传感器是否已经存在
							if (!CSensorIDGenerator::CreateInstance().IsSensorExist(szSensorName))
							{
								//添加传感器
								int nSensorID = CSensorIDGenerator::CreateInstance().AddSensor(szSensorName);
								if (nSensorID >= 0)
								{
									//添加对应SensorID的数据
									CSensorDataManager::CreateInstance().AddSensorData(nSensorID);
								}

								//通知设备上线
								std::string* pDeviceName = new std::string(szSensorName);
								//通知设备下线
								::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_NOTIFY_DETECT_DEVICE,(WPARAM)pDeviceName,1);
							}
						}
						//传感器下线
						else
						{
							DEBUG_LOG("the device [%s] is off.",szSensorName);
							//根据名称查询到设备的ID
							int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(szSensorName);
							//根据设备ID删除设备的数据
							if (nSensorID >= 0)
							{
								CSensorDataManager::CreateInstance().DelSensorData(nSensorID);
							}
							//从传感器ID管理中删除
							CSensorIDGenerator::CreateInstance().DelSensor(szSensorName);

							std::string* pDeviceName = new std::string(szSensorName);
							//通知设备下线
							::PostMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_NOTIFY_DETECT_DEVICE,(WPARAM)pDeviceName,0);
						}
						nIndex += (nFrameLength + 2);
						//修改数据的处理
						nHandledBytes += (nFrameLength + 2);
						continue;
					}
				}
			}
		}

		//传感器上报数据通知
		else if (pData[nIndex] == 0xBD)
		{
			//处理数据上报通知
			if (nIndex + 1 < nDataLength)
			{
				//获取数据长度
				int nFrameLength = (int)pData[nIndex + 1];
				//判断整包长度
				if ((nIndex + nFrameLength + 1) < nDataLength)
				{
					if (Utility::CalCRC8(&pData[nIndex], nFrameLength + 1) == pData[nIndex + nFrameLength + 1])
					{
						int nSensorNameLength = (int)pData[nIndex + 1 + 1];
						//获取传感器名称
						memset(szSensorName,0,sizeof(szSensorName));
						memcpy(szSensorName, &pData[nIndex + 1 + 1 + 1], nSensorNameLength);
						float fValue = 0.0f;
						memcpy(&fValue, &pData[nIndex + 1 + 1 + 1 + nSensorNameLength], 4);
						nIndex += (nFrameLength + 2);
						//修改数据的处理
						nHandledBytes += (nFrameLength + 2);
						
						//拷贝数据
						if (fValue > 0)
						{
							DEBUG_LOG("the device [%s] data is [%.2f].",szSensorName,fValue);
							//根据传感器名称获取ID
							int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(szSensorName);
							if (nSensorID >= 0)
							{
								//根据ID获取数据
								CSensorData* pSensorData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(nSensorID);
								if (nullptr != pSensorData)
								{
									pSensorData->AddSensorData(fValue);
								}
							}
						}
						//boost::posix_time::ptime p = boost::posix_time::second_clock::local_time();
						//std::cout << "["<<p<<"] "<<"the sensor [" << szSensorName << "] value is [" << fValue << "]" << std::endl;
						continue;
					}
				}
			}
		}
		else
		{		
		}
		++nIndex;
	}
#endif
	
}

void CSerialPortService::WriteHandler(BYTE* pData, int nDataLength, const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	using namespace std;
	if (ec)
	{
		return;
	}

	if (nDataLength != bytes_transferred)
	{
		ERROR_LOG("WriteHandler failed.");
		return;
	}

	DELETE_ARRAY_POINTER(pData);
}

bool CSerialPortService::InitSerialPort(void)
{

	boost::system::error_code ec;
	m_SerialPort.open(m_SerialOption.strSerialPort, ec);
	if (ec)
	{
		return false;
	}
	//波特率
	m_SerialPort.set_option(boost::asio::serial_port::baud_rate(m_SerialOption.nBaudRate),ec);    
	if (ec)
	{
		return false;
	}

	if (m_SerialOption.bFlowControl)
	{
		m_SerialPort.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::hardware));  //流控制
	}
	else
	{
		m_SerialPort.set_option(boost::asio::serial_port::flow_control(boost::asio::serial_port::flow_control::none));  //流控制
	}
	if (ec)
	{
		return false;
	}

	switch (m_SerialOption.nPairty)
	{
		//不使用校验
	case 0:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none),ec); //奇偶校验
		break;
		//奇数校验
	case 1:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::odd), ec);
		break;
		//偶数校验
	case 2:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::even), ec);
		break;
	default:
		m_SerialPort.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none), ec);
		break;
	}
	if (ec)
	{
		return false;
	}

	switch (m_SerialOption.nStopBits)
	{
	case 0:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one),ec); //停止位
		break;
	case 1:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::onepointfive),ec); //停止位
		break;
	case 2:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::two),ec); //停止位
		break;
	default:
		m_SerialPort.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one), ec);
		break;
	}

	if (ec)
	{
		return false;
	}

	m_SerialPort.set_option(boost::asio::serial_port::character_size(m_SerialOption.nDataBits),ec);// 通信字节位数，4—8
	if (ec)
	{
		return false;
	}
	return true;
}

CSerialPortService CSerialPortService::s_obj;

