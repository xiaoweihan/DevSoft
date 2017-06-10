#include "StdAfx.h"
#include "SensorIDGenerator.h"
#include <boost/foreach.hpp>
#include <boost/thread/lock_guard.hpp>
#include <algorithm>
#include <string>
//时间轴的ID
const int DEFAULT_TIME_AXIS_ID = 1000;
const std::string DEFAULT_TIME_NAME = "t(s)时间";
CSensorIDGenerator::CSensorIDGenerator(void)
{
	SENSOR_TYPE_INFO_ELEMENT TempElement;
	TempElement.strSensorName = DEFAULT_TIME_NAME;
	TempElement.nSensorID = DEFAULT_TIME_AXIS_ID;
	m_SensorTypeArray.push_back(TempElement);
}

CSensorIDGenerator::~CSensorIDGenerator(void)
{
}

CSensorIDGenerator& CSensorIDGenerator::CreateInstance( void )
{
	return s_Instance;
}

/*******************************************************************
*函数名称:GetSpecialSensorID
*功能描述:获取特殊设备的ID
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/10 11:26:04
*******************************************************************/
int CSensorIDGenerator::GetSpecialSensorID(void)
{
	return DEFAULT_TIME_AXIS_ID;
}

/*******************************************************************
*函数名称:IsSpecialSensorID
*功能描述:是否是特殊设备的ID
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/10 11:26:07
*******************************************************************/
bool CSensorIDGenerator::IsSpecialSensorID(int nSensorID)
{
	if (nSensorID == DEFAULT_TIME_AXIS_ID)
	{
		return true;
	}

	return false;
}

/*******************************************************************
*函数名称:QuerySensorTypeIDByName
*功能描述:根据传感器名称获取对应的ID
*输入参数:
*输出参数:
*返回值:传感器ID
*作者:xiaowei.han
*日期:2017/06/10 8:45:49
*******************************************************************/
int CSensorIDGenerator::QuerySensorTypeIDByName( const std::string& strSensorName )
{
	using namespace boost;
	if (strSensorName.empty())
	{
		return -1;
	}
	auto FindPred = [&strSensorName](const SENSOR_TYPE_INFO_ELEMENT& Element)->bool
	{
		if (Element.strSensorName == strSensorName)
		{
			return true;
		}
		return false;
	};
	int nSensorID = -1;
	lock_guard<mutex> Lock(m_Lock);
	auto FindIter = std::find_if(m_SensorTypeArray.begin(),m_SensorTypeArray.end(),FindPred);
	if (FindIter != m_SensorTypeArray.end())
	{
		nSensorID = FindIter->nSensorID;
	}
	return nSensorID;
}

/*******************************************************************
*函数名称:QueryrNameBySensorID
*功能描述:根据传感器ID获取传感器名称
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/10 8:46:21
*******************************************************************/
std::string CSensorIDGenerator::QueryrNameBySensorID(int nID)
{
	using namespace boost;
	std::string strSensorName;

	auto FindPred = [&nID](const SENSOR_TYPE_INFO_ELEMENT& Element)->bool
	{
		if (Element.nSensorID == nID)
		{
			return true;
		}
		return false;
	};

	lock_guard<mutex> Lock(m_Lock);
	auto FindIter = std::find_if(m_SensorTypeArray.begin(),m_SensorTypeArray.end(),FindPred);
	//如果找到
	if (FindIter != m_SensorTypeArray.end())
	{
		strSensorName = FindIter->strSensorName;
	}
	return strSensorName;
}

/*******************************************************************
*函数名称:AddSensor
*功能描述:添加新的传感器
*输入参数:
*输出参数:
*返回值:新的传感器对应的ID
*作者:xiaowei.han
*日期:2017/06/10 8:47:25
*******************************************************************/
int CSensorIDGenerator::AddSensor( const std::string& strSensorName)
{
	using namespace boost;
	if (strSensorName.empty())
	{
		return -1;
	}
	auto FindPred = [&strSensorName](const SENSOR_TYPE_INFO_ELEMENT& Element)->bool
	{
		if (Element.strSensorName == strSensorName)
		{
			return true;
		}
		return false;
	};
	lock_guard<mutex> Lock(m_Lock);
	auto Iter = std::find_if(m_SensorTypeArray.begin(),m_SensorTypeArray.end(),FindPred);
	if (Iter != m_SensorTypeArray.end())
	{
		return Iter->nSensorID;
	}
	//如果不存在
	SENSOR_TYPE_INFO_ELEMENT TempElement;
	TempElement.strSensorName = strSensorName;
	TempElement.nSensorID = s_nTypeIndex++;
	m_SensorTypeArray.push_back(TempElement);
	return TempElement.nSensorID;
}

/*******************************************************************
*函数名称:DelSensor
*功能描述:删除传感器
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/10 8:47:49
*******************************************************************/
void CSensorIDGenerator::DelSensor( const std::string& strSensorName )
{
	using namespace boost;
	if (strSensorName.empty())
	{
		return;
	}

	auto FindPred = [&strSensorName](const SENSOR_TYPE_INFO_ELEMENT& Element)->bool
	{
		if (Element.strSensorName == strSensorName)
		{
			return true;
		}
		return false;
	};
	
	lock_guard<mutex> Lock(m_Lock);

	auto Iter = std::find_if(m_SensorTypeArray.begin(),m_SensorTypeArray.end(),FindPred);

	if (Iter != m_SensorTypeArray.end())
	{
		m_SensorTypeArray.erase(Iter);
	}

}

/*******************************************************************
*函数名称:GetAllSensorName
*功能描述:获取所有传感器的名称
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/10 8:48:12
*******************************************************************/
void CSensorIDGenerator::GetAllSensorName(std::vector<std::string>& SensorNameArray,bool bInclude /*= false*/)
{
	using namespace boost;
	SensorNameArray.clear();
	lock_guard<mutex> Lock(m_Lock);

	if (bInclude)
	{
		BOOST_FOREACH(auto& Element,m_SensorTypeArray)
		{
			SensorNameArray.push_back(Element.strSensorName);
		}
	}
	else
	{
		BOOST_FOREACH(auto& Element,m_SensorTypeArray)
		{
			if (Element.nSensorID != DEFAULT_TIME_AXIS_ID)
			{
				SensorNameArray.push_back(Element.strSensorName);
			}		
		}
	}


}

/*******************************************************************
*函数名称:IsSensorExist
*功能描述:判断传感器是否存在
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/10 8:48:39
*******************************************************************/
bool CSensorIDGenerator::IsSensorExist(const std::string& strSensorName)
{
	using namespace boost;
	bool bResult = false;
	auto FindPred = [&strSensorName](const SENSOR_TYPE_INFO_ELEMENT& Element)->bool
	{
		if (Element.strSensorName == strSensorName)
		{
			return true;
		}
		return false;
	};

	lock_guard<mutex> Lock(m_Lock);
	auto FindIter = std::find_if(m_SensorTypeArray.begin(),m_SensorTypeArray.end(),FindPred);
	//如果找到
	if (FindIter != m_SensorTypeArray.end())
	{
		bResult = true;
	}
	return bResult;
}

int CSensorIDGenerator::s_nTypeIndex = 1;

CSensorIDGenerator CSensorIDGenerator::s_Instance;
