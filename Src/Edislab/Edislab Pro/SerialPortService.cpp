#include "stdafx.h"
#include "SerialPortService.h"
#include <boost/bind.hpp>
#include "Log.h"
#include "SensorIDGenerator.h"
#include "SensorDataManager.h"
#include "SensorData.h"
#include "Utility.h"
//接收缓冲区的大小
const int MAX_BUFFER_SIZE = 100;
CSerialPortService::CSerialPortService():
	m_bLoop(false),
	m_nSerialPort(-1),
	m_pDeviceNameBuffer(nullptr),
	m_nDeviceNameBufferLength(0),
	m_bCopyDeviceName(false)
{
}


CSerialPortService::~CSerialPortService()
{
	DELETE_ARRAY_POINTER(m_pDeviceNameBuffer);
	m_nDeviceNameBufferLength = 0;
}

//开启串口服务
void CSerialPortService::StartSerialPortService(void)
{
	char szCom[MAX_BUFFER_SIZE] = { 0 };

	sprintf_s(szCom, "\\\\.\\COM%d", m_nSerialPort);

	//打开串口
	if (!m_Com.Open(szCom))
	{
		ERROR_LOG("open com failed.");
		return;
	}

	//设置通信选项
	if (!m_Com.SetComProperty(&m_SerialPortOpt))
	{
		ERROR_LOG("SetComProperty failed.");
		return;
	}

	//设置读取的超时时间
	if (!m_Com.SetTimeout(1000))
	{
		ERROR_LOG("SetTimeout failed.");
		return;
	}

	//开启读取线程
	if (m_pReceThread)
	{
		m_bLoop = true;
		//开始通信
		m_pReceThread = boost::make_shared<boost::thread>(boost::bind(&CSerialPortService::ReceiveProc,this));
	}
}

//停止串口服务
void CSerialPortService::StopSerialPortService(void)
{
	m_bLoop = false;
	if (m_pReceThread)
	{
		m_pReceThread->join();
	}
	m_Com.Close();
}

//开启接收线程
void CSerialPortService::ReceiveProc(void)
{
	BYTE chHead = 0;
	unsigned int nReadByte = 0;
	while (m_bLoop)
	{
		//读取头
		if (m_Com.Read(&chHead,sizeof(chHead),nReadByte))
		{
			if (nReadByte == sizeof(chHead))
			{
				switch (chHead)
				{
					//上线或者下线
				case 0xBA:
					HandleDeviceOnOffMsg();
					break;
					//周期上报数据
				case 0xBD:
					HandleDeviceDataMsg();
					break;
				default:
					break;
				}
			}	
		}
		else
		{
			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}
	}
}

void CSerialPortService::HandleDeviceOnOffMsg(void)
{
	BYTE szDeviceName[MAX_BUFFER_SIZE] = { 0 };
	//再接收一个字节
	BYTE chLength = 0;
	unsigned int nReadByte = 0;
	if (m_Com.Read(&chLength, sizeof(chLength), nReadByte))
	{
		if (nReadByte == sizeof(chLength))
		{
			//接收剩下的内容
			BYTE* pData = new BYTE[chLength + 2];
			if (nullptr != pData)
			{
				pData[0] = 0xBA;
				pData[1] = chLength;
				if (m_Com.Read(pData + 2, chLength, nReadByte))
				{
					if (chLength == nReadByte)
					{
						//判断是否符合CRC
						if (CalCRC8(pData, chLength + 1) == pData[chLength + 1])
						{
							//获取传感器名称长度
							int nDeviceNameLength = pData[2];
							memcpy(szDeviceName, pData + 3, nDeviceNameLength);
							//拷贝数据名称
							CopyDeviceName(pData + 3,nDeviceNameLength);
							//下线通知
							if (0x00 == pData[chLength])
							{
								NECESSARY_LOG("the device [%s] is off.",szDeviceName);
								//从传感器ID管理中删除
								CSensorIDGenerator::CreateInstance().DelSensor(std::string((char*)szDeviceName));
							}
							//上限通知
							else
							{
								NECESSARY_LOG("the device [%s] is on.",szDeviceName);
							}
						}
					}
				}
				delete pData;
				pData = nullptr;
			}
		}
	}
}

void CSerialPortService::HandleDeviceDataMsg(void)
{
	BYTE szDeviceName[MAX_BUFFER_SIZE] = { 0 };
	//再接收一个字节
	BYTE chLength = 0;
	unsigned int nReadByte = 0;
	if (m_Com.Read(&chLength, sizeof(chLength), nReadByte))
	{
		if (nReadByte == sizeof(chLength))
		{
			//接收剩下的内容
			BYTE* pData = new BYTE[chLength + 2];
			if (nullptr != pData)
			{
				pData[0] = 0xBD;
				pData[1] = chLength;
				if (m_Com.Read(pData + 2, chLength, nReadByte))
				{
					if (chLength == nReadByte)
					{
						//判断是否符合CRC
						if (CalCRC8(pData, chLength + 1) == pData[chLength + 1])
						{
							//获取传感器名称长度
							int nDeviceNameLength = pData[2];
							memcpy(szDeviceName, pData + 3, nDeviceNameLength);
							//拷贝数据名称
							CopyDeviceName(pData + 3,nDeviceNameLength);
							//获取数据
							float fValue = 0.0f;
							memcpy(&fValue, pData + 3 + nDeviceNameLength, 4);
							if (fValue > 0)
							{
								NECESSARY_LOG("the device [%s] data is [%.1f].",szDeviceName,fValue);
								//根据传感器名称获取ID
								int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(std::string((char*)szDeviceName));
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
						}
					}
				}
				delete pData;
				pData = nullptr;
			}
		}
	}


}

BYTE CSerialPortService::CalCRC8( BYTE* pBuf,unsigned int nsize )
{
	BYTE crc = 0;

	if (nullptr == pBuf || 0 == nsize)
	{
		return crc;
	}

	while (nsize--)
	{
		crc ^= *pBuf;
		pBuf++;
	}
	return crc;
}

void CSerialPortService::CopyDeviceName( BYTE* pData,int nDataLength )
{
	if (nullptr == pData || 0 == nDataLength)
	{
		return;
	}

	if (m_bCopyDeviceName)
	{
		DELETE_ARRAY_POINTER(m_pDeviceNameBuffer);
		m_nDeviceNameBufferLength = 0;
		m_pDeviceNameBuffer = new BYTE[nDataLength];
		if (nullptr != m_pDeviceNameBuffer)
		{
			memcpy(m_pDeviceNameBuffer,pData,nDataLength);
			m_nDeviceNameBufferLength = nDataLength;
			m_bCopyDeviceName = false;
		}
	}
}

