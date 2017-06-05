#ifndef TYPE_H
#define TYPE_H
#include <vector>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/shared_lock_guard.hpp>
#include <boost/function.hpp>
#include "Macro.h"
class CSplitBar;
class CEdislabProView;
typedef std::vector<CWnd*> WidgetArray;
typedef std::vector<CWnd*>::iterator WidgetArrayIter;
typedef std::vector<CSplitBar*> SplitBarArray;
typedef std::vector<CSplitBar*>::iterator SplitBarArrayIter;

//分隔条的交点信息
typedef struct _split_intersect_rect
{
	CRect InterSectRect;

	//由哪2个矩形相交
	//水平分隔条
	CRect* pRect1;
	//垂直分隔条
	CRect* pRect2;
	//垂直分隔条的索引
	int nVerIndex;
	//水平分隔条的索引
	int nHorIndex;

	_split_intersect_rect(void)
	{
		pRect1 = NULL;
		pRect2 = NULL;
		nVerIndex = -1;
		nHorIndex = -1;
	}

}SPLIT_INTERSECT_RECT,* LP_SPLIT_INTERSECT_RECT;

//鼠标状态
enum MOUSE_STATE
{
	MOVE_NO_STATE = 0,
	//水平移动
	MOVE_HOR,
	//垂直移动
	MOVE_VER,
	//水平和垂直移动
	MOVE_ALL

};

//记录移动分隔条的信息
typedef struct _split_bar_info
{
	//分隔条类型
	MOUSE_STATE Type;
	//占用的索引
	unsigned int nIndex;

	_split_bar_info(void)
	{
		Type = MOVE_NO_STATE;
		nIndex = -1;
	}

}SPLIT_BAR_INFO,* LP_SPLIT_BAR_INFO;


enum NEW_PAGE_OPT
{
	NEW_PAGE_EMPTY = 0,
	NEW_PAGE_COPY,
	NEW_PAGE_CUSTOM
};

//新增元素的配置的结构体
typedef struct _add_page_element
{
	//新建页的选项
	NEW_PAGE_OPT eumOpt;
	//页的名称
	CString strPageName;
	
	int nGridNum;

	int nDiagramNum;

	int nDeviceNum;

	_add_page_element(void)
	{
		eumOpt = NEW_PAGE_COPY;
		nGridNum = 1;
		nDeviceNum = 1;
		nDiagramNum = 1;
		strPageName = DEFAULT_PAGE_NAME;
	}

}ADD_PAGE_ELEMENT,* LP_ADD_PAGE_ELEMENT;

enum SENSOR_TYPE{
	SENSOR_PHYSICS = 1,
	SENSOR_CHEMISTRY = 2,
	SENSOR_BIOLOGY = 4,
	SENSOR_ALL = 0xFFFFFF
};

// 传感器类型信息
typedef struct _sensor_type_info
{
	SENSOR_TYPE enumType;
	std::string strTypeName;

	_sensor_type_info()
	{
		enumType = SENSOR_ALL;
	}
}SENSOR_TYPE_INFO, *LP_SENSOR_TYPE_INFO;


//传感器信息定义
typedef struct _sensor_com_config_element
{
	//传感器ID
	int nSensorID;
	//传感器名称
	std::string strSensorName;
	//COM口索引
	std::string strSerialPort;
	//波特率
	unsigned int nBaudRate;
	//通信字节位数
	int nDataBits;
	//停止位
	int nStopBits;//(0:1个停止位 1:1.5个停止位(Linux暂不支持1.5个停止位) 2:2个停止位)
	//校验类型
	int nPairty;//(0:不使用校验  1:奇数校验 2:偶数校验 3:标记校验（Linux下没有此项） 4:空格校验)
	//是否使用流控
	bool bUseFlowControl;

	_sensor_com_config_element(void)
	{
		nSensorID = -1;
		strSensorName = "";
		nBaudRate = 9600;
		nDataBits = 8;
		nStopBits = 0;
		nPairty = 0;
		strSerialPort = "";
		bUseFlowControl = false;
	}
}SENSOR_COM_CONFIG_ELEMENT,* LP_SENSOR_COM_CONFIG_ELEMENT;

//传感器量程信息
typedef struct _sensor_range_info_element
{
	//量程名称
	std::string strRangeName;
	//校准值
	std::string strCalibrationvalue;
	//int nCalibrationvalue;
	//默认值
	int nDefaultvalue;
	//最大值
	int nMaxvalue;
	//最小值
	int nMinvalue;
	//monitor值
	std::string strMonitorvalue;
	//精度
	int nAccuracy;
	//单位
	std::string strUnitName;
	//转化公式
	std::string strConverformula;
	//转化回调函数
	boost::function<float(float,float,float)> pConverformulaFunc;

}SENSOR_RANGE_INFO_ELEMENT,* LP_SENSOR_RANGE_INFO_ELEMENT;

//传感器配置结构体定义
typedef struct _sensor_config_element
{
	//传感器的ID
	unsigned int nSensorID;
	//传感器类别 (1:物理 2:化学 4:生物 7:所有)
	int nSensorCategory;
	//传感器型号
	std::string strSensorModelName;
	//传感器名称
	std::string strSensorName;
	//传感器符号
	std::string strSensorSymbol;
	//传感器说明
	std::string strSensorDescription;
	//传感器的COM通信配置信息
	//SENSOR_COM_CONFIG_ELEMENT SensorComConfigInfo;
	// 当前选择的量程信息   add by tangmh
	int nCurRangeIndex;
	// end add
	//量程信息
	std::vector<SENSOR_RANGE_INFO_ELEMENT> SensorRangeInfoArray;
	//默认频率
	//int nSensorDefaultfrequency;
	float fSensorDefaultfrequency;
	//最大频率
	float nSensorMaxfrequency;
	//最小频率
	float nSensorMinfrequency;
	//时间长度
	int nSensorTimeLength;
	//多采样
	bool bMultiSample;
	//传感器类型
	std::string strSensorTypeName;
	//是否需要添加到列表中
	bool bAddtoSensorlist;

	_sensor_config_element(void)
	{
		nCurRangeIndex = -1;
		nSensorID = -1;
		bAddtoSensorlist = true;
		SensorRangeInfoArray.clear();
	}
}SENSOR_CONFIG_ELEMENT,* LP_SENSOR_CONFIG_ELEMENT;

//读锁
typedef boost::shared_lock_guard<boost::mutex> ReadLock;

//写锁
typedef boost::lock_guard<boost::mutex> WriteLock;


//传感器类型
typedef struct _sensor_type_info_element
{
	std::string strSensorName;
	int nSensorID;

	_sensor_type_info_element(void)
	{
		strSensorName = "";
		nSensorID = -1;
	}
}SENSOR_TYPE_INFO_ELEMENT,* LP_SENSOR_TYPE_INFO_ELEMENT;

//函数指针定义
typedef void (*pCommandEntry)(CEdislabProView* pView);
//更新函数指针定义
typedef void (*pUpdateCommandEntry)(CEdislabProView* pView,CCmdUI* pCmdUI);

enum RECORD_TYPE{
	FIXED_FREQUENCY_RC_TYPE,
	PHOTO_GATE_RC_TYPE,
	TRIGER_RC_TYPE,
	SPECIALIZED_RC_TYPE,
	MANUAL_RC_TYPE
};

typedef struct _sensor_record_info
{
	// 实验数据记录模式
	RECORD_TYPE enumRecordType;
	
	// 采样频率
	float fFrequency;

	// 是否限定时间
	bool bLimitTime;

	// 限定的时间
	float fLimitTime;

	// 是否在0时采样
	bool bSampleAtStart;

	// 是否多采样
	bool bMultiSample;

	// 多少点求均值
	int nAverageNums;

	_sensor_record_info()
	{
        enumRecordType = FIXED_FREQUENCY_RC_TYPE;
		fFrequency = 0.0f;
		bLimitTime = false;
		fLimitTime = 0.0f;
		bSampleAtStart = false;
		bMultiSample = false;
		nAverageNums = 1;
	}

}SENSOR_RECORD_INFO, *LPSENSOR_RECORD_INFO;


#endif