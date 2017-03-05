#include "stdafx.h"
#include "GridCtrlBase.h"


CGridCtrlBase::CGridCtrlBase(void)
{
}


CGridCtrlBase::~CGridCtrlBase(void)
{
}

BEGIN_MESSAGE_MAP(CGridCtrlBase, CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CBCGPGridRow* CGridCtrlBase::CreateNewRow()
{
	//获取列数
	int nColumnNum = GetColumnCount();

	//创建新的一行
	CBCGPGridRow* pRow = CreateRow(nColumnNum);

	CBCGPGridItem* pItem = NULL;
	if (NULL != pRow)
	{
		for (int i = 0; i < nColumnNum; ++i)
		{
			pItem = pRow->GetItem(i);
			if (NULL != pItem)
			{
				pItem->AllowEdit (FALSE);
			}	
		}
	}
	return pRow;
}

int CGridCtrlBase::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	EnableMarkSortedColumn(FALSE);
	EnableHeader(TRUE,BCGP_GRID_HEADER_SELECT);
	EnableRowHeader(TRUE );
	EnableLineNumbers(TRUE);
	SetClearInplaceEditOnEnter(FALSE);
	EnableInvertSelOnCtrl (FALSE);
	SetScrollBarsStyle (CBCGPScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER);
	SetSingleSel(TRUE);
	SetGridItemReadOnly(true);
	//设置列头
	CRect rc;
	GetClientRect(rc);
	int nColumnNum = static_cast<int>(m_ColumnTitleArray.GetSize());
	for (int i = 0; i < nColumnNum; ++i)
	{
		InsertColumn (i,m_ColumnTitleArray[i], rc.Width() / nColumnNum);
	}

	return 0;
}

void CGridCtrlBase::OnLButtonDown( UINT nFlags, CPoint point )
{
	if (FALSE == m_bReadOnly)
	{
		SetGridItemReadOnly(true);
	}

	CBCGPGridCtrl::OnLButtonDown(nFlags, point);
}

void CGridCtrlBase::OnLButtonDblClk( UINT nFlags, CPoint point )
{
	SetGridItemReadOnly(false);
	CBCGPGridCtrl::OnLButtonDblClk(nFlags, point);
}

void CGridCtrlBase::OnContextMenu( CWnd* pWnd, CPoint point )
{
	CBCGPGridCtrl::OnContextMenu(pWnd, point);
}

/************************************************************************************************************
函数名称:AddColumnTitle
功能描述:创建责任区
输入参数:strColumTitle:列的名称  nIndex:要插入的索引
输出参数:无
返回值:无
************************************************************************************************************/
void CGridCtrlBase::AddColumnTitle( const CString& strColumTitle,int nIndex /*= -1*/ )
{
	if (-1 == nIndex)
	{
		m_ColumnTitleArray.Add(strColumTitle);
	}
	else
	{
		m_ColumnTitleArray.InsertAt(nIndex,strColumTitle);
	}
}

void CGridCtrlBase::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	int nColumnNum = GetColumnCount();

	//设置列宽
	for (int i = 0; i < nColumnNum; ++i)
	{
		SetColumnWidth(i,(cx - 30) / nColumnNum);
	}
}

/************************************************************************************************************
函数名称:OnItemChanged
功能描述:Grid项改变处理函数
输入参数:无
输出参数:无
返回值:无
************************************************************************************************************/
void CGridCtrlBase::OnItemChanged( CBCGPGridItem*pItem,int nRow,int nColumn )
{
	//参数合法性判断
	if (NULL != pItem)
	{
		//发送消息告诉父窗口
		CWnd* pParentWnd = GetParent();

		if (NULL != pParentWnd)
		{
			//pParentWnd->SendMessage(WM_NOTIFY_GRID_ITEM_CHANGE_MSG,(WPARAM)nRow,(LPARAM)nColumn);
		}
	}
}

/************************************************************************************************************
函数名称:OnLButtonUp
功能描述:响应左键弹起
输入参数:strColumTitle:列的名称  nIndex:要插入的索引
输出参数:无
返回值:无
************************************************************************************************************/
void CGridCtrlBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	//首先判断行头是否选中
	CBCGPGridRow* pRow = HitTestRowHeader(point);

	if (NULL != pRow)
	{
		if (TRUE == pRow->IsSelected())
		{
			//发送消息告诉父窗口
			CWnd* pParentWnd = GetParent();

			if (NULL != pParentWnd)
			{
				//pParentWnd->SendMessage(WM_NOTIFY_GRID_SELECT_ROW_MSG,(WPARAM)(pRow->GetRowId()),0);	
			}
		}
		CBCGPGridCtrl::OnLButtonUp(nFlags, point);
		return;
	}
	//判断是否选中某一行
	CBCGPGridItemID ItemID;
	CBCGPGridItem* pItem = NULL;
	pRow = HitTest(point,ItemID,pItem);

	//如果选中某一行
	if (NULL != pRow)
	{
		if (TRUE == pRow->IsSelected())
		{
			//发送消息告诉父窗口
			CWnd* pParentWnd = GetParent();

			//选中了某一项才发送消息出去
			if (NULL != pItem)
			{
				if (NULL != pParentWnd)
				{
					//pParentWnd->SendMessage(WM_NOTIFY_GRID_SELECT_ROW_MSG,(WPARAM)(ItemID.m_nRow),0);
				}	
			}
		}
	}
	CBCGPGridCtrl::OnLButtonUp(nFlags, point);
}

/************************************************************************************************************
函数名称:SetGridItemReadOnly
功能描述:设置Grid是否可读
输入参数:bReadOnly:true:可读 false:不可读
输出参数:无
返回值:无
************************************************************************************************************/
void CGridCtrlBase::SetGridItemReadOnly( bool bReadOnly )
{
	if (bReadOnly)
	{
		m_bReadOnly = TRUE;
	}
	else
	{
		m_bReadOnly = FALSE;
	}
}

int CGridCtrlBase::s_nGridID = 10000;