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
#include <fstream>
#include "Log.h"
#include "Utility.h"
#include "SensorIDGenerator.h"
#include "SensorDataManager.h"
#include "SerialPortService.h"
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
	// 添加传感器分类信息
	SENSOR_TYPE_INFO typeInfo;
	typeInfo.enumType = SENSOR_PHYSICS;
	typeInfo.strTypeName = "物理";
	m_SensorTypeInfo.push_back(typeInfo);
	typeInfo.enumType = SENSOR_CHEMISTRY;
	typeInfo.strTypeName = "化学";
	m_SensorTypeInfo.push_back(typeInfo);
	typeInfo.enumType = SENSOR_BIOLOGY;
	typeInfo.strTypeName = "生物";
	m_SensorTypeInfo.push_back(typeInfo);
	typeInfo.enumType = SENSOR_ALL;
	typeInfo.strTypeName = "所有";
	m_SensorTypeInfo.push_back(typeInfo);

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

		//加载传感器列表
		if (!LoadSensorList(Parser))
		{
			return false;
		}
		//加载传感器通信配置
		if (!LoadSensorComList(Parser))
		{
			return false;
		}
	}
	catch (boost::filesystem::filesystem_error& e)
	{
		ERROR_LOG("oh,shit,this is something wrong in filesystem,the error is [%s].",e.what());

		return false;
	}
	return true;
}

/*******************************************************************
*函数名称:LoadSensorList
*功能描述:加载传感器列表
*输入参数:
*输出参数:
*返回值:true:succeed false:failed
*作者:xiaowei.han
*日期:2017/06/10 8:39:02
*******************************************************************/
bool CSensorConfig::LoadSensorList( rapidjson::Document& Parser )
{
	// 解析传感器配置
	if (!Parser.HasMember("SensorConfig"))
	{
		return false;
	}

	const rapidjson::Value& ProcessArray = Parser["SensorConfig"];

	if (ProcessArray.IsArray())
	{
		for (auto Iter = ProcessArray.Begin(); Iter != ProcessArray.End(); ++Iter)
		{
			SENSOR_CONFIG_ELEMENT Element;
			if (Iter->IsObject())
			{
				// 传感器ID
				if (!Iter->HasMember("SensorID") || !(*Iter)["SensorID"].IsInt())
				{
					continue;
				}

				Element.nSensorID = (*Iter)["SensorID"].GetInt();

				// 传感器名
				if (!Iter->HasMember("SensorName") || !(*Iter)["SensorName"].IsString())
				{
					continue;
				}

				Element.strSensorName = Utility::ConverUTF8ToGB2312((*Iter)["SensorName"].GetString());

				// 是否添加到列表
				if (!Iter->HasMember("AddtoSensorlist") || !(*Iter)["AddtoSensorlist"].IsBool())
				{
					continue;
				}

				Element.bAddtoSensorlist = (*Iter)["AddtoSensorlist"].GetBool();

				// 类型名称
				if (!Iter->HasMember("TypeName") || !(*Iter)["TypeName"].IsString())
				{
					continue;
				}

				Element.strSensorTypeName =  Utility::ConverUTF8ToGB2312((*Iter)["TypeName"].GetString());

				// 是否多采样
				if (!Iter->HasMember("MultiSample") || !(*Iter)["MultiSample"].IsBool())
				{
					continue;
				}

				Element.bMultiSample =  (*Iter)["MultiSample"].GetBool();

				// 传感器型号
				if (!Iter->HasMember("SensorModelName") || !(*Iter)["SensorModelName"].IsString())
				{
					continue;
				}

				Element.strSensorModelName =  Utility::ConverUTF8ToGB2312((*Iter)["SensorModelName"].GetString());

				// 传感器符号
				if (!Iter->HasMember("Symbol") || !(*Iter)["Symbol"].IsString())
				{
					continue;
				}

				Element.strSensorSymbol =  Utility::ConverUTF8ToGB2312((*Iter)["Symbol"].GetString());

				// 传感器描述
				if (!Iter->HasMember("Description") || !(*Iter)["Description"].IsString())
				{
					continue;
				}

				Element.strSensorDescription =  Utility::ConverUTF8ToGB2312((*Iter)["Description"].GetString());

				// 传感器类型（物理\化学\生物或者全部？）
				if (!Iter->HasMember("SensorCategory") || !(*Iter)["SensorCategory"].IsInt())
				{
					continue;
				}

				Element.nSensorCategory =  (*Iter)["SensorCategory"].GetInt();

				// 量程信息
				if(!Iter->HasMember("RangeInfo") || !(*Iter)["RangeInfo"].IsArray())
				{
					continue;
				}

				for(auto rangeIter = (*Iter)["RangeInfo"].Begin(); rangeIter != (*Iter)["RangeInfo"].End() ; ++rangeIter)
				{
					SENSOR_RANGE_INFO_ELEMENT RangeInfo;
					// 量程名
					if (!rangeIter->HasMember("Name") || !(*rangeIter)["Name"].IsString())
					{
						continue;
					}

					std::string strRangeName = (*rangeIter)["Name"].GetString();
					RangeInfo.strRangeName =  Utility::ConverUTF8ToASCII(strRangeName);

					// 校准值
					if (!rangeIter->HasMember("Calibrationvalue") || !(*rangeIter)["Calibrationvalue"].IsString())
					{
						continue;
					}

					RangeInfo.strCalibrationvalue =  (*rangeIter)["Calibrationvalue"].IsString();

					// 默认值
					if (!rangeIter->HasMember("Defaultvalue") || !(*rangeIter)["Defaultvalue"].IsInt())
					{
						continue;
					}

					RangeInfo.nDefaultvalue = (*rangeIter)["Defaultvalue"].GetInt();

					// 最小值
					if (!rangeIter->HasMember("Minvalue") || !(*rangeIter)["Minvalue"].IsInt())
					{
						continue;
					}

					RangeInfo.nMinvalue = (*rangeIter)["Minvalue"].GetInt();

					// 最大值
					if (!rangeIter->HasMember("Maxvalue") || !(*rangeIter)["Maxvalue"].IsInt())
					{
						continue;
					}

					RangeInfo.nMaxvalue = (*rangeIter)["Maxvalue"].GetInt();

					// monitor
					if (!rangeIter->HasMember("Monitor") || !(*rangeIter)["Monitor"].IsString())
					{
						continue;
					}

					RangeInfo.strMonitorvalue = Utility::ConverUTF8ToGB2312((*rangeIter)["Monitor"].GetString());

					// 精度
					if (!rangeIter->HasMember("Accuracy") || !(*rangeIter)["Accuracy"].IsInt())
					{
						continue;
					}

					RangeInfo.nAccuracy = (*rangeIter)["Accuracy"].GetInt();

					// 单位
					if (!rangeIter->HasMember("Unit") || !(*rangeIter)["Unit"].IsString())
					{
						continue;
					}

					RangeInfo.strUnitName = Utility::ConverUTF8ToGB2312((*rangeIter)["Unit"].GetString());

					// 转换公式
					if (!rangeIter->HasMember("Converformula") || !(*rangeIter)["Converformula"].IsString())
					{
						continue;
					}

					RangeInfo.strConverformula = Utility::ConverUTF8ToGB2312((*rangeIter)["Converformula"].GetString());

					Element.SensorRangeInfoArray.push_back(RangeInfo);
				}

				// 默认频率
				if (!Iter->HasMember("Defaultfrequency") || !(*Iter)["Defaultfrequency"].IsFloat())
				{
					continue;
				}

				Element.fSensorDefaultfrequency =  (*Iter)["SensorCategory"].GetFloat();

				// 最小频率
				if (!Iter->HasMember("Minfrequency") || !(*Iter)["Minfrequency"].IsFloat())
				{
					continue;
				}

				Element.nSensorMinfrequency =  (*Iter)["Minfrequency"].GetFloat();

				// 最大频率
				if (!Iter->HasMember("Maxfrequency") || !(*Iter)["Maxfrequency"].IsFloat())
				{
					continue;
				}

				Element.nSensorMaxfrequency =  (*Iter)["Maxfrequency"].GetFloat();

				// 时长
				if (!Iter->HasMember("Timelength") || !(*Iter)["Timelength"].IsInt())
				{
					continue;
				}

				Element.nSensorTimeLength =  (*Iter)["Timelength"].GetInt();

				m_SensorConfigArray.push_back(Element);
			}
		}
	}

	return true;
}

/*******************************************************************
*函数名称:LoadSensorComList
*功能描述:加载传感器COM通信配置
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/10 8:39:02
*******************************************************************/
bool CSensorConfig::LoadSensorComList( rapidjson::Document& Parser )
{

	if (!Parser.HasMember("ComConfig"))
	{
		return false;
	}
	const rapidjson::Value& ComArray = Parser["ComConfig"];

	SENSOR_COM_CONFIG_ELEMENT ComElement;
	if (ComArray.IsObject())
	{
		// 端口号
		if (!ComArray.HasMember("PortName") || !ComArray["PortName"].IsString())
		{
			return false;
		}

		ComElement.strSerialPort = ComArray["PortName"].GetString();

		// 校验类型
		if (!ComArray.HasMember("Pairty") || !ComArray["Pairty"].IsInt())
		{
			return false;
		}

		ComElement.nPairty = ComArray["Pairty"].GetInt();

		// 停止位
		if (!ComArray.HasMember("StopBits") || !ComArray["StopBits"].IsInt())
		{
			return false;
		}

		ComElement.nStopBits = ComArray["StopBits"].GetInt();

		// 通信字节位数
		if (!ComArray.HasMember("DataBits") || !ComArray["DataBits"].IsInt())
		{
			return false;
		}

		ComElement.nDataBits = ComArray["DataBits"].GetInt();

		// 波特率
		if (!ComArray.HasMember("BaudRate") || !ComArray["BaudRate"].IsInt())
		{
			return false;
		}

		ComElement.nBaudRate = ComArray["BaudRate"].GetInt();

		// 是否使用流控
		if (!ComArray.HasMember("UseFlowControl") || !ComArray["UseFlowControl"].IsBool())
		{
			return false;
		}
		ComElement.bUseFlowControl = ComArray["UseFlowControl"].GetBool();

		m_SensorComConfig = ComElement;
	}
	return true;
}

/*******************************************************************
*函数名称:GetSensorList
*功能描述:获取传感器信息
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/10 8:39:02
*******************************************************************/
void CSensorConfig::GetSensorList(std::vector<SENSOR_CONFIG_ELEMENT> &vecSensorArry, SENSOR_TYPE enumType)
{
      BOOST_FOREACH(auto &sensor, m_SensorConfigArray)
	  {
	       if (sensor.nSensorCategory & enumType)
	       {
			   vecSensorArry.push_back(sensor);
	       }
	  }
}

/*******************************************************************
*函数名称:GetSensorInfo
*功能描述:根据传感器ID获取传感器信息
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/10 8:39:02
*******************************************************************/
SENSOR_CONFIG_ELEMENT CSensorConfig::GetSensorInfo(int nSensorID)
{
	BOOST_FOREACH(auto &sensor, m_SensorConfigArray)
	{
		if (nSensorID == sensor.nSensorID)
		{
			return sensor;
		}
	}

	return SENSOR_CONFIG_ELEMENT();
}

/*******************************************************************
*函数名称:GetSensorRecordInfo
*功能描述:获取传感器采样信息
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/10 8:39:02
*******************************************************************/
const SENSOR_RECORD_INFO& CSensorConfig::GetSensorRecordInfo()
{
	return m_SensorRecordInfo;
}

/*******************************************************************
*函数名称:SetSensorRecordInfo
*功能描述:设置传感器采样信息
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/10 8:39:02
*******************************************************************/
void CSensorConfig::SetSensorRecordInfo(SENSOR_RECORD_INFO& _recordInfo)
{
	m_SensorRecordInfo = _recordInfo;
}






CSensorConfig CSensorConfig::s_SensorConfig;
