/************************************************************************************
Copyright(C):self
FileName:SensorConfig.h
Description:传感器配置类，所有传感器配置从该类读取
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
************************************************************************************/
#pragma once
#include <vector>
#include <boost/thread/mutex.hpp>
#include <string>
#include "Type.h"
#define COM_TEST (1)
class CSensorConfig
{

public:
	static CSensorConfig& CreateInstance(void);

private:
	CSensorConfig(void);
	~CSensorConfig(void);

public:
	//加载配置信息
	bool LoadSensorConfig(void);

	//根据传感器名称获取传感器COM的配置
	bool GetSensorConfigBySensorName(const std::string& strSensorName,LP_SENSOR_CONFIG_ELEMENT pConfig);

#ifdef COM_TEST
	bool GetFirstSensorConfig(LP_SENSOR_CONFIG_ELEMENT pConfig);
#endif
private:
	//锁
	boost::mutex m_RWLock;
	//保存传感器的配置信息
	std::vector<SENSOR_CONFIG_ELEMENT> m_SensorConfigArray;
	//保存传感器的COM通信配置
	std::vector<SENSOR_COM_CONFIG_ELEMENT> m_SensorComConfigArray;
private:
	static CSensorConfig s_SensorConfig;
};

