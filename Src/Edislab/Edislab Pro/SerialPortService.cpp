#include "stdafx.h"
#include "SerialPortService.h"
#include <boost/bind.hpp>
#include <iostream>
CSerialPortService::CSerialPortService():
	m_bLoop(false)
{
}


CSerialPortService::~CSerialPortService()
{
}

void CSerialPortService::StartSerialPortService(void)
{

	char szCom[100] = { 0 };

	sprintf_s(szCom, "\\\\.\\COM%d", m_nSerialPort);

	if (!m_Com.Open(szCom))
	{
		return;
	}

	if (!m_Com.SetComProperty(&m_SerialPortOpt))
	{
		return;
	}

	if (!m_Com.SetTimeout(1000))
	{
		return;
	}

	m_bLoop = true;

	//开始通信
	m_pReceThread = boost::make_shared<boost::thread>(boost::bind(&CSerialPortService::ReceiveProc,this));

}

void CSerialPortService::StopSerialPortService(void)
{
	m_bLoop = false;

	m_pReceThread->join();

	m_Com.Close();
}

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
				case 0xBA:
					HandleDeviceOnOffMsg();
					break;
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
	static bool bSample = false;
	using namespace std;
	BYTE szDeviceName[100] = { 0 };
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
							if (0x00 == pData[chLength])
							{
								cout << szDeviceName << " is off !" << endl;
							}
							else
							{
								cout << szDeviceName << " is on !" << endl;

								//开始采集
								if (!bSample)
								{ 
									BYTE Bffer[18] = { 0 };
									Bffer[0] = 0xAB;
									Bffer[1] = 0x10;
									Bffer[2] = 0x0D;
									Bffer[3] = 0xB4;
									Bffer[4] = 0xF3;
									Bffer[5] = 0xC6;
									Bffer[6] = 0xF8;
									Bffer[7] = 0xD1;
									Bffer[8] = 0xB9;
									Bffer[9] = 0xC7;
									Bffer[10] = 0xBF;
									Bffer[11] = 0x5B;
									Bffer[12] = 0x4B;
									Bffer[13] = 0x70;
									Bffer[14] = 0x61;
									Bffer[15] = 0x5D;
									Bffer[16] = 0x00;
									Bffer[17] = 0x83;
									
									unsigned int nWriteLength = 0;
									m_Com.Write(Bffer, sizeof(Bffer), nWriteLength);

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

void CSerialPortService::HandleDeviceDataMsg(void)
{


	using namespace std;
	BYTE szDeviceName[100] = { 0 };
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
							

							//获取数据
							float fValue = 0.0f;

							memcpy(&fValue, pData + 3 + nDeviceNameLength, 4);

							if (fValue > 0)
							{
								cout << szDeviceName << " value is: " << fValue << endl;
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


