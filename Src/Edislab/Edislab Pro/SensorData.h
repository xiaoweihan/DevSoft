/**********************************************************
*版权所有:xiaowei.han
*文件名称:SensorData.h
*功能描述:传感器采集数据类
*作者:xiaowei.han
*日期:2017/05/14
**********************************************************/
#pragma once
#include <deque>
#include <vector>
#include <boost/thread/mutex.hpp>
class CSensorData
{
public:
	CSensorData(void);
	~CSensorData(void);

	//添加数据
	void AddSensorData(float fData);

	//设置保存数据的最大长度
	void SetDataMaxLength(int nDtatMaxLength)
	{
		m_nMaxDataLength = nDtatMaxLength;
	}

	//设置传感器ID
	void SetSensorID(int nSensorID)
	{
		m_nSensorID = nSensorID;
	}

	//获取传感器数据
	void GetSensorData(std::vector<float>& SensorDataList);

	//根据索引获取数据
	bool GetSensorData(int nIndex,float& fValue);
private:
	//用于同步的锁
	boost::mutex m_DataLock;
	//数据
	std::deque<float> m_SensorDataList;
	//数据的最大个数
	int m_nMaxDataLength;
	//对应传感器的编号
	int m_nSensorID;
};

