/**********************************************************
*版权所有:xiaowei.han
*文件名称:GridDisplayColumnInfo.h
*功能描述:Grid显示列信息数据管理器
*作者:xiaowei.han
*日期:2017/05/14
**********************************************************/
#pragma once
#include <vector>

typedef struct _column_info
{
	CString strColumnName;
	int nSensorID;

	_column_info()
	{
		strColumnName = _T("");
		nSensorID = -1;
	}

	void Reset()
	{
		strColumnName = _T("");
		nSensorID = -1;
	}
}COLUMN_INFO,* LP_COLUMN_INFO;

typedef struct _grid_display_info
{
	//列的名称
	CString strHeadName;

	//包含列的索引
	std::vector<COLUMN_INFO> ContainColumnIndexArray;

	_grid_display_info()
	{
		strHeadName = _T("");
		ContainColumnIndexArray.clear();
	}

	void Reset()
	{
		strHeadName = _T("");
		ContainColumnIndexArray.clear();
	}

}GRID_DISPLAY_INFO,* LP_GRID_DISPLAY_INFO;

class CGridDisplayColumnInfo
{
public:
	static CGridDisplayColumnInfo& CreateInstance();

	void GetGridDisplayInfo(std::vector<GRID_DISPLAY_INFO>& DisplayInfoArray);

	//查询表头名称是否已经存在
	bool IsHeaderNameExist(const CString& strHeaderName);

	//判断指定表头列名是否已经存在
	bool IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName);

	//根据列名称获取对应的传感器ID
	int QuerySensorIDByColumnName(const CString& strColumnName);

	//添加列信息
	void AddDisplayColumnInfo(const GRID_DISPLAY_INFO& GridColumnInfo);

	//在某个头中添加一列信息
	void AddDisplayColumnInfo(const CString& strHeaderName,const COLUMN_INFO& AddColumnInfo);

    //移除某一列
	void RemoveColumnInfo(const CString& strHeaderName,const CString& strColumnName);
	
	//移除某一表头
	void RemoveHeader(const CString& strHeaderName);
protected:
	CGridDisplayColumnInfo(void);
	~CGridDisplayColumnInfo(void);
private:
	void InitGridDisplayInfo();
protected:
	//Grid的显示信息
	std::vector<GRID_DISPLAY_INFO> m_HeaderInfoArray;
private:
	static CGridDisplayColumnInfo s_obj;

};

