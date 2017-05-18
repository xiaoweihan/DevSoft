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
#include <rapidjson/document.h>
#include "Type.h"
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
private:
	//加载传感器列表信息
	bool LoadSensorList(rapidjson::Document& Parser);
	//加载传感器通信配置信息
	bool LoadSensorComList(rapidjson::Document& Parser);
	//锁
	boost::mutex m_RWLock;
	//保存传感器的配置信息
	std::vector<SENSOR_CONFIG_ELEMENT> m_SensorConfigArray;
	//保存传感器的COM通信配置
	//std::vector<SENSOR_COM_CONFIG_ELEMENT> m_SensorComConfigArray;
	SENSOR_COM_CONFIG_ELEMENT m_SensorComConfig;
private:
	static CSensorConfig s_SensorConfig;
};

