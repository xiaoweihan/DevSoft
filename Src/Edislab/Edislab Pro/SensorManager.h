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
#include <string>
#include "Type.h"
class CSensorManager
{

public:
	static CSensorManager& CreateInstance(void);
private:
	CSensorManager(void);
	~CSensorManager(void);
public:
	//开始注册一个传感器
	bool RegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement);
	//重载
	bool RegisterSensor(int nSensorTypeID,int nSensorSerialID);
	//反注册一个传感器
	bool UnRegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement);
	//重载
	bool UnRegisterSensor(int nSensorTypeID,int nSensorSerialID);
	//根据传感器名称获取传感器ID
	bool QuerySensorIDByName(const std::string& strSensorName,SENSOR_TYPE_KEY& SensorKeyID);
	//获取所有传感器信息
	void GetSensorList(std::vector<SENSOR_TYPE_INFO_ELEMENT>& SensorList,bool bInculde = false);
	//获取特殊传感器的ID
	SENSOR_TYPE_KEY GetSpecialSensorID(void);
	//判断是否是特殊的传感器ID
	bool IsSpecialSensorID(const SENSOR_TYPE_KEY& SensorKeyID);
	//根据传感器ID获取传感器名称
	std::string QuerySensorNameByID(const SENSOR_TYPE_KEY& SensorKeyID);
private:
	//传感器信息列表
	std::vector<SENSOR_TYPE_INFO_ELEMENT> m_SensorInfoArray;
	//锁
	boost::mutex m_Lock;
	//全局唯一实例
	static CSensorManager s_obj;
};

