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
	//接收缓冲区的大小
	enum {MAX_BUFFER_SIZE = 1024};

private:
	void ReceiveProc(void);

	//处理上线和下线消息
	void HandleDeviceOnOffMsg(void);

	//处理接收数据信息
	void HandleDeviceDataMsg(void);

	//进行CRC校验和
	BYTE CalCRC8(BYTE* pBuf,unsigned int nsize);
private:
	//默认的接收缓冲区大小
	int m_nRecvBufferSize;

	//缓冲区已经接收的数据长度
	int m_nRecvDataLength;

	//接收缓冲区
	BYTE m_RecvBuffer[MAX_BUFFER_SIZE];

	//串口号
	int m_nSerialPort;

	//串口配置
	COMPROPERTY m_SerialPortOpt;

	boost::shared_ptr<boost::thread> m_pReceThread;

	boost::atomic_bool m_bLoop;

	//串口类
	CHandleCom m_Com;
};

#endif