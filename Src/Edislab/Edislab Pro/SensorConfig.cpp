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
#include "SensorIDGenerator.h"
#include "SensorComManager.h"
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

		// 解析传感器配置
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

							RangeInfo.strRangeName =  Utility::ConverUTF8ToGB2312 ((*rangeIter)["Name"].GetString());

							// 校准值
							if (!rangeIter->HasMember("Calibrationvalue") || !(*rangeIter)["Calibrationvalue"].IsInt())
							{
								continue;
							}

							RangeInfo.nCalibrationvalue =  (*rangeIter)["Calibrationvalue"].GetInt();

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
		}

		if (Parser.HasMember("ComConfig"))
		{

			const rapidjson::Value& ComArray = Parser["ComConfig"];

			if (ComArray.IsArray())
			{
				for (auto Iter = ComArray.Begin(); Iter != ComArray.End(); ++Iter)
				{
					SENSOR_COM_CONFIG_ELEMENT ComElement;
					if (Iter->IsObject())
					{
						// 传感器名称
						if (!Iter->HasMember("SensorName") || !(*Iter)["SensorName"].IsString())
						{
							continue;
						}

						ComElement.strSensorName = Utility::ConverUTF8ToGB2312((*Iter)["SensorName"].GetString());

						// 端口号
						if (!Iter->HasMember("PortName") || !(*Iter)["PortName"].IsInt())
						{
							continue;
						}

						ComElement.nComIndex = (*Iter)["PortName"].GetInt();

						// 校验类型
						if (!Iter->HasMember("Pairty") || !(*Iter)["Pairty"].IsInt())
						{
							continue;
						}

						ComElement.nPairty = (*Iter)["Pairty"].GetInt();

						// 停止位
						if (!Iter->HasMember("StopBits") || !(*Iter)["StopBits"].IsInt())
						{
							continue;
						}

						ComElement.nStopBits = (*Iter)["StopBits"].GetInt();

						// 通信字节位数
						if (!Iter->HasMember("DataBits") || !(*Iter)["DataBits"].IsInt())
						{
							continue;
						}

						ComElement.nDataBits = (*Iter)["DataBits"].GetInt();

						// 波特率
						if (!Iter->HasMember("BaudRate") || !(*Iter)["BaudRate"].IsInt())
						{
							continue;
						}

						ComElement.nBaudRate = (*Iter)["BaudRate"].GetInt();

						// 是否使用流控
						if (!Iter->HasMember("UseFlowControl") || !(*Iter)["UseFlowControl"].IsBool())
						{
							continue;
						}
						ComElement.bUseFlowControl = (*Iter)["UseFlowControl"].GetBool();
						//添加传感器
						int nSensorID = CSensorIDGenerator::CreateInstance().AddSensor(ComElement.strSensorName);
						if (nSensorID >= 0)
						{
							//添加对应SensorID的数据
							CSensorDataManager::CreateInstance().AddSensorData(nSensorID);
							//添加通信类
							CSensorComManager::CreateInstance().AddSensorCom(nSensorID);
							CSerialPortService* pCom = CSensorComManager::CreateInstance().QueryComBySensorID(nSensorID);
							if (nullptr != pCom)
							{
								//通信配置
								COMPROPERTY SensorComOption;
								SensorComOption.nBaudRate = ComElement.nBaudRate;
								SensorComOption.nDataBits = ComElement.nDataBits;
								SensorComOption.nPairty = ComElement.nPairty;
								SensorComOption.nStopBits = ComElement.nStopBits;
								if (ComElement.bUseFlowControl)
								{
									SensorComOption.nFlowControl = 1;
								}
								else
								{
									SensorComOption.nFlowControl = 0;
								}
								pCom->SetSerialPortOption(ComElement.nComIndex,SensorComOption);
							}

						}
						ComElement.nSensorID = nSensorID;
						m_SensorComConfigArray.push_back(ComElement);
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

CSensorConfig CSensorConfig::s_SensorConfig;
