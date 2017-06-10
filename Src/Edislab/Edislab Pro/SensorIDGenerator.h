#pragma once
#include <vector>
#include <string>
#include <boost/thread/mutex.hpp>
#include "Type.h"
class CSensorIDGenerator
{
public:
	//创建单实例
	static CSensorIDGenerator& CreateInstance(void);

	//根据设备名称查询设备编号
	int QuerySensorTypeIDByName(const std::string& strSensorName);
	std::string QueryrNameBySensorID(int id);

	//增加设备
	int AddSensor(const std::string& strSensorName);

	//删除设备
	void DelSensor(const std::string& strSensorName);

	//获取所有的传感器名称
	void GetAllSensorName(std::vector<std::string>& SensorNameArray);

	//判断传感器名称是否存在
	bool IsSensorExist(const std::string& strSensorName);

private:
	CSensorIDGenerator(void);
	virtual ~CSensorIDGenerator(void);
private:
	//唯一的单实例
	static CSensorIDGenerator s_Instance;
	//设备ID生成器
	static int s_nTypeIndex;
private:
	//设备名称与设备ID表
	std::vector<SENSOR_TYPE_INFO_ELEMENT> m_SensorTypeArray;
	//锁
	boost::mutex m_Lock;
};

