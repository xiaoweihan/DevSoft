/****************************************************************************************
*版权所有:
*文件名:Global.h
*描述:全局变量定义
*日期:2016.9
******************************************************************************************/

#ifndef GLOBAL_H
#define GLOBAL_H
#include <set>
#include <vector>

#define SET std::set
#define VECTOR std::vector 
#include <afx.h>		//此引用，待删除（没必要的引用）
///************************************************************************/
//Grid常量定义
///************************************************************************/
const COLORREF UnActiveColor = RGB(200,200,200);
const COLORREF ActiveColor = RGB(0,0,25);
#define		DEFUALT_COLUMN_NAME_X	_T("X")
#define		DEFUALT_COLUMN_NAME_Y	_T("Y")
#define		CURRENT_GROUP			_T("当前组")
#define		NEW_GROUP				_T("新组")
#define		NEW_COLUMN				_T("新栏")
#define		GRID_HEADER_LINES		2
#define		DEFAULT_COLUMN_WIDTH	100
#define		INIT_ROW_NUM			100
/***********************************全局数据基本定义如下**********************************************/
#define GROUP_ID				0	//组ID从0开始
#define	VEC_STRING				VECTOR<CString>
#define SENSOR_MAX_ID			70	//传感器设备列ID(1~70)
#define USE_COL_ID				101	//用户自定义列ID从101开始，累加
#define DEFAULT_COL_COUNT		2	//默认数据2列
#define DEFAULT_ROW_COUNT		100	//默认数据100行
#define DEFAULT_DATA			_T("")//默认空数据
#define MAX_GROUP_NUM			20	//最多支持20组
#define MAX_COLUMN_NUM			20	//每组最多支持20组
#define LAB_MAX_COUNT			6
#define MAX_WIDGET_NUM			9
/******************************************************************************/
/////////////////////////////////实验数据定义区//////////////////////////////////
/******************************************************************************/
///************************************************************************/
//控件数据定义
///************************************************************************/
//点尺寸--实验数据列属性
typedef enum _COLUMN_POINT_SIZE
{
	//超大
	POINTSIZE_HUGE=0, 
	//大
	POINTSIZE_LARGE, 
	//较大
	POINTSIZE_LARGER, 
	//普通
	POINTSIZE_NORMAL,
	//小
	POINTSIZE_SMALL
}COLUMN_POINT_SIZE;

//类型--实验数据列属性
typedef enum _COLUMN_TYPE
{
	//手工栏
	TYPE_HANDWORK=0, 
	//计算栏
	TYPE_CALCULATE,
	//传感器栏
	TYPE_SENSOR,
	//时间栏
	TYPE_TIME,
}COLUMN_USE_TYPE;

//小数点--实验数据列属性
typedef enum _COLUMN_DECIMAL_POINT
{
	//自动
	DECIMAL_POINT_AUTO=-1, 
	DECIMAL_POINT_0, 
	DECIMAL_POINT_1, 
	DECIMAL_POINT_2,
	DECIMAL_POINT_3,
	DECIMAL_POINT_4,
	DECIMAL_POINT_5,
	DECIMAL_POINT_6,
	DECIMAL_POINT_7,
	DECIMAL_POINT_8,
	DECIMAL_POINT_9
}COLUMN_DECIMAL_POINT;

//编辑--实验数据列属性
typedef enum _COLUMN_EDIT
{
	//允许
	COLUMN_EDIT_PERMIT=0, 
	//仅数字
	COLUMN_EDIT_ONLY_NUM,
	//禁止
	COLUMN_EDIT_FORBID
}COLUMN_EDIT;
//默认颜色池
//ID管理
class IDManager
{
public:
	static IDManager* CreateInstance()
	{
		static IDManager s_IDManager;
		return &s_IDManager;
	}
private:
	IDManager(void){}
	virtual ~IDManager(void){}

	SET<int> idSet;
public:
	//生成一个ID
	int generateID()
	{
		int id=-1;
		for(int i=0; i<INT_MAX; ++i)
		{
			if(idSet.find(i)==idSet.end())
			{
				id = i;
				idSet.insert(id);
				break;
			}
		}
		return id;
	}
	//删除一个ID
	void delID(int id)
	{
		idSet.erase(id);
	}
};
//Grid列 数据结构定义
typedef struct _ColumnData
{
	//数据列ID
	int		nColumnID;
	//传感器ID
	int		nSensorID;
	//通道ID
	int		nSensorChanel;
	//数据列名称
	CString strColumnName;
	//注释
	CString strColumnUnit;
	//列颜色
	COLORREF	nColColor;
	//点尺寸
	COLUMN_POINT_SIZE	emPointSize;

	//类型
	COLUMN_USE_TYPE		emType;
	//小数点
	COLUMN_DECIMAL_POINT	emDecimalPoint;
	//编辑
	COLUMN_EDIT		emEdit;
	//数据
	VEC_STRING data;
	_ColumnData(void)
	{
		nColumnID = -1;//IDManager::CreateInstance()->generateID();
		nSensorID = -1;
		nSensorChanel = -1;
		strColumnName.Empty();
		strColumnUnit.Empty();
		nColColor = 0;
		emPointSize = POINTSIZE_NORMAL;
		emType = TYPE_HANDWORK;
		emDecimalPoint = DECIMAL_POINT_AUTO;
		emEdit = COLUMN_EDIT_PERMIT;
	}
	~_ColumnData(){
		//IDManager::CreateInstance()->delID(nColumnID);
	}

}COLUMNDATA,* LPCOLUMNDATA;

//Grid组 数据结构定义
typedef struct _GroupData
{
	//数据组ID
	int			nGroupID;
	//数据组名称
	CString		strGroupName;
	//注释
	CString		strGroupNote;
	//添加时间
	SYSTEMTIME	sysAddTime;
	//默认组 //当前组与其他组
	bool		bDefault;

	VECTOR<COLUMNDATA> vecColData;

	_GroupData(void)
	{
		nGroupID = -1;//IDManager::CreateInstance()->generateID();
		strGroupName = CURRENT_GROUP;
		strGroupNote.Empty();
		GetLocalTime(&sysAddTime);
		bDefault = false;

		vecColData.clear();
	}
	~_GroupData()
	{
		//IDManager::CreateInstance()->delID(nGroupID);
	}

}GROUPDATA,* LPGROUPDATA;

//传感器与列索引的结构
typedef struct _PairIndex
{
	//组索引
	int	nGroup;

	//列信息索引
	int	nID;

	//列数据索引
	int nColIndex;

	//每列的实际数据个数
	int nColDataCount;

	_PairIndex()
	{
		nID = 0;
		nColIndex = 0;
		nColDataCount = 0;
	}

}PAIRINDEX,* LPPAIRINDEX;

//行列数据
//extern vector<VEC_STRING>		g_LabData;
//
////行列数据归属/属性
//extern vector<GROUPDATA>		g_LabDataAttribution;
//
////实验数据行数
//extern int						g_LabDataRowCount;
//
////数据头
//extern vector<PAIRINDEX>		g_LabDataIndex;

extern BOOL g_bAutoSelect;
#endif
