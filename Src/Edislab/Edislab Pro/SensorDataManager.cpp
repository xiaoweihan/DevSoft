#include "StdAfx.h"
#include "SensorDataManager.h"
#include <boost/foreach.hpp>
#include <boost/thread/lock_guard.hpp>
#include "SensorData.h"

CSensorDataManager& CSensorDataManager::CreateInstance()
{
	return s_obj;
}

#ifndef NEW_VERSION
void CSensorDataManager::AddSensorData(int nSensorID)
{
	using namespace boost;
	lock_guard<mutex> Lock(m_SensorDataMapLock);

    auto Iter = m_SensorDataMap.find(nSensorID);
	//不存在才添加
	if (Iter == m_SensorDataMap.end())
	{
		CSensorData* pData = new CSensorData;
		if (nullptr != pData)
		{
			//设置传感器的ID
			pData->SetSensorID(nSensorID);
			m_SensorDataMap.emplace(nSensorID,pData);
		}
	}
}

void CSensorDataManager::DelSensorData(int nSensorID)
{
	using namespace boost;
	lock_guard<mutex> Lock(m_SensorDataMapLock);

	auto Iter = m_SensorDataMap.find(nSensorID);
	//不存在才添加
	if (Iter == m_SensorDataMap.end())
	{
		return;
	}

	if (nullptr != Iter->second)
	{
		delete Iter->second;
		Iter->second = nullptr;
	}

	m_SensorDataMap.erase(Iter);
}

CSensorData* CSensorDataManager::GetSensorDataBySensorID( int nSensorID )
{
	using namespace boost;
	lock_guard<mutex> Lock(m_SensorDataMapLock);

	auto Iter = m_SensorDataMap.find(nSensorID);
	//不存在才添加
	if (Iter == m_SensorDataMap.end())
	{
		return nullptr;
	}

	return Iter->second;
}
#else

void CSensorDataManager::AddSensorData(const SENSOR_TYPE_KEY& KeyElement)
{
	using namespace boost;
	lock_guard<mutex> Lock(m_SensorDataMapLock);

	auto Iter = m_SensorDataMap.find(KeyElement);
	//不存在才添加
	if (Iter == m_SensorDataMap.end())
	{
		CSensorData* pData = new CSensorData;
		if (nullptr != pData)
		{
			//设置传感器的ID
			pData->SetSensorID(KeyElement.nSensorID,KeyElement.nSensorSerialID);
			m_SensorDataMap[KeyElement] = pData;
		}
	}
}


void CSensorDataManager::DelSensorData(const SENSOR_TYPE_KEY& KeyElement)
{
	using namespace boost;
	lock_guard<mutex> Lock(m_SensorDataMapLock);

	auto Iter = m_SensorDataMap.find(KeyElement);
	//不存在才添加
	if (Iter == m_SensorDataMap.end())
	{
		return;
	}

	if (nullptr != Iter->second)
	{
		delete Iter->second;
		Iter->second = nullptr;
	}

	m_SensorDataMap.erase(Iter);
}


CSensorData* CSensorDataManager::GetSensorDataBySensorID(const SENSOR_TYPE_KEY& KeyElement)
{
	using namespace boost;
	lock_guard<mutex> Lock(m_SensorDataMapLock);

	auto Iter = m_SensorDataMap.find(KeyElement);
	//不存在才添加
	if (Iter == m_SensorDataMap.end())
	{
		return nullptr;
	}

	return Iter->second;
}


#endif


CSensorDataManager::CSensorDataManager(void)
{
	m_SensorDataMap.clear();
}

CSensorDataManager::~CSensorDataManager(void)
{
	BOOST_FOREACH(auto& Element,m_SensorDataMap)
	{
		if (nullptr != Element.second)
		{
			delete Element.second;
			Element.second = nullptr;
		}
	}

	m_SensorDataMap.clear();
}

CSensorDataManager CSensorDataManager::s_obj;
