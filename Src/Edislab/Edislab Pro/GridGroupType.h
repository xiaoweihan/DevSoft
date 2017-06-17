/**********************************************************
*版权所有:xiaowei.han
*文件名称:GridGroupType.h
*功能描述:Grid分组显示的类型定义
*作者:xiaowei.han
*日期:2017/06/04
**********************************************************/

#ifndef GRID_GROUP_H
#define GRID_GROUP_H
#include <vector>
//Grid中的列的信息
typedef struct _column_info
{
	CString strColumnName;
	_column_info()
	{
		strColumnName = _T("");
	}
	void Reset()
	{
		strColumnName = _T("");
	}
}COLUMN_INFO,* LP_COLUMN_INFO;

//Grid控件中列的分组信息
typedef struct _column_group_info
{
	//分组的名称
	CString strGroupName;

	//子项目的信息
	std::vector<COLUMN_INFO> ColumnArray;

	_column_group_info()
	{
		strGroupName = _T("");
		ColumnArray.clear();
	}

	void Reset()
	{
		strGroupName = _T("");
		ColumnArray.clear();
	}

}COLUMN_GROUP_INFO,* LP_COLUMN_GROUP_INFO;


//列的显示信息
typedef struct _show_column_info
{
	CString strColumnName;
	bool bShow;
	_show_column_info()
	{
		strColumnName = _T("");
		bShow = true;
	}
	void Reset()
	{
		strColumnName = _T("");
		bShow = true;
	}
}SHOW_COLUMN_INFO,* LP_SHOW_COLUMN_INFO;

//列的分组显示信息
typedef struct _show_column_group_info
{
	//分组的名称
	CString strGroupName;

	//子项目的信息
	std::vector<SHOW_COLUMN_INFO> ColumnArray;

	_show_column_group_info()
	{
		strGroupName = _T("");
		ColumnArray.clear();
	}

	void Reset()
	{
		strGroupName = _T("");
		ColumnArray.clear();
	}

}SHOW_COLUMN_GROUP_INFO,* LP_SHOW_COLUMN_GROUP_INFO;

#endif