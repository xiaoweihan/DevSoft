#ifndef SERIAL_PORT_SERVICE_H
#define SERIAL_PORT_SERVICE_H
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/atomic.hpp>
#include "SerialPort.h"
class CSerialPortService
{
public:
	CSerialPortService();
	~CSerialPortService();
	void SetSerialPortOption(int nPort, const COMPROPERTY& Options)
	{
		m_nSerialPort = nPort;
		m_SerialPortOpt = Options;
	}
	//开启服务
	void StartSerialPortService(void);
	void StopSerialPortService(void);
private:
	void ReceiveProc(void);

	//处理上线和下线消息
	void HandleDeviceOnOffMsg(void);

	//处理接收数据信息
	void HandleDeviceDataMsg(void);

	//进行CRC校验和
	BYTE CalCRC8(BYTE* pBuf,unsigned int nsize);

	//拷贝数据名称
	void CopyDeviceName(BYTE* pData,int nDataLength);
private:
	//存放设备名称的数据
	BYTE* m_pDeviceNameBuffer;

	//存放设备名称数据的长度
	int m_nDeviceNameBufferLength;

	//串口号
	int m_nSerialPort;

	//串口配置
	COMPROPERTY m_SerialPortOpt;

	boost::shared_ptr<boost::thread> m_pReceThread;

	boost::atomic_bool m_bLoop;

	//串口类
	CHandleCom m_Com;

	//是否拷贝设备名称
	bool m_bCopyDeviceName;
};

#endif