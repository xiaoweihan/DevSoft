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
	//创建单实例
	static CSensorConfig& CreateInstance(void);
private:
	CSensorConfig(void);
	~CSensorConfig(void);
public:
	//加载配置信息
	bool LoadSensorConfig(void);

	//获取COM配置
	const SENSOR_COM_CONFIG_ELEMENT& GetComConfig()
	{
		return m_SensorComConfig;
	}

	//获取所有传感器列表
	void GetSensorList(std::vector<SENSOR_CONFIG_ELEMENT> &vecSensorArry, SENSOR_TYPE enumType);

	// 获取传感器分类信息
	const std::vector<SENSOR_TYPE_INFO>& GetSensorTypeInfo()
	{
		return m_SensorTypeInfo;
	}

	//根据传感器ID返回传感器信息
	SENSOR_CONFIG_ELEMENT GetSensorInfo(int nSensorID);
	
	//获取传感器采样配置信息
	const SENSOR_RECORD_INFO& GetSensorRecordInfo();

	//设置传感器采样配置信息
	void SetSensorRecordInfo(SENSOR_RECORD_INFO& _recordInfo);
private:
	//加载传感器列表信息
	bool LoadSensorList(rapidjson::Document& Parser);
	//加载传感器通信配置信息
	bool LoadSensorComList(rapidjson::Document& Parser);
	//锁
	boost::mutex m_RWLock;
	//保存传感器的配置信息
	std::vector<SENSOR_CONFIG_ELEMENT> m_SensorConfigArray;
	//传感器COM通信配置
	SENSOR_COM_CONFIG_ELEMENT m_SensorComConfig;
	// 保存传感器采集设置信息
	SENSOR_RECORD_INFO m_SensorRecordInfo;
	//
	std::vector<SENSOR_TYPE_INFO> m_SensorTypeInfo;
private:
	static CSensorConfig s_SensorConfig;
};

