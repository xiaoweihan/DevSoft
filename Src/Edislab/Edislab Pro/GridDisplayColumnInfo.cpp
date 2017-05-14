/**********************************************************
*版权所有:xiaowei.han
*文件名称:GridDisplayColumnInfo.cpp
*功能描述:Grid显示列信息数据管理器
*作者:xiaowei.han
*日期:2017/05/14
**********************************************************/
#include "StdAfx.h"
#include "GridDisplayColumnInfo.h"
#include <algorithm>
#include <boost/foreach.hpp>
CGridDisplayColumnInfo& CGridDisplayColumnInfo::CreateInstance()
{
	return s_obj;
}

void CGridDisplayColumnInfo::GetGridDisplayInfo(std::vector<GRID_DISPLAY_INFO>& DisplayInfoArray)
{
	DisplayInfoArray.clear();
	DisplayInfoArray.assign(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end());
}

CGridDisplayColumnInfo::CGridDisplayColumnInfo(void)
{
	m_HeaderInfoArray.clear();
	InitGridDisplayInfo();
}


CGridDisplayColumnInfo::~CGridDisplayColumnInfo(void)
{
	m_HeaderInfoArray.clear();
}

void CGridDisplayColumnInfo::InitGridDisplayInfo()
{
	GRID_DISPLAY_INFO TempInfo;
	TempInfo.strHeadName = _T("当前");
	COLUMN_INFO TempColumnInfo;
	TempColumnInfo.strColumnName = _T("t(s)时间");
	TempInfo.ContainColumnIndexArray.push_back(TempColumnInfo);
	TempColumnInfo.Reset();
	TempColumnInfo.strColumnName = _T("p(kpa)压强");
	TempColumnInfo.nSensorID = 1;
	TempInfo.ContainColumnIndexArray.push_back(TempColumnInfo);
	m_HeaderInfoArray.push_back(TempInfo);
}

/*******************************************************************
*函数名称:IsHeaderNameExist
*功能描述:判断行头是否已经存在
*输入参数:strHeaderName:行头的名称
*输出参数:None
*返回值:true:存在 false:不存在
*作者:xiaowei.han
*日期:2017/05/13 15:59:47
*******************************************************************/
bool CGridDisplayColumnInfo::IsHeaderNameExist(const CString& strHeaderName)
{
	if (strHeaderName.IsEmpty())
	{
		return false;
	}
	//查找行头信息
	auto HeaderPred = [&strHeaderName](const GRID_DISPLAY_INFO& Info)->bool
	{
		if (Info.strHeadName == strHeaderName)
		{
			return true;
		}
		return false;
	};
	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);
	if (HeaderIter != m_HeaderInfoArray.end())
	{
		return true;
	}
	return false;
}

/*******************************************************************
*函数名称:IsColumnExistInFixedHeader
*功能描述:判断列是否已经存在
*输入参数:strHeaderName:行头的名称 strColumnName:列头名称
*输出参数:None
*返回值:true:存在 false:不存在
*作者:xiaowei.han
*日期:2017/05/13 15:59:47
*******************************************************************/
bool CGridDisplayColumnInfo::IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName)
{
	if (strHeaderName.IsEmpty() || strColumnName.IsEmpty())
	{
		return false;
	}

	//查找某列的名字是否等于要删除的
	auto HeaderPred = [&strHeaderName](const GRID_DISPLAY_INFO& Info)->bool
	{
		if (Info.strHeadName == strHeaderName)
		{
			return true;
		}
		return false;
	};
	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);
	if (HeaderIter == m_HeaderInfoArray.end())
	{
		return false;
	}

	auto ColumnPred = [&strColumnName](const COLUMN_INFO& ColumnInfo)->bool
	{
		if (ColumnInfo.strColumnName == strColumnName)
		{
			return true;
		}
		return false;
	};

	auto ColumnIter = std::find_if(HeaderIter->ContainColumnIndexArray.begin(),HeaderIter->ContainColumnIndexArray.end(),ColumnPred);
	//不存在
	if (ColumnIter == HeaderIter->ContainColumnIndexArray.end())
	{
		return false;
	}
	return true;
}

int CGridDisplayColumnInfo::QuerySensorIDByColumnName(const CString& strColumnName)
{
	if (strColumnName.IsEmpty())
	{
		return -1;
	}

	BOOST_FOREACH(auto& HeaderElement,m_HeaderInfoArray)
	{
		BOOST_FOREACH(auto& ColumnElement,HeaderElement.ContainColumnIndexArray)
		{
			if (ColumnElement.strColumnName == strColumnName)
			{
				return ColumnElement.nSensorID;
			}
		}
	}
	return -1;
}

CGridDisplayColumnInfo CGridDisplayColumnInfo::s_obj;
