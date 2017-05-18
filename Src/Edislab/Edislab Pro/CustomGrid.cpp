/**********************************************************
*版权所有:xiaowei.han
*文件名称:CustomGrid.cpp
*功能描述:自定义的Grid控件
*作者:xiaowei.han
*日期:2017/04/22
**********************************************************/
#include "stdafx.h"
#include "CustomGrid.h"
#include <algorithm>
#include <boost/foreach.hpp>
//默认增加列的宽度
const int DEFAULT_COLUMN_WIDTH = 20;
IMPLEMENT_DYNCREATE(CCustomGrid, CBCGPGridCtrl)
CCustomGrid::CCustomGrid():m_nDisplayVitrualRows(60),
m_pCallBack(nullptr)
{
	m_HeaderInfoArray.clear();
}

CCustomGrid::~CCustomGrid()
{
	m_HeaderInfoArray.clear();
}

CBCGPGridColumnsInfo& CCustomGrid::GetColumnsInfo()
{
	return m_ColumnsEx;
}

const CBCGPGridColumnsInfo& CCustomGrid::GetColumnsInfo() const
{
	return m_ColumnsEx;
}

void CCustomGrid::DrawHeaderItem(CDC* pDC, CRect rect, CBCGPHeaderItem* pHeaderItem)
{
	CBCGPGridCtrl::DrawHeaderItem(pDC,rect,pHeaderItem);
}

int CCustomGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	EnableMarkSortedColumn(TRUE, FALSE);
	EnableHeader(TRUE, BCGP_GRID_HEADER_HIDE_ITEMS);
	SetClearInplaceEditOnEnter(FALSE);
	EnableInvertSelOnCtrl();
	SetScrollBarsStyle(CBCGPScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER);
	ShowHorzScrollBar(TRUE);
	ShowVertScrollBar(TRUE);
	EnableRowHeader(TRUE);
	EnableLineNumbers(TRUE);
	SetEditFirstClick(FALSE);
	SetWholeRowSel(TRUE);
	m_ColumnsEx.EnableAutoSize(TRUE);

	if (nullptr != m_pCallBack)
	{
		EnableVirtualMode(m_pCallBack,(LPARAM)this);
		SetVirtualRows(m_nDisplayVitrualRows);
	}

	CreateHeaderInfo();
	CreateColumnInfo();
	AdjustLayout();
	return 0;
}
CRect CCustomGrid::OnGetHeaderRect (CDC* pDC, const CRect& rectDraw)
{
	CRect rect = CBCGPGridCtrl::OnGetHeaderRect(pDC, rectDraw);
	rect.bottom = rect.top + rect.Height() * m_ColumnsEx.GetHeaderLineCount();
	return rect;
}

void CCustomGrid::OnDrawHeader (CDC* pDC)
{
	if (nullptr == pDC)
	{
		return;
	}
	m_ColumnsEx.PrepareDrawHeader();
	CBCGPGridCtrl::OnDrawHeader(pDC);
}

void CCustomGrid::OnPrintHeader(CDC* pDC, CPrintInfo* pInfo)
{
	m_ColumnsEx.PrepareDrawHeader();
	CBCGPGridCtrl::OnPrintHeader(pDC, pInfo);
}

void CCustomGrid::OnPosSizeChanged()
{
	CBCGPGridCtrl::OnPosSizeChanged();
	m_ColumnsEx.ReposHeaderItems();
}
BEGIN_MESSAGE_MAP(CCustomGrid, CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/*******************************************************************
*函数名称:SetHeaderInfoArray
*功能描述:设置列表行头信息(在未创建之前调用)
*输入参数:HeaderInfoArray:表头信息
*输出参数:None
*返回值:None
*作者:xiaowei.han
*日期:2017/05/13 15:56:55
*******************************************************************/
void CCustomGrid::SetHeaderInfoArray(const std::vector<HEADRER_INFO>& HeaderInfoArray)
{
	if (HeaderInfoArray.empty())
	{
		return;
	}
	m_HeaderInfoArray.assign(HeaderInfoArray.begin(),HeaderInfoArray.end());
}

/*******************************************************************
*函数名称:AddHeaderInfo
*功能描述:动态增加表头信息
*输入参数:HeaderInfo:表头信息
*输出参数:None
*返回值:None
*作者:xiaowei.han
*日期:2017/05/13 15:57:53
*******************************************************************/
void CCustomGrid::AddHeaderInfo(const HEADRER_INFO& HeaderInfo)
{
	//参数合法性判断
	if (HeaderInfo.ContainColumnIndexArray.empty() || NULL == GetSafeHwnd())
	{
		return;
	}
	auto Pred = [&HeaderInfo](const HEADRER_INFO& Info)->bool
	{
		if (HeaderInfo.strHeadName == Info.strHeadName)
		{
			return true;
		}
		return false;
	};
	auto Iter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),Pred);
	//如果已经存在
	if (Iter != m_HeaderInfoArray.end())
	{	
		return;
	}
	m_HeaderInfoArray.push_back(HeaderInfo);
	//获取前面列的个数
	int nTotalColumnCounts = m_ColumnsEx.GetColumnCount();
	//需要合并的列信息
	CArray<int, int> MergeColumnsArray;
	MergeColumnsArray.RemoveAll();
	int nColumnIdex = nTotalColumnCounts;
	BOOST_FOREACH(auto& ColumnName,HeaderInfo.ContainColumnIndexArray)
	{

		m_ColumnsEx.InsertColumn(nColumnIdex,ColumnName,DEFAULT_COLUMN_WIDTH);
		m_ColumnsEx.SetColumnWidthAutoSize(nColumnIdex,TRUE);
		MergeColumnsArray.Add(nColumnIdex);
		++nColumnIdex;
	}
	CArray<int, int> MergeLinesArray;
	MergeLinesArray.RemoveAll();
	MergeLinesArray.Add (0);
	m_ColumnsEx.AddHeaderItem (&MergeColumnsArray,&MergeLinesArray,-1,HeaderInfo.strHeadName,HDF_CENTER);
	AdjustLayout();
}

/*******************************************************************
*函数名称:RemoveHeaderInfo
*功能描述:动态移除表头
*输入参数:strHeaderName：待移除的表头名称
*输出参数:None
*返回值:None
*作者:xiaowei.han
*日期:2017/05/13 15:59:47
*******************************************************************/
void CCustomGrid::RemoveHeaderInfo(const CString& strHeaderName)
{
	if (TRUE == strHeaderName.IsEmpty())
	{
		return;
	}
	//不允许全部删除表头
	if (1 == m_HeaderInfoArray.size())
	{
		return;
	}
	//用于查找的lamda表达式
	auto Pred = [&strHeaderName](const HEADRER_INFO& Info)->bool
	{
		if (strHeaderName == Info.strHeadName)
		{
			return true;
		}
		return false;
	};
	auto Iter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),Pred);
	//如果没有找到
	if (Iter == m_HeaderInfoArray.end())
	{
		return;
	}
	//如果找到
	m_HeaderInfoArray.erase(Iter);
	//删除所有列信息
	m_ColumnsEx.RemoveAllHeaderItems();
	m_ColumnsEx.DeleteAllColumns();
	//重新设置列
	CreateHeaderInfo();
	CreateColumnInfo();
	AdjustLayout();
}

/*******************************************************************
*函数名称:RemoveColumn
*功能描述:动态移除列
*输入参数:strColumnName：待移除的列名称
*输出参数:None
*返回值:None
*作者:xiaowei.han
*日期:2017/05/13 15:59:47
*******************************************************************/
void CCustomGrid::RemoveColumn(const CString& strColumnName)
{
	if (TRUE == strColumnName.IsEmpty() || NULL == GetSafeHwnd())
	{
		return;
	}
	//首先删除所有的列
	RemoveAll();
	//查找某列的名字是否等于要删除的
	auto Pred = [&strColumnName](const HEADRER_INFO& Info)->bool
	{
		bool bResult = false;
		for (int i = 0; i < (int)Info.ContainColumnIndexArray.size(); ++i)
		{
			if (Info.ContainColumnIndexArray[i] == strColumnName)
			{
				bResult = true;
				break;
			}
		}
		return bResult;
	};
	

	auto Iter = std::find_if(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end(),Pred);

	if (Iter == m_HeaderInfoArray.end())
	{
		return;
	}

	//找到的话
	auto FindIter = std::find(Iter->ContainColumnIndexArray.begin(),Iter->ContainColumnIndexArray.end(),strColumnName);

	if (FindIter != Iter->ContainColumnIndexArray.end())
	{
		Iter->ContainColumnIndexArray.erase(FindIter);
	}

	m_ColumnsEx.RemoveAllHeaderItems();
	m_ColumnsEx.DeleteAllColumns();
	CreateHeaderInfo();
	CreateColumnInfo();
	AdjustLayout();
}

/*******************************************************************
*函数名称:AddColumnInfo
*功能描述:动态增加列
*输入参数:strColumnName：待增加的列名称 strHeaderName:增加列所在行头的名称
*输出参数:None
*返回值:None
*作者:xiaowei.han
*日期:2017/05/13 15:59:47
*******************************************************************/
void CCustomGrid::AddColumnInfo(const CString& strHeaderName,const CString& strColumnName)
{
	if (strHeaderName.IsEmpty() || strColumnName.IsEmpty())
	{
		return;
	}
	//查找列头
	//首先删除所有的列
	RemoveAll();
	//查找某列的名字是否等于要删除的
	auto HeaderPred = [&strHeaderName](const HEADRER_INFO& Info)->bool
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
		return;
	}


	auto ColumnPred = [&strColumnName](const CString& strName)->bool
	{
		if (strName == strColumnName)
		{
			return true;
		}
		return false;
	};

	auto ColumnIter = std::find_if(HeaderIter->ContainColumnIndexArray.begin(),HeaderIter->ContainColumnIndexArray.end(),ColumnPred);
	//不存在
	if (ColumnIter == HeaderIter->ContainColumnIndexArray.end())
	{
		HeaderIter->ContainColumnIndexArray.push_back(strColumnName);
	}

	//删除所有列信息
	m_ColumnsEx.RemoveAllHeaderItems();
	m_ColumnsEx.DeleteAllColumns();
	//重新设置列
	CreateHeaderInfo();
	CreateColumnInfo();

	AdjustLayout();
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
bool CCustomGrid::IsHeaderNameExist(const CString& strHeaderName)
{
	if (strHeaderName.IsEmpty())
	{
		return false;
	}
	//查找行头信息
	auto HeaderPred = [&strHeaderName](const HEADRER_INFO& Info)->bool
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
bool CCustomGrid::IsColumnExistInFixedHeader(const CString& strHeaderName,const CString& strColumnName)
{
	if (strHeaderName.IsEmpty() || strColumnName.IsEmpty())
	{
		return false;
	}

	//查找某列的名字是否等于要删除的
	auto HeaderPred = [&strHeaderName](const HEADRER_INFO& Info)->bool
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

	auto ColumnPred = [&strColumnName](const CString& strName)->bool
	{
		if (strName == strColumnName)
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

/*******************************************************************
*函数名称:GetHeaderInfo
*功能描述:获取行头信息
*输入参数:None
*输出参数:HeaderInfoArray：保存行头的信息
*返回值:None
*作者:xiaowei.han
*日期:2017/05/13 16:03:08
*******************************************************************/
void CCustomGrid::GetHeaderInfo(std::vector<HEADRER_INFO>& HeaderInfoArray)
{
	HeaderInfoArray.clear();

	HeaderInfoArray.assign(m_HeaderInfoArray.begin(),m_HeaderInfoArray.end());
}


void CCustomGrid::CreateColumnInfo(void)
{

	if (m_HeaderInfoArray.empty())
	{
		return;
	}
	int nColumnIdex = 0;
	BOOST_FOREACH(auto& Element,m_HeaderInfoArray)
	{	
		BOOST_FOREACH(auto& ColumnName,Element.ContainColumnIndexArray)
		{
			m_ColumnsEx.InsertColumn(nColumnIdex,ColumnName,20);
			m_ColumnsEx.SetColumnWidthAutoSize(nColumnIdex,TRUE);
			++nColumnIdex;
		}
	}

}

void CCustomGrid::CreateHeaderInfo(void)
{
	if (m_HeaderInfoArray.empty())
	{
		return;
	}
	m_ColumnsEx.SetHeaderLineCount(2);
	int nColumnIdex = 0;
	//需要合并的列信息
	CArray<int, int> MergeColumnsArray;
	CArray<int, int> MergeLinesArray;
	BOOST_FOREACH(auto& Element,m_HeaderInfoArray)
	{
		//保证多表头的列不为空
		if (!Element.ContainColumnIndexArray.empty())
		{
			MergeColumnsArray.RemoveAll();
			BOOST_FOREACH(auto& ColumnName,Element.ContainColumnIndexArray)
			{
				MergeColumnsArray.Add(nColumnIdex++);
			}
			MergeLinesArray.RemoveAll();
			MergeLinesArray.Add (0);
			m_ColumnsEx.AddHeaderItem (&MergeColumnsArray,&MergeLinesArray,-1,Element.strHeadName,HDF_CENTER);
		}
	}
}

//Grid控件的ID(自增)
unsigned int CCustomGrid::s_GridID = 0;

void CCustomGrid::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);

	if (NULL != GetSafeHwnd())
	{
		int nColumnNum = m_ColumnsEx.GetColumnCount();

		if (nColumnNum > 0)
		{
			AdjustLayout();
			Invalidate(TRUE);	
		}
	}
}




void CCustomGrid::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CBCGPGridCtrl::OnTimer(nIDEvent);
}


BOOL CCustomGrid::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CBCGPGridCtrl::PreTranslateMessage(pMsg);
}
