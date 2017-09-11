/*********************************************************
Copyright(C):
FileName:SensorManager.cpp
Descripton:全局传感器管理类
Author:xiaowei.han
Data:2017/09/11
Others:
History:
Version:1.0
*********************************************************/
#include "StdAfx.h"
#include "SensorManager.h"
#include <boost/thread/lock_factories.hpp>
#include <algorithm>
#include <string>

CSensorManager::CSensorManager(void)
{
	m_SensorInfoArray.clear();
}


CSensorManager::~CSensorManager(void)
{
	m_SensorInfoArray.clear();
}

/*********************************************************
FunctionName:RegisterSensor
FunctionDesc:注册传感器
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
bool CSensorManager::RegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement)
{
	auto Lock = boost::make_unique_lock(m_Lock);
	auto Iter = std::find(m_SensorInfoArray.begin(),m_SensorInfoArray.end(),SensorElement);
	//如果找到
	if (Iter != m_SensorInfoArray.end())
	{
		return false;
	}
	m_SensorInfoArray.push_back(SensorElement);
	return true;
}

/*********************************************************
FunctionName:RegisterSensor
FunctionDesc:注册传感器
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
bool CSensorManager::RegisterSensor(int nSensorTypeID,int nSensorSerialID)
{
	using namespace std;
	//根据传感器类型的ID查询传感器的名称
	string strSensorName;
	return RegisterSensor(SENSOR_TYPE_INFO_ELEMENT(strSensorName,nSensorTypeID,nSensorSerialID));
}

/*********************************************************
FunctionName:RegisterSensor
FunctionDesc:注册传感器
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
bool CSensorManager::UnRegisterSensor(const SENSOR_TYPE_INFO_ELEMENT& SensorElement)
{
	auto Lock = boost::make_unique_lock(m_Lock);
	auto Iter = std::find(m_SensorInfoArray.begin(),m_SensorInfoArray.end(),SensorElement);

	//找到对应的元素
	if (Iter != m_SensorInfoArray.end())
	{
		m_SensorInfoArray.erase(Iter);
	}
	
	return true;
}

/*********************************************************
FunctionName:RegisterSensor
FunctionDesc:注册传感器
InputParam:
OutputParam:
ResultValue:
Author:xiaowei.han
*********************************************************/
bool CSensorManager::UnRegisterSensor(int nSensorTypeID,int nSensorSerialID)
{
	return UnRegisterSensor(SENSOR_TYPE_INFO_ELEMENT("",nSensorTypeID,nSensorSerialID));
}

CSensorManager& CSensorManager::CreateInstance(void)
{
	return s_obj;
}

CSensorManager CSensorManager::s_obj;
