/*********************************************************
Copyright(C):
FileName:SensorManager.h
Descripton:全局传感器管理类
Author:xiaowei.han
Data:2017/09/11
Others:
History:
Version:1.0
*********************************************************/
#pragma once
#include <vector>
#include <boost/thread/mutex.hpp>
#include "Type.h"
class CSensorManager
{

public:
	static CSensorManager& CreateInstance(void);
private:
	CSensorManager(void);
	virtual ~CSensorManager(void);

	//开始注册一个传感器
	bool RegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement);
	//重载
	bool RegisterSensor(int nSensorTypeID,int nSensorSerialID);
	//反注册一个传感器
	bool UnRegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement);
	//重载
	bool UnRegisterSensor(int nSensorTypeID,int nSensorSerialID);

private:
	//传感器信息列表
	std::vector<SENSOR_TYPE_INFO_ELEMENT> m_SensorInfoArray;
	//锁
	boost::mutex m_Lock;
	//全局唯一实例
	static CSensorManager s_obj;
};

