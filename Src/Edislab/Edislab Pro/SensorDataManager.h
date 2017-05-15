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
class CSensorData;
class CSensorDataManager
{
public:
	static CSensorDataManager& CreateInstance();

	//增加一个传感器数据
	void AddSensorData(int nSensorID);

	//删除一个传感器数据
	void DelSensorData(int nSensorID);

	//根据传感器ID获取传感器数据
	CSensorData* GetSensorDataBySensorID(int nSensorID);
private:
	CSensorDataManager(void);
	~CSensorDataManager(void);
private:
	//传感器数据集合主键为传感器编号
	boost::unordered_map<int,CSensorData*> m_SensorDataMap;
	//锁
	boost::mutex m_SensorDataMapLock;
private:
	static CSensorDataManager s_obj;
};

