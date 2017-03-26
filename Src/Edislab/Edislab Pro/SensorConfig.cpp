/************************************************************************************
Copyright(C):selp
FileName:SensorConfig.cpp
Description:传感器配置类，所有传感器配置从该类读取
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
************************************************************************************/
#include "StdAfx.h"
#include "SensorConfig.h"
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr.hpp>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/document.h>
#include <fstream>
#include "Log.h"
#include "Utility.h"

//定制的删除器
static void DeleteFunction(std::ifstream* pReader)
{
	if (nullptr != pReader)
	{
		if (pReader->is_open())
		{
			pReader->close();
		}

		delete pReader;
		pReader = nullptr;
	}
}

CSensorConfig::CSensorConfig(void)
{
	m_SensorConfigArray.clear();
}


CSensorConfig::~CSensorConfig(void)
{
	m_SensorConfigArray.clear();
}

/*****************************************************************************
* @FunctionName : CreateInstance
* @FunctionDestription : 创建单实例
* @author : xiaowei.han
* @date : 2017/3/25 10:26
* @version : ver 1.0
* @inparam :
* @outparam: 
* @return : 
*****************************************************************************/
CSensorConfig& CSensorConfig::CreateInstance( void )
{
	return s_SensorConfig;
}

/*****************************************************************************
* @FunctionName : LoadSensorConfig
* @FunctionDestription : 加载传感器配置信息
* @author : xiaowei.han
* @date : 2017/3/25 10:26
* @version : ver 1.0
* @inparam :
* @outparam: 
* @return : 
*****************************************************************************/
bool CSensorConfig::LoadSensorConfig( void )
{

	std::string strConfigPath = Utility::GetExeDirecory() + std::string("\\SensorConfig.json");

	//判断文件是否存在
	boost::filesystem::path ConfigPath(strConfigPath);

	try
	{
		//判断配置文件是否存在
		if (!boost::filesystem::exists(ConfigPath))
		{
			ERROR_LOG("the [%s] is not exist!",strConfigPath.c_str());
			return false;
		}
		//开始进行json文件解析
		boost::shared_ptr<std::ifstream> pConfigReader(new std::ifstream(strConfigPath), DeleteFunction);

		if (!pConfigReader)
		{
			ERROR_LOG("open [%s] file failed.", strConfigPath.c_str());
			return false;
		}

		rapidjson::IStreamWrapper JsonReader(*pConfigReader);
		rapidjson::Document Parser;
		Parser.ParseStream(JsonReader);

		if (Parser.HasMember("SensorConfig"))
		{
			const rapidjson::Value& ProcessArray = Parser["SensorConfig"];

			if (ProcessArray.IsArray())
			{
				for (auto Iter = ProcessArray.Begin(); Iter != ProcessArray.End(); ++Iter)
				{
					SENSOR_CONFIG_ELEMENT Element;
					if (Iter->IsObject())
					{
						if (!Iter->HasMember("SensorName") || !(*Iter)["SensorName"].IsString())
						{
							continue;
						}

						Element.strSensorName = Utility::ConverUTF8ToGB2312((*Iter)["SensorName"].GetString());

						if (!Iter->HasMember("PortName") || !(*Iter)["PortName"].IsInt())
						{
							continue;
						}
						Element.nComIndex = (*Iter)["PortName"].GetInt();

						if (!Iter->HasMember("Pairty") || !(*Iter)["Pairty"].IsInt())
						{
							continue;
						}
						Element.nPairty = (*Iter)["Pairty"].GetInt();

						if (!Iter->HasMember("StopBits") || !(*Iter)["StopBits"].IsInt())
						{
							continue;
						}
						Element.nStopBits = (*Iter)["StopBits"].GetInt();

						if (!Iter->HasMember("DataBits") || !(*Iter)["DataBits"].IsInt())
						{
							continue;
						}
						Element.nDataBits = (*Iter)["DataBits"].GetInt();

						if (!Iter->HasMember("BaudRate") || !(*Iter)["BaudRate"].IsInt())
						{
							continue;
						}
						Element.nBaudRate = (*Iter)["BaudRate"].GetInt();

						if (!Iter->HasMember("UseFlowControl") || !(*Iter)["UseFlowControl"].IsBool())
						{
							continue;
						}
						Element.bUseFlowControl = (*Iter)["UseFlowControl"].GetBool();

						m_SensorConfigArray.push_back(Element);
					}
				}
			}
		}

	}
	catch (boost::filesystem::filesystem_error& e)
	{
		ERROR_LOG("oh,shit,this is something wrong in filesystem,the error is [%s].",e.what());

		return false;
	}

	return true;
}

/*****************************************************************************
* @FunctionName : GetSensorConfigBySensorName
* @FunctionDestription : 根据传感器名称获取传感器配置信息
* @author : xiaowei.han
* @date : 2017/3/25 10:26
* @version : ver 1.0
* @inparam :
* @outparam: 
* @return : 
*****************************************************************************/
bool CSensorConfig::GetSensorConfigBySensorName( const std::string& strSensorName,LP_SENSOR_CONFIG_ELEMENT pConfig )
{

	bool bResult = false;
	if (nullptr == pConfig || strSensorName.empty())
	{
		return bResult;
	}

	BOOST_FOREACH(auto& Element,m_SensorConfigArray)
	{
		if (Element.strSensorName == strSensorName)
		{
			*pConfig = Element;
			bResult = true;
			break;
		}
	}

	return bResult;
}

#ifdef COM_TEST
bool CSensorConfig::GetFirstSensorConfig( LP_SENSOR_CONFIG_ELEMENT pConfig )
{
	if (nullptr == pConfig)
	{
		return false;
	}

	if (m_SensorConfigArray.empty())
	{
		return false;
	}

	*pConfig = m_SensorConfigArray[0];
	return true;
}
#endif

CSensorConfig CSensorConfig::s_SensorConfig;
