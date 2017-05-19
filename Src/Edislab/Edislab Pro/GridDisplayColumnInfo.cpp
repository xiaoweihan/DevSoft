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

	//刚开始的初始化为X和Y
	GRID_DISPLAY_INFO TempInfo;
	TempInfo.strHeadName = _T("当前");
	COLUMN_INFO TempColumnInfo;
	TempColumnInfo.strColumnName = _T("X");
	TempInfo.ContainColumnIndexArray.push_back(TempColumnInfo);
	TempColumnInfo.Reset();
	TempColumnInfo.strColumnName = _T("Y");
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

//int CGridDisplayColumnInfo::QuerySensorIDByColumnName(const CString& strColumnName)
//{
//	if (strColumnName.IsEmpty())
//	{
//		return -1;
//	}
//
//	BOOST_FOREACH(auto& HeaderElement,m_HeaderInfoArray)
//	{
//		BOOST_FOREACH(auto& ColumnElement,HeaderElement.ContainColumnIndexArray)
//		{
//			if (ColumnElement.strColumnName == strColumnName)
//			{
//				return ColumnElement.nSensorID;
//			}
//		}
//	}
//	return -1;
//}

void CGridDisplayColumnInfo::AddDisplayColumnInfo( const GRID_DISPLAY_INFO& GridColumnInfo )
{
	m_HeaderInfoArray.push_back(GridColumnInfo);
}

void CGridDisplayColumnInfo::AddDisplayColumnInfo( const CString& strHeaderName,const COLUMN_INFO& AddColumnInfo )
{
	//查找行头信息
	auto HeaderPred = [&strHeaderName](const GRID_DISPLAY_INFO& Info)->bool
	{
		if (Info.strHeadName == strHeaderName)
		{
			return true;
		}
		return false;
	};

	auto ColumnPred = [&AddColumnInfo](const COLUMN_INFO& ColumnInfo)->bool
	{
		if (ColumnInfo.nSensorID < 0 || ColumnInfo.strColumnName == AddColumnInfo.strColumnName)
		{
			return true;
		}
		return false;
	};

	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);
	
	//存在
	if (HeaderIter != m_HeaderInfoArray.end())
	{
		
		//在从中插找ID小于0的因素
		auto ColumnIter = std::find_if(HeaderIter->ContainColumnIndexArray.begin(),HeaderIter->ContainColumnIndexArray.end(),ColumnPred);
		//存在
		if (ColumnIter != HeaderIter->ContainColumnIndexArray.end())
		{
			if (ColumnIter->nSensorID < 0)
			{
				ColumnIter->nSensorID = AddColumnInfo.nSensorID;
				ColumnIter->strColumnName = AddColumnInfo.strColumnName;
			}
		}
		else
		{
			HeaderIter->ContainColumnIndexArray.push_back(AddColumnInfo);
		}
	}
}

void CGridDisplayColumnInfo::RemoveColumnInfo( const CString& strHeaderName,const CString& strColumnName )
{

	//查找行头信息
	auto HeaderPred = [&strHeaderName](const GRID_DISPLAY_INFO& Info)->bool
	{
		if (Info.strHeadName == strHeaderName)
		{
			return true;
		}
		return false;
	};

	auto ColumnPred = [&strColumnName](const COLUMN_INFO& ColumnInfo)->bool
	{
		if (ColumnInfo.strColumnName == strColumnName)
		{
			return true;
		}
		return false;
	};

	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);

	//存在
	if (HeaderIter != m_HeaderInfoArray.end())
	{

		//在从中插找ID小于0的因素
		auto ColumnIter = std::find_if(HeaderIter->ContainColumnIndexArray.begin(),HeaderIter->ContainColumnIndexArray.end(),ColumnPred);
		//存在
		if (ColumnIter != HeaderIter->ContainColumnIndexArray.end())
		{
			HeaderIter->ContainColumnIndexArray.erase(ColumnIter);

			//如果元素为空了，则移除整列
			if (HeaderIter->ContainColumnIndexArray.empty())
			{
				m_HeaderInfoArray.erase(HeaderIter);
			}
		}
	}
}

void CGridDisplayColumnInfo::RemoveHeader( const CString& strHeaderName )
{
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

	//存在
	if (HeaderIter != m_HeaderInfoArray.end())
	{
		m_HeaderInfoArray.erase(HeaderIter);
	}
}

CGridDisplayColumnInfo CGridDisplayColumnInfo::s_obj;
