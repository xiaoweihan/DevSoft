#ifndef SENSOR_COM_MANAGER_H
#define SENSOR_COM_MANAGER_H
#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>
class CSerialPortService;
class CSensorComManager
{
public:
	//创建单实例
	static CSensorComManager& CreateInstance(void);

	//增加传感器ID对应的通信类
	void AddSensorCom(int nSensorID);

	//删除传感器ID对应的通信类
	void DelSensorCom(int nSensorID);

	//根据设备的ID找对应的通信类
	CSerialPortService* QueryComBySensorID(int nSensorID);

	//所有传感器开始采集
	void StartSensorCollect();

	void StopSensorCollect();
private:
	CSensorComManager(void);
	~CSensorComManager(void);
private:
	boost::unordered_map<int,CSerialPortService*> m_SensorComMap;
	boost::mutex m_Lock;
private:
	static CSensorComManager s_obj;
};

#endif