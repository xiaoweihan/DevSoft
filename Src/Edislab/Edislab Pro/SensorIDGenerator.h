#pragma once
#include <vector>
#include <string>
#include "Type.h"
class CSensorIDGenerator
{

public:
	static CSensorIDGenerator& CreateInstance(void);

	int QuerySensorTypeIDByName(const std::string& strName);
private:
	CSensorIDGenerator(void);
	virtual ~CSensorIDGenerator(void);
private:
	static CSensorIDGenerator s_Instance;
	static int s_nTypeIndex;
private:
	std::vector<SENSOR_TYPE_INFO_ELEMENT> m_SensorTypeArray;

};

