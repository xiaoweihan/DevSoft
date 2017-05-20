#ifndef SERIAL_PORT_SERVICE_H
#define SERIAL_PORT_SERVICE_H
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/atomic.hpp>
#include <boost/function.hpp>
#include <string>
#include "SerialPort.h"

//设备上线下线回调定义
typedef boost::function<void(const std::string&,bool)> pDeviceCallBack;


class CSerialPortService
{

public:
	static CSerialPortService& CreateInstance();
private:
	CSerialPortService();
	~CSerialPortService();
public:
	void SetSerialPortOption(int nPort, const COMPROPERTY& Options)
	{
		m_nSerialPort = nPort;
		m_SerialPortOpt = Options;
	}
	//开启服务
	void StartSerialPortService(void);
	void StopSerialPortService(void);

	//开始采集命令
	void StartSensorCollect(const std::string& strSensorName);
	//停止采集
	void StopSensorCollect(const std::string& strSensorName);

	//设置上报周期
	void SetSensorFrequence(const std::string& strSensorName,int nMillSecond);

	//注册设备上下线回调
	void RegisterDeviceCallBack(pDeviceCallBack DeviceCallback)
	{
		m_pCallBack = DeviceCallback;
	}
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

	static CSerialPortService s_obj;

	//设备上线下线回调
	pDeviceCallBack m_pCallBack;
};

#endif