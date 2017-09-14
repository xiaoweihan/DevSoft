/**********************************************************
*版权所有:xiaowei.han
*文件名称:SensorDataManager.h
*功能描述:传感器数据管理类
*作者:xiaowei.han
*日期:2017/05/14
**********************************************************/
#pragma once
#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>
#include "Type.h"
class CSensorData;
class CSensorDataManager
{
public:
	static CSensorDataManager& CreateInstance();
#ifndef NEW_VERSION
	//增加一个传感器数据
	void AddSensorData(int nSensorID);

	//删除一个传感器数据
	void DelSensorData(int nSensorID);

	//根据传感器ID获取传感器数据
	CSensorData* GetSensorDataBySensorID(int nSensorID);
#else
	//增加一个传感器数据
	void AddSensorData(const SENSOR_TYPE_KEY& KeyElement);

	//删除一个传感器数据
	void DelSensorData(const SENSOR_TYPE_KEY& KeyElement);

	//根据传感器ID获取传感器数据
	CSensorData* GetSensorDataBySensorID(const SENSOR_TYPE_KEY& KeyElement);
#endif


private:
	CSensorDataManager(void);
	~CSensorDataManager(void);
private:
#ifndef NEW_VERSION
	//传感器数据集合主键为传感器编号
	boost::unordered_map<int,CSensorData*> m_SensorDataMap;
#else
	boost::unordered_map<SENSOR_TYPE_KEY,CSensorData*> m_SensorDataMap;
#endif
	//锁
	boost::mutex m_SensorDataMapLock;
private:
	static CSensorDataManager s_obj;
};

