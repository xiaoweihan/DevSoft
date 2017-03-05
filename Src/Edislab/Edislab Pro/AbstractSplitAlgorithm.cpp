#include "StdAfx.h"
#include "AbstractSplitAlgorithm.h"
#include "Utility.h"
#include "WidgetLayout.h"
CAbstractSplitAlgorithm::CAbstractSplitAlgorithm(void):
m_bWndFront(false),
m_pHostWnd(NULL),
m_pLayout(NULL)
{
	m_WidgetArray.clear();
}


CAbstractSplitAlgorithm::~CAbstractSplitAlgorithm(void)
{
	m_WidgetArray.clear();
}



void CAbstractSplitAlgorithm::RecalculateLayout( int nWidth,int nHeight )
{
	//自适应
	AdjustLayout(nWidth,nHeight);
}



void CAbstractSplitAlgorithm::ChangeLayout( int nWidth,int nHeight )
{
	std::vector<CRect> LayoutSizeArray;

	//计算改变后的位置
	CalculateLayoutSize(nWidth,nHeight,LayoutSizeArray);

	if (LayoutSizeArray.size() == m_WidgetArray.size())
	{
		int nSize = static_cast<int>(LayoutSizeArray.size());

		for (int i = 0; i < nSize; ++i)
		{
			if (NULL != m_WidgetArray[i] && NULL != m_WidgetArray[i]->GetSafeHwnd())
			{
				m_WidgetArray[i]->MoveWindow(&LayoutSizeArray[i]);
				m_WidgetArray[i]->Invalidate(TRUE);
				m_WidgetArray[i]->ShowWindow(SW_SHOW);
			}
		}
	}
}

void CAbstractSplitAlgorithm::SetWidgetArray( const WidgetArray& Array )
{
	m_WidgetArray = Array;
}

void CAbstractSplitAlgorithm::SetWndFront( bool bWndFront )
{
	m_bWndFront = bWndFront;
}

void CAbstractSplitAlgorithm::SetHostWnd( CWnd* pHostWnd )
{
	m_pHostWnd = pHostWnd;
}

MOUSE_STATE CAbstractSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{
	return MOVE_NO_STATE;
}

void CAbstractSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	m_StartPoint = pt;
	//判断位置
	MOUSE_STATE CurrentState = QueryPosStatus(pt);
	switch (CurrentState)
	{
	case MOVE_ALL:
		m_CursorState = IDC_SIZEALL;
		break;
	case MOVE_HOR:
		m_CursorState = IDC_SIZEWE;
		//SetCursor(AfxGetApp()->LoadStandardCursor());
		break;
	case MOVE_VER:
		m_CursorState = IDC_SIZENS;
		//SetCursor(AfxGetApp()->LoadStandardCursor());
		break;
	default:
		break;
	}
	SetCursor(AfxGetApp()->LoadStandardCursor(m_CursorState));
}

void CAbstractSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{

}

void CAbstractSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//判断位置
	MOUSE_STATE CurrentState = QueryPosStatus(pt);
	switch (CurrentState)
	{
	case MOVE_ALL:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
		break;
	case MOVE_HOR:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
		break;
	case MOVE_VER:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
		break;
	default:
		break;
	}
}

void CAbstractSplitAlgorithm::SetLayout( CWidgetLayout* pLayout )
{
	m_pLayout = pLayout;
}

int CAbstractSplitAlgorithm::GetPrexHorSplitBarIndex( int nCurrentIndex )
{
	if (nCurrentIndex <= 0)
	{
		return -1;
	}

	return nCurrentIndex - 1;
}

int CAbstractSplitAlgorithm::GetPrexVerSplitBarIndex( int nCurrentIndex )
{
	if (nCurrentIndex <= 0)
	{
		return -1;
	}

	return nCurrentIndex - 1;
}

void CAbstractSplitAlgorithm::ShowHorSplitBar( bool bShow )
{
	if (NULL == m_pLayout)
	{
		return;
	}

	//获取水平分隔条
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();
	//隐藏分隔条
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		if (bShow)
		{
			pLayout->Invalidate(TRUE);
			pLayout->ShowWindow(SW_SHOW);
		}
		else
		{
			pLayout->ShowWindow(SW_HIDE);
		}	
	}
}

void CAbstractSplitAlgorithm::ShowVerSplitBar( bool bShow )
{
	if (NULL == m_pLayout)
	{
		return;
	}

	//获取垂直分隔条
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	//隐藏分隔条
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		if (bShow)
		{
			pLayout->Invalidate(TRUE);
			pLayout->ShowWindow(SW_SHOW);
		}
		else
		{
			pLayout->ShowWindow(SW_HIDE);
		}	
	}
}
