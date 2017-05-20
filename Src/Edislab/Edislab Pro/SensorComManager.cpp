#include "stdafx.h"
#include "SensorComManager.h"
#include <boost/foreach.hpp>
#include <boost/thread/lock_guard.hpp>
#include "Utility.h"
#include "SerialPortService.h"
CSensorComManager& CSensorComManager::CreateInstance( void )
{
	return s_obj;
}

void CSensorComManager::AddSensorCom( int nSensorID )
{
	using namespace boost;
	lock_guard<mutex> Lock(m_Lock);

	auto Iter = m_SensorComMap.find(nSensorID);

	//不存在才添加
	if (Iter == m_SensorComMap.end())
	{
		CSerialPortService* pCom = new CSerialPortService;
		if (nullptr != pCom)
		{
			m_SensorComMap.emplace(nSensorID,pCom);
		}
	}
}

void CSensorComManager::DelSensorCom( int nSensorID )
{
	using namespace boost;
	lock_guard<mutex> Lock(m_Lock);

	auto Iter = m_SensorComMap.find(nSensorID);

	//找到删除
	if (Iter != m_SensorComMap.end())
	{
		if (Iter->second != nullptr)
		{
			Iter->second->StopSerialPortService();
			DELETE_POINTER(Iter->second);
		}

		m_SensorComMap.erase(Iter);
	}
}

CSerialPortService* CSensorComManager::QueryComBySensorID( int nSensorID )
{
	using namespace boost;
	lock_guard<mutex> Lock(m_Lock);

	auto Iter = m_SensorComMap.find(nSensorID);

	if (Iter == m_SensorComMap.end())
	{
		return nullptr;
	}

	return Iter->second;
}

void CSensorComManager::StartSensorCollect()
{
	using namespace boost;
	lock_guard<mutex> Lock(m_Lock);
	BOOST_FOREACH(auto& V,m_SensorComMap)
	{
		if (nullptr != V.second)
		{
			V.second->StartSensorCollect();
		}
	}
}

void CSensorComManager::StopSensorCollect()
{
	using namespace boost;
	lock_guard<mutex> Lock(m_Lock);
	BOOST_FOREACH(auto& V,m_SensorComMap)
	{
		if (nullptr != V.second)
		{
			V.second->StopSensorCollect();
		}
	}
}

CSensorComManager::CSensorComManager( void )
{
	m_SensorComMap.clear();
}

CSensorComManager::~CSensorComManager( void )
{
	BOOST_FOREACH(auto& V,m_SensorComMap)
	{
		if (V.second != nullptr)
		{
			V.second->StopSerialPortService();
		}
		DELETE_POINTER(V.second);
	}

	m_SensorComMap.clear();
}

CSensorComManager CSensorComManager::s_obj;
