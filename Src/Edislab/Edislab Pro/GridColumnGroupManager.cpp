/**********************************************************
*版权所有:xiaowei.han
*文件名称:GridDisplayColumnInfo.cpp
*功能描述:Grid显示列信息数据管理器
*作者:xiaowei.han
*日期:2017/05/14
**********************************************************/
#include "StdAfx.h"
#include "GridColumnGroupManager.h"
#include <algorithm>
#include <boost/foreach.hpp>
#include "Utility.h"
CGridColumnGroupManager& CGridColumnGroupManager::CreateInstance()
{
	return s_obj;
}

/*******************************************************************
*函数名称:GetGridDisplayInfo
*功能描述:获取列分组信息
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/04 21:48:09
*******************************************************************/
void CGridColumnGroupManager::GetGridDisplayInfo(std::vector<COLUMN_GROUP_INFO>& DisplayInfoArray)
{
	DisplayInfoArray.clear();
	DisplayInfoArray.assign(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end());
}

CGridColumnGroupManager::CGridColumnGroupManager(void)
{
	m_HeaderInfoArray.clear();
	InitGridDisplayInfo();
}


CGridColumnGroupManager::~CGridColumnGroupManager(void)
{
	m_HeaderInfoArray.clear();
}

/*******************************************************************
*函数名称:InitGridDisplayInfo
*功能描述:初始化列分组信息
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/04 21:48:09
*******************************************************************/
void CGridColumnGroupManager::InitGridDisplayInfo()
{

#if 0
	//初始化显示
	COLUMN_GROUP_INFO TempInfo;
	TempInfo.strGroupName = _T("当前");
	COLUMN_INFO TempColumnInfo;
	TempColumnInfo.strColumnName = _T("X");
	TempInfo.ColumnArray.push_back(TempColumnInfo);
	TempColumnInfo.Reset();
	TempColumnInfo.strColumnName = _T("Y");
	TempInfo.ColumnArray.push_back(TempColumnInfo);	
	
	m_HeaderInfoArray.push_back(TempInfo);
#endif
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
bool CGridColumnGroupManager::IsHeaderNameExist(const CString& strHeaderName)
{
	if (strHeaderName.IsEmpty())
	{
		return false;
	}
	//查找行头信息
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
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
bool CGridColumnGroupManager::IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName)
{
	if (strHeaderName.IsEmpty() || strColumnName.IsEmpty())
	{
		return false;
	}

	//查找某列的名字是否等于要删除的
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
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

	auto ColumnIter = std::find_if(HeaderIter->ColumnArray.begin(),HeaderIter->ColumnArray.end(),ColumnPred);
	//不存在
	if (ColumnIter == HeaderIter->ColumnArray.end())
	{
		return false;
	}
	return true;
}

/*******************************************************************
*函数名称:AddDisplayColumnInfo
*功能描述:添加列分组的信息
*输入参数:None
*输出参数:None
*返回值:None
*作者:xiaowei.han
*日期:2017/05/13 15:59:47
*******************************************************************/
void CGridColumnGroupManager::AddDisplayColumnInfo( const COLUMN_GROUP_INFO& GridColumnInfo )
{
	//查找行头信息
	auto HeaderPred = [&GridColumnInfo](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == GridColumnInfo.strGroupName)
		{
			return true;
		}
		return false;
	};

	auto HeaderIter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),HeaderPred);
	//如果存在
	if (HeaderIter != m_HeaderInfoArray.end())
	{
		return;
	}

	m_HeaderInfoArray.push_back(GridColumnInfo);
}

/*******************************************************************
*函数名称:AddDisplayColumnInfo
*功能描述:添加列到已知分组的信息
*输入参数:None
*输出参数:None
*返回值:None
*作者:xiaowei.han
*日期:2017/05/13 15:59:47
*******************************************************************/
void CGridColumnGroupManager::AddDisplayColumnInfo( const CString& strHeaderName,const COLUMN_INFO& AddColumnInfo )
{
	//查找行头信息
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
		{
			return true;
		}
		return false;
	};

	//寻找列名称的lamda表达式
	auto ColumnPred = [&AddColumnInfo](const COLUMN_INFO& ColumnInfo)->bool
	{
		//列名称相同
		if (ColumnInfo.strColumnName == AddColumnInfo.strColumnName)
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
		auto ColumnIter = std::find_if(HeaderIter->ColumnArray.begin(),HeaderIter->ColumnArray.end(),ColumnPred);
		//存在
		if (ColumnIter != HeaderIter->ColumnArray.end())
		{
			return;
		}
		else
		{
			HeaderIter->ColumnArray.push_back(AddColumnInfo);
		}
	}
	//没有则添加一个
	else
	{
		COLUMN_GROUP_INFO NewGroupInfo;
		NewGroupInfo.strGroupName = strHeaderName;
		NewGroupInfo.ColumnArray.push_back(AddColumnInfo);
		m_HeaderInfoArray.push_back(NewGroupInfo);
	}
}

/*******************************************************************
*函数名称:RemoveColumnInfo
*功能描述:移除列信息
*输入参数:None
*输出参数:None
*返回值:None
*作者:xiaowei.han
*日期:2017/05/13 15:59:47
*******************************************************************/
void CGridColumnGroupManager::RemoveColumnInfo( const CString& strHeaderName,const CString& strColumnName )
{

	//查找行头信息
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
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
		auto ColumnIter = std::find_if(HeaderIter->ColumnArray.begin(),HeaderIter->ColumnArray.end(),ColumnPred);
		//存在
		if (ColumnIter != HeaderIter->ColumnArray.end())
		{
			HeaderIter->ColumnArray.erase(ColumnIter);

			//如果元素为空了，则移除整列
			if (HeaderIter->ColumnArray.empty())
			{
				m_HeaderInfoArray.erase(HeaderIter);
			}
		}
	}
}

/*******************************************************************
*函数名称:RemoveHeader
*功能描述:移除分组信息
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/04 21:50:34
*******************************************************************/
void CGridColumnGroupManager::RemoveHeader( const CString& strHeaderName )
{
	//查找行头信息
	auto HeaderPred = [&strHeaderName](const COLUMN_GROUP_INFO& Info)->bool
	{
		if (Info.strGroupName == strHeaderName)
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

CGridColumnGroupManager CGridColumnGroupManager::s_obj;
