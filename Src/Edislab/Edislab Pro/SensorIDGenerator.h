#pragma once
#include <vector>
#include <string>
#include <boost/thread/mutex.hpp>
#include "Type.h"
class CSensorIDGenerator
{
public:
	static CSensorIDGenerator& CreateInstance(void);

	//根据设备名称查询设备编号
	int QuerySensorTypeIDByName(const std::string& strSensorName);

	//增加设备
	int AddSensor(const std::string& strSensorName);

	//删除设备
	void DelSensor(const std::string& strSensorName);

private:
	CSensorIDGenerator(void);
	virtual ~CSensorIDGenerator(void);
private:
	static CSensorIDGenerator s_Instance;
	static int s_nTypeIndex;
private:
	std::vector<SENSOR_TYPE_INFO_ELEMENT> m_SensorTypeArray;
	//锁
	boost::mutex m_Lock;
};

