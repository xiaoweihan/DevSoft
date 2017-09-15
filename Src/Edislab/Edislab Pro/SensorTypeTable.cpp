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
#include "StdAfx.h"
#include "SensorTypeTable.h"
#include <boost/thread/lock_factories.hpp>
#include <boost/assign.hpp>
#include <string>
#include <boost/scope_exit.hpp>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/encodedstream.h>
#include <boost/filesystem.hpp>
#include "Utility.h"
#include "Log.h"
CSensorTypeTable::CSensorTypeTable(void)
{
	m_SensorTypeMap.clear();
	//InitSensorTypeTable();
}


CSensorTypeTable::~CSensorTypeTable(void)
{
	m_SensorTypeMap.clear();
}

#if 0
void CSensorTypeTable::InitSensorTypeTable(void)
{
	int nSensorID = 0;
	std::string strSensorName = "静力传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);
	
	nSensorID = 1;
	strSensorName = "微力传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 2;
	strSensorName = "拉压式电子秤";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 3;
	strSensorName = "拉压力传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 4;
	strSensorName = "光电门传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 5;
	strSensorName = "声振动传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 6;
	strSensorName = "光照度传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 7;
	strSensorName = "电流传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 8;
	strSensorName = "电压传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 9;
	strSensorName = "微电流传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 10;
	strSensorName = "普通温度传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 11;
	strSensorName = "磁感应强度传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 12;
	strSensorName = "气体压强传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 13;
	strSensorName = "G-M计数器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 14;
	strSensorName = "位移（分体）";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 15;
	strSensorName = "光强度分布传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 16;
	strSensorName = "远程测距传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 17;
	strSensorName = "加速度传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 18;
	strSensorName = "高温传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 19;
	strSensorName = "氧气传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 20;
	strSensorName = "呼吸率传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 21;
	strSensorName = "PH值传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);
	
	
	nSensorID = 22;
	strSensorName = "氧化还原";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 23;
	strSensorName = "氯离子传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 24;
	strSensorName = "硝酸根传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 25;
	strSensorName = "氨根传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 26;
	strSensorName = "钠离子传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);


	nSensorID = 27;
	strSensorName = "电导率传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 28;
	strSensorName = "总盐度传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 29;
	strSensorName = "溶解氧传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 30;
	strSensorName = "二氧化碳传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 31;
	strSensorName = "酒精传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 32;
	strSensorName = "心率传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 33;
	strSensorName = "心电图传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 34;
	strSensorName = "湿度传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);
	
	nSensorID = 35;
	strSensorName = "色度计";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);


	nSensorID = 36;
	strSensorName = "浊度计";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 37;
	strSensorName = "常开/常闭控制传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 38;
	strSensorName = "热辐射传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 39;
	strSensorName = "二氧化硫/氯气/一氧化碳传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 40;
	strSensorName = "角度传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 41;
	strSensorName = "氢气/甲烷传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 42;
	strSensorName = "微电压传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 43;
	strSensorName = "多量程电流传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 44;
	strSensorName = "多量程电压传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);

	nSensorID = 45;
	strSensorName = "电流/电压合体传感器";
	m_SensorTypeMap.emplace(nSensorID,strSensorName);
}
#endif

CSensorTypeTable& CSensorTypeTable::CreateInstance(void)
{
	return s_obj;
}


CSensorTypeTable::SENSOR_TYPE_VALUE CSensorTypeTable::QuerySensorNameByID(int nSensorTypeID)
{
	SENSOR_TYPE_VALUE Result;

	auto Lock = boost::make_unique_lock(m_Lock);

	auto Iter = m_SensorTypeMap.find(nSensorTypeID);

	if (Iter != m_SensorTypeMap.end())
	{
		Result = Iter->second;
	}

	return Result;
}

bool CSensorTypeTable::LoadSensorTypeListFromFile(void)
{
	using namespace std;
	using namespace rapidjson;
	std::string strConfigPath = Utility::GetExeDirecory() + std::string("\\SensorType.json");

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
		FILE* fp = fopen(strConfigPath.c_str(),"rb");

		if (nullptr == fp)
		{
			ERROR_LOG("open [%s] file failed.", strConfigPath.c_str());
			return false;
		}
		BOOST_SCOPE_EXIT(&fp)
		{
			if (nullptr != fp)
			{
				fclose(fp);
				fp = nullptr;
			}

		}BOOST_SCOPE_EXIT_END
		//获取文件的长度
		fseek(fp,0,SEEK_END);
		int nFileLength = (int)ftell(fp);
		if (nFileLength < 0)
		{
			ERROR_LOG("get [%s] file length failed.", strConfigPath.c_str());
			return false;
		}
		//申请内存
		char* pBuffer =  new char[nFileLength];
		if (nullptr == pBuffer)
		{
			ERROR_LOG("Allocate memory failed.");
			return false;
		}
		BOOST_SCOPE_EXIT(&pBuffer)
		{
			if (nullptr != pBuffer)
			{
				delete []pBuffer;
			}
		}BOOST_SCOPE_EXIT_END
		ZeroMemory(pBuffer,nFileLength);
		fseek(fp,0,SEEK_SET);
		FileReadStream bis(fp,pBuffer,nFileLength);
		AutoUTFInputStream<unsigned, FileReadStream> eis(bis);
		Document Parser;      
		Parser.ParseStream<0, AutoUTF<unsigned> >(eis);
		if (Parser.HasParseError())  
		{  
			ERROR_LOG("Parse json file [%s] failed,the error [%d].", strConfigPath.c_str(),Parser.GetParseError());
			return false;
		}  

		//加载传感器类型列表
		if (!LoadSensorTypeList(Parser))
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

bool CSensorTypeTable::LoadSensorTypeList(rapidjson::Document& Parser)
{
	const rapidjson::Value& SensorTypeArray = Parser["SensorTypeList"];

	if (!SensorTypeArray.IsArray())
	{
		return false;
	}


	for (auto Iter = SensorTypeArray.Begin(); Iter != SensorTypeArray.End(); ++Iter)
	{
		if (Iter->IsObject())
		{
			SENSOR_TYPE_VALUE SensorElement;
			// 传感器ID
			if (!Iter->HasMember("SensorTypeID") || !(*Iter)["SensorTypeID"].IsInt())
			{
				continue;
			}

			SensorElement.nSensorTypeID = (*Iter)["SensorTypeID"].GetInt();

			// 传感器名
			if (!Iter->HasMember("SensorTypeName") || !(*Iter)["SensorTypeName"].IsString())
			{
				continue;
			}
			SensorElement.strSensorTypeName = Utility::ConverUTF8ToGB2312((*Iter)["SensorTypeName"].GetString());

			// 传感器单位
			if (!Iter->HasMember("SensorTypeUnit") || !(*Iter)["SensorTypeUnit"].IsString())
			{
				continue;
			}
			SensorElement.strSensorUnit = (*Iter)["SensorTypeUnit"].GetString();

			m_SensorTypeMap.emplace(SensorElement.nSensorTypeID,SensorElement);
		}
	}
	return true;
}

CSensorTypeTable CSensorTypeTable::s_obj;
