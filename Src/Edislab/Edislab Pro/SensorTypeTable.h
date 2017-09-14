/*********************************************************
Copyright(C):
FileName:SensorTypeTable.h
Descripton:预置传感器类型表
Author:xiaowei.han
Data:2017/09/11
Others:
History:
Version:1.0
*********************************************************/
#pragma once
#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>
#include <string>
class CSensorTypeTable
{

public:
	static CSensorTypeTable& CreateInstance(void);
private:
	CSensorTypeTable(void);
	~CSensorTypeTable(void);

public:
	std::string QuerySensorNameByID(int nSensorTypeID);

private:
	void InitSensorTypeTable(void);
private:
	boost::mutex m_Lock;

	boost::unordered_map<int,std::string> m_SensorTypeMap;

	static CSensorTypeTable s_obj;
};

