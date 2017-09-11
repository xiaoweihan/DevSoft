/*********************************************************
Copyright(C):
FileName:SensorTypeTable.h
Descripton:预支传感器类型表
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
CSensorTypeTable::CSensorTypeTable(void)
{
	m_SensorTypeMap.clear();
	InitSensorTypeTable();
}


CSensorTypeTable::~CSensorTypeTable(void)
{
	m_SensorTypeMap.clear();
}

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

CSensorTypeTable& CSensorTypeTable::CreateInstance(void)
{
	return s_obj;
}

std::string CSensorTypeTable::QuerySensorNameByID(int nSensorTypeID)
{
	auto Lock = boost::make_unique_lock(m_Lock);

	auto Iter = m_SensorTypeMap.find(nSensorTypeID);

	if (Iter != m_SensorTypeMap.end())
	{
		return Iter->second;
	}

	return "";
}

CSensorTypeTable CSensorTypeTable::s_obj;
