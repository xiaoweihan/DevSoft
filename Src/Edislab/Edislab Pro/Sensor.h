#pragma once
class CSensor
{
public:
	CSensor(void);
	~CSensor(void);

private:
	char SensorType;
	CString m_strSensorName;
	int m_nSensorID;
	CString m_str;
};

