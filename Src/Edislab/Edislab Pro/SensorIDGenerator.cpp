#include "StdAfx.h"
#include "SensorIDGenerator.h"
#include <boost/foreach.hpp>

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

int CSensorIDGenerator::QuerySensorTypeIDByName( const std::string& strName )
{
	bool bResult = false;
	int nID = -1;
	BOOST_FOREACH(auto& Element,m_SensorTypeArray)
	{
		if (Element.strSensorName == strName)
		{
			bResult = true;
			nID = Element.nSensorID;
			break;
		}
	}


	if (bResult)
	{
		return nID;
	}

	SENSOR_TYPE_INFO_ELEMENT TempElement;
	TempElement.strSensorName = strName;
	TempElement.nSensorID = s_nTypeIndex++;
	m_SensorTypeArray.push_back(TempElement);

	return TempElement.nSensorID;
}

int CSensorIDGenerator::s_nTypeIndex = 0;

CSensorIDGenerator CSensorIDGenerator::s_Instance;
