#include "StdAfx.h"
#include "SensorIDGenerator.h"
#include <boost/foreach.hpp>
#include <boost/thread/lock_guard.hpp>
CSensorIDGenerator::CSensorIDGenerator(void)
{
}


CSensorIDGenerator::~CSensorIDGenerator(void)
{
}

CSensorIDGenerator& CSensorIDGenerator::CreateInstance( void )
{
	return s_Instance;
}

int CSensorIDGenerator::QuerySensorTypeIDByName( const std::string& strSensorName )
{
	
	if (strSensorName.empty())
	{
		return -1;
	}
	int nSensorID = -1;
	using namespace boost;
	lock_guard<mutex> Lock(m_Lock);
	BOOST_FOREACH(auto& Element,m_SensorTypeArray)
	{
		//如果存在则返回ID
		if (Element.strSensorName == strSensorName)
		{
			nSensorID = Element.nSensorID;
			break;
		}
	}

	return nSensorID;
}

int CSensorIDGenerator::AddSensor( const std::string& strSensorName )
{
	if (strSensorName.empty())
	{
		return -1;
	}

	using namespace boost;
	lock_guard<mutex> Lock(m_Lock);

	BOOST_FOREACH(auto& Element,m_SensorTypeArray)
	{
		//如果存在则返回ID
		if (Element.strSensorName == strSensorName)
		{
			return Element.nSensorID;
		}
	}
	//如果不存在
	SENSOR_TYPE_INFO_ELEMENT TempElement;
	TempElement.strSensorName = strSensorName;
	TempElement.nSensorID = s_nTypeIndex++;
	m_SensorTypeArray.push_back(TempElement);
	return TempElement.nSensorID;
}

void CSensorIDGenerator::DelSensor( const std::string& strSensorName )
{
	if (strSensorName.empty())
	{
		return;
	}
	using namespace boost;
	lock_guard<mutex> Lock(m_Lock);

	auto Iter = m_SensorTypeArray.begin();

	for (; Iter != m_SensorTypeArray.end(); ++Iter)
	{
		if (Iter->strSensorName == strSensorName)
		{
			m_SensorTypeArray.erase(Iter);
			break;
		}
	}

}

void CSensorIDGenerator::GetAllSensorName(std::vector<std::string>& SensorNameArray)
{
	SensorNameArray.clear();
	using namespace boost;
	lock_guard<mutex> Lock(m_Lock);

	BOOST_FOREACH(auto& Element,m_SensorTypeArray)
	{
		SensorNameArray.push_back(Element.strSensorName);
	}
}

int CSensorIDGenerator::s_nTypeIndex = 0;

CSensorIDGenerator CSensorIDGenerator::s_Instance;
