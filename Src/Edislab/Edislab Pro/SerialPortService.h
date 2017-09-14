#pragma once
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <string>
typedef struct _comproperty
{
	//串口编号
	std::string strSerialPort;
	//波特率
	unsigned int nBaudRate;
	//通信字节位数
	int nDataBits;
	//(0:1个停止位 1:1.5个停止位(Linux暂不支持1.5个停止位) 2:2个停止位)
	int nStopBits;
	//(0:不使用校验  1:奇数校验 2:偶数校验 3:标记校验（Linux下没有此项） 4:空格校验)
	int nPairty;
	//(0:不使用流控 1:使用硬件流控)
	bool bFlowControl;
	_comproperty()
	{
		strSerialPort = "";
		nBaudRate = 9600;
		nDataBits = 8;
		nStopBits = 0;
		nPairty = 0;
		bFlowControl = false;
	}
}COMPROPERTY, *LPCOMPROPERTY;

class CSerialPortService
{

public:
	static CSerialPortService& CreateInstance();
private:
	CSerialPortService();
	~CSerialPortService();

public:
	void SetSerialPortOption(const COMPROPERTY& Options)
	{
		m_SerialOption = Options;
	}

	//开启服务
	void StartSerialPortService(void);

	//停止服务
	void StopSerialPortService(void);

	//开始采集命令
	void StartSensorCollect(const std::string& strSensorName);

	void StartSensorCollect(int nSensorTypeID,int nSensorSeqID);

	//停止采集
	void StopSensorCollect(const std::string& strSensorName);

	void StopSensorCollect(int nSensorTypeID,int nSensorSeqID);

	//设置上报周期
	void SetSensorFrequence(const std::string& strSensorName,int nMillSecond);

	void SetSensorFrequence(int nSensorTypeID,int nSensorSeqID,int nMillSecond);
private:
	//接收完成通知
	void ReadHandler(const boost::system::error_code& ec, std::size_t bytes_transferred);

	//判断是否有整包数据
	int HandlerData(BYTE* pData,int nDataLength);

	//发送完成通知
	void WriteHandler(BYTE* pData, int nDataLength, const boost::system::error_code& ec, std::size_t bytes_transferred);

	//设置传感器开始采集
	void AsyncWriteData(BYTE* pData, int nDataLength);
private:
	//初始化串口
	bool InitSerialPort(void);
	//接收缓冲区
	BYTE* m_pRecvBuffer;
	//已经占用的字节
	int m_nUseBufferBytes;
	//串口通信配置项
	COMPROPERTY m_SerialOption;
	//IoService
	boost::asio::io_service m_IoService;
	boost::asio::io_service::work m_Work;
	boost::asio::serial_port m_SerialPort;
	//单实例对象
	static CSerialPortService s_obj;

	//接收线程
	boost::shared_ptr<boost::thread> m_pRecvThread;
};

