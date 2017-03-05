#include "StdAfx.h"
#include "ConcreteSplitAlgorithm.h"
#include "WidgetLayout.h"
#include "Utility.h"
const int DEFAULT_GAP = 4;
COneWidgetSplitAlgorithm::COneWidgetSplitAlgorithm(void)
{
}


COneWidgetSplitAlgorithm::~COneWidgetSplitAlgorithm(void)
{
}

/*****************************************************************************
@FunctionName : AdjustLayout
@FunctionDescription : 自适应布局
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void COneWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{

	if (m_WidgetArray.empty())
	{
		return;
	}

	//移动和父窗口一样大
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget)
	{
		pWidget->MoveWindow(0,0,nWidth,nHeight,TRUE);
		pWidget->ShowWindow(SW_SHOW);
	}

}

void COneWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	CRect WidgetRect;

	WidgetRect.left = 0;
	WidgetRect.top = 0;
	WidgetRect.right = nWidth;
	WidgetRect.bottom = nHeight;

	LayoutSizeArray.push_back(WidgetRect);
}

CTwoWidgetSplitAlgorithm::CTwoWidgetSplitAlgorithm( void )
{	
}

CTwoWidgetSplitAlgorithm::~CTwoWidgetSplitAlgorithm( void )
{
}

void CTwoWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 2)
	{
		return;
	}

	CRect rect1;
	rect1.left = 0;
	rect1.top = 0;
	rect1.right = rect1.left + (nWidth - DEFAULT_GAP) / 2;
	rect1.bottom = rect1.top + nHeight;

	CWnd* pWidget = m_WidgetArray[0];

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rect1,TRUE);
		pWidget->ShowWindow(SW_SHOW);
	}

	CRect rect2;
	rect2.left = rect1.right + DEFAULT_GAP;
	rect2.top = rect1.top;
	rect2.right = nWidth;
	rect2.bottom = rect1.bottom;

	pWidget = m_WidgetArray[1];

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rect2,TRUE);
		pWidget->ShowWindow(SW_SHOW);
	}

	//重新计算分隔条的位置
	CRect SplitRect;
	SplitRect.left = (nWidth - DEFAULT_GAP) / 2;
	SplitRect.top = 0;
	SplitRect.right = SplitRect.left + DEFAULT_GAP;
	SplitRect.bottom = SplitRect.top + nHeight;
	m_HorSplitBarRect[0] = SplitRect;
}

void CTwoWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	CRect WidgetRect1;

	//计算第一个widget的的窗口位置
	WidgetRect1.left = 0;
	WidgetRect1.top = m_HorSplitBarRect[0].top;
	WidgetRect1.right = m_HorSplitBarRect[0].left;
	WidgetRect1.bottom = m_HorSplitBarRect[0].bottom;

	LayoutSizeArray.push_back(WidgetRect1);


	CRect WidgetRect2;

	//计算第二个widget的的窗口位置
	WidgetRect2.left = m_HorSplitBarRect[0].right;
	WidgetRect2.top = m_HorSplitBarRect[0].top;
	WidgetRect2.right = nWidth;
	WidgetRect2.bottom = m_HorSplitBarRect[0].bottom;

	LayoutSizeArray.push_back(WidgetRect2);
}

MOUSE_STATE CTwoWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{

	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}
	

	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CTwoWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CTwoWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	ClipCursor(NULL);
	//隐藏分隔条
	ShowHorSplitBar(false);
	ShowVerSplitBar(false);
	//计算拖动分隔条之后的布局
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//重新计算交叉点位置
	ReCalculateIntersetRect();
	//改变布局
	ChangeLayout(nWidth,nHeight);
}

void CTwoWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{

	//左键移动
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			int nXMove = pt.x - m_StartPoint.x;
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			int nYMove = pt.y - m_StartPoint.y;
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			int nYMove = pt.y - m_StartPoint.y;
			int nXMove = pt.x - m_StartPoint.x;
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}
	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CTwoWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	//参数合法性判断
	if (NULL == m_pLayout)
	{
		return;
	}
	//获取拖动的分割调的位置
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	//进行位置X轴方向上的偏移
	rc.OffsetRect(nMoveDistance,0);

	RECT Rect;
	Rect.left = 0 + DEFAULT_GAP;
	Rect.top = 0;
	Rect.bottom = nHeight;
	Rect.right = nWidth - DEFAULT_GAP;

	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&Rect);
		ClipCursor(&Rect);
	}
	//获取水平分隔条
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		//移动分隔条到相应位置
		pLayout->MoveWindow(&rc,TRUE);
		//刷新
		pLayout->Invalidate(TRUE);
		//显示分隔条
		pLayout->ShowWindow(SW_SHOW);
	}		
}

void CTwoWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{
	//计算分隔条的矩形
	m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
}

CThreeWidgetSplitAlgorithm::CThreeWidgetSplitAlgorithm( void )
{

}

CThreeWidgetSplitAlgorithm::~CThreeWidgetSplitAlgorithm( void )
{

}

void CThreeWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 3)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}


}

void CThreeWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP) / 2;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP) / 2;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}
	CRect rc2;
	rc2.left = rc1.left;
	rc2.top = rc1.bottom + DEFAULT_GAP;
	rc2.right = rc2.left + (nWidth - DEFAULT_GAP) / 2;
	rc2.bottom = nHeight;
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}
	pWidget = m_WidgetArray[2];
	CRect rc3;
	rc3.left = rc1.right + DEFAULT_GAP;
	rc3.top = rc1.top;
	rc3.right = nWidth;
	rc3.bottom = nHeight;

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}


	//第一个分隔条位置
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc3.left;
	m_HorSplitBarRect[0].bottom = rc3.bottom;

	//第二个分隔条位置
	m_VerSplitBarRect[0].left = rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = rc1.right;
	m_VerSplitBarRect[0].bottom = rc2.top;

}

void CThreeWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP) / 2;
	rc1.bottom = nHeight;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}


	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = nWidth;
	rc2.bottom = rc2.top + (nHeight - DEFAULT_GAP) / 2;
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	pWidget = m_WidgetArray[2];
	CRect rc3;
	rc3.left = rc2.left;
	rc3.top = rc2.bottom + DEFAULT_GAP;
	rc3.right = rc2.right;
	rc3.bottom = nHeight;

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//第一个分隔条位置
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = m_HorSplitBarRect[0].left + DEFAULT_GAP;
	m_HorSplitBarRect[0].bottom = m_HorSplitBarRect[0].top + rc1.Width();

	//第二个分隔条位置
	m_VerSplitBarRect[0].left = rc2.left;
	m_VerSplitBarRect[0].top = rc2.bottom;
	m_VerSplitBarRect[0].right = nWidth;
	m_VerSplitBarRect[0].bottom = rc3.top;
}

void CThreeWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	if (m_bWndFront)
	{

		//第一个窗口位置
		CRect WidgetRect1;
		WidgetRect1.left = 0;
		WidgetRect1.top = m_HorSplitBarRect[0].top;
		WidgetRect1.right = m_HorSplitBarRect[0].left;
		WidgetRect1.bottom = m_HorSplitBarRect[0].bottom;

		LayoutSizeArray.push_back(WidgetRect1);

		//第二个窗口位置
		CRect WidgetRect2;
		WidgetRect2.left = m_HorSplitBarRect[0].right;
		WidgetRect2.top = m_HorSplitBarRect[0].top;
		WidgetRect2.right = nWidth;
		WidgetRect2.bottom = m_VerSplitBarRect[0].top;

		LayoutSizeArray.push_back(WidgetRect2);

		//第三个窗口位置
		CRect WidgetRect3;
		WidgetRect3.left = m_HorSplitBarRect[0].right;
		WidgetRect3.top = m_VerSplitBarRect[0].bottom;
		WidgetRect3.right = nWidth;
		WidgetRect3.bottom = nHeight;

		LayoutSizeArray.push_back(WidgetRect3);
	}
	else
	{
		//第一个窗口位置
		CRect WidgetRect1;
		WidgetRect1.left = 0;
		WidgetRect1.top = 0;
		WidgetRect1.right = m_HorSplitBarRect[0].left;
		WidgetRect1.bottom = m_VerSplitBarRect[0].top;

		LayoutSizeArray.push_back(WidgetRect1);

		//第二个窗口位置
		CRect WidgetRect2;
		WidgetRect2.left = m_VerSplitBarRect[0].left;
		WidgetRect2.top = m_VerSplitBarRect[0].bottom;
		WidgetRect2.right = m_HorSplitBarRect[0].left;
		WidgetRect2.bottom = nHeight;

		LayoutSizeArray.push_back(WidgetRect2);

		//第三个窗口位置
		CRect WidgetRect3;
		WidgetRect3.left = m_HorSplitBarRect[0].right;
		WidgetRect3.top = m_HorSplitBarRect[0].top;
		WidgetRect3.right = nWidth;
		WidgetRect3.bottom = nHeight;

		LayoutSizeArray.push_back(WidgetRect3);

	}
}

MOUSE_STATE CThreeWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{

	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}
	

	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}
	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CThreeWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CThreeWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	ClipCursor(NULL);
	if (NULL == m_pLayout)
	{
		return;
	}
	//如果是水平移动
	if (MOVE_HOR == m_SelectSplitBarInfo.Type)
	{
		//获取水平移动的像素
		int nXMove = pt.x - m_StartPoint.x;
		//计算分隔条的矩形
		m_HorSplitBarRect[m_SelectSplitBarInfo.nIndex].OffsetRect(nXMove,0);
		//获取水平分隔条
		CWnd* pHorSplit = m_pLayout->GetMoveHorSplitBar();
		if (NULL != pHorSplit && NULL != pHorSplit->GetSafeHwnd())
		{
			pHorSplit->ShowWindow(SW_HIDE);
		}

		//如果大窗口在前面
		if (m_bWndFront)
		{
			//修改关联的垂直分隔条
			m_VerSplitBarRect[0].left += (nXMove);
		}
		//如果大窗口在后面
		else
		{
			//修改关联的垂直分隔条
			m_VerSplitBarRect[0].right += (nXMove);
		}
	}
	//如果是垂直移动
	if (MOVE_VER == m_SelectSplitBarInfo.Type)
	{
		//获取垂直移动的像素
		int nYMove = pt.y - m_StartPoint.y;
		//计算分隔条的矩形
		m_VerSplitBarRect[m_SelectSplitBarInfo.nIndex].OffsetRect(0,nYMove);
		CWnd* pVerSplit = m_pLayout->GetMoveVerSplitBar();
		if (NULL != pVerSplit && NULL != pVerSplit->GetSafeHwnd())
		{
			pVerSplit->ShowWindow(SW_HIDE);
		}	
	}
	//重新计算布局
	ChangeLayout(nWidth,nHeight);	
}

void CThreeWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//左键移动
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			int nXMove = pt.x - m_StartPoint.x;
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			int nYMove = pt.y - m_StartPoint.y;
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CThreeWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}

	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);


	RECT Rect;
	Rect.left = 0 + DEFAULT_GAP;
	Rect.top = 0;
	Rect.bottom = nHeight;
	Rect.right = nWidth - DEFAULT_GAP;

	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&Rect);
		ClipCursor(&Rect);
	}

	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}		
}

void CThreeWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}

	CRect rc = m_VerSplitBarRect[nCurrentIndex];
	rc.OffsetRect(0,nMoveDistance);

	RECT Rect;
	Rect.left = 0;
	Rect.top = DEFAULT_GAP;
	Rect.bottom = nHeight - DEFAULT_GAP;
	Rect.right = nWidth;

	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&Rect);
		ClipCursor(&Rect);
	}

	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	
}

CFourWidgetSplitAlgorithm::CFourWidgetSplitAlgorithm( void )
{
}

CFourWidgetSplitAlgorithm::~CFourWidgetSplitAlgorithm( void )
{
}

void CFourWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 4)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CFourWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 3) / 4;
	rc1.bottom = nHeight;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + (nWidth - DEFAULT_GAP * 3) / 4;
	rc2.bottom = nHeight;
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	pWidget = m_WidgetArray[2];
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc2.top;
	rc3.right = rc3.left + (nWidth - DEFAULT_GAP * 3) / 4 ;
	rc3.bottom = nHeight;

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	pWidget = m_WidgetArray[3];
	CRect rc4;
	rc4.left = rc3.right + DEFAULT_GAP;
	rc4.top = rc3.top;
	rc4.right = nWidth;
	rc4.bottom = nHeight;

	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//计算分隔条位置
	//第一个分隔条
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc2.left;
	m_HorSplitBarRect[0].bottom = rc1.bottom;

	//第二个分隔条
	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = rc3.left;
	m_HorSplitBarRect[1].bottom = rc2.bottom;

	//第三个分隔条
	m_HorSplitBarRect[2].left = rc3.right;
	m_HorSplitBarRect[2].top = rc3.top;
	m_HorSplitBarRect[2].right = rc4.left;
	m_HorSplitBarRect[2].bottom = rc3.bottom;
}

void CFourWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{
	AdjustLayoutFront(nWidth,nHeight);
}

void CFourWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	//第一个Widget窗口位置
	CRect WidgetRect1;
	WidgetRect1.left = 0;
	WidgetRect1.top = m_HorSplitBarRect[0].top;
	WidgetRect1.right = m_HorSplitBarRect[0].left;
	WidgetRect1.bottom = nHeight;
	LayoutSizeArray.push_back(WidgetRect1);

	//第二个Widget窗口位置
	CRect WidgetRect2;
	WidgetRect2.left = m_HorSplitBarRect[0].right;
	WidgetRect2.top = m_HorSplitBarRect[0].top;
	WidgetRect2.right = m_HorSplitBarRect[1].left;
	WidgetRect2.bottom = nHeight;
	LayoutSizeArray.push_back(WidgetRect2);

	//第三个Widget窗口位置
	CRect WidgetRect3;
	WidgetRect3.left = m_HorSplitBarRect[1].right;
	WidgetRect3.top = m_HorSplitBarRect[1].top;
	WidgetRect3.right = m_HorSplitBarRect[2].left;
	WidgetRect3.bottom = nHeight;
	LayoutSizeArray.push_back(WidgetRect3);

	//第四个Widget窗口位置
	CRect WidgetRect4;
	WidgetRect4.left = m_HorSplitBarRect[2].right;
	WidgetRect4.top = m_HorSplitBarRect[2].top;
	WidgetRect4.right = nWidth;
	WidgetRect4.bottom = nHeight;
	LayoutSizeArray.push_back(WidgetRect4);

}

MOUSE_STATE CFourWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{

	
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CFourWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CFourWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{

	if (MOVE_HOR == m_SelectSplitBarInfo.Type)
	{
		int nXMove = pt.x - m_StartPoint.x;

		m_HorSplitBarRect[m_SelectSplitBarInfo.nIndex].OffsetRect(nXMove,0);

		if (NULL != m_pLayout)
		{
			CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

			if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
			{
				pLayout->ShowWindow(SW_HIDE);
			}
		}
		CalculateNextSplitBar(m_SelectSplitBarInfo.nIndex,nXMove,nWidth,nHeight);
	}

	ChangeLayout(nWidth,nHeight);

	ClipCursor(NULL);
}

void CFourWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//左键移动
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			int nXMove = pt.x - m_StartPoint.x;

			//处理水平移动
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}
	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CFourWidgetSplitAlgorithm::CalculateNextSplitBar( int nCurrentIndex,int nXMove,int nWidth,int nHeight )
{
	//判断其它影响的分割栏
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		//在选择的后面
		if (i > nCurrentIndex)
		{
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
}

void CFourWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}

	CRect rc = m_HorSplitBarRect[nCurrentIndex];

	rc.OffsetRect(nMoveDistance,0);

	//获取前一个窗口的索引
	int nPrexIndex = GetPrexHorSplitBarIndex(m_SelectSplitBarInfo.nIndex);

	//限制鼠标范围
	RECT CursorRect;
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	else
	{
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		ClipCursor(&CursorRect);
	}

	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	
}


CFiveWidgetSplitAlgorithm::CFiveWidgetSplitAlgorithm( void )
{

}

CFiveWidgetSplitAlgorithm::~CFiveWidgetSplitAlgorithm( void )
{

}

void CFiveWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 5)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CFiveWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{

	//移动第一个窗口位置
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc1.bottom = nHeight;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第二个窗口位置
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc2.bottom = rc2.top + (nHeight - DEFAULT_GAP) / 2;
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第三个窗口位置
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc2.top;
	rc3.right = rc3.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc3.bottom = rc3.top + (nHeight - DEFAULT_GAP) / 2;
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第四个窗口位置
	CRect rc4;
	rc4.left = rc2.left;
	rc4.top = rc2.bottom + DEFAULT_GAP;
	rc4.right = rc4.left + rc2.Width();
	rc4.bottom = rc4.top + rc2.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第5个窗口位置
	CRect rc5;
	rc5.left = rc4.right + DEFAULT_GAP;
	rc5.top = rc4.top;
	rc5.right = rc5.left + rc4.Width();
	rc5.bottom = rc5.top + rc4.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}


	//计算分隔条的位置
	//第一个分隔条
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc2.left;
	m_HorSplitBarRect[0].bottom = rc1.bottom;

	//第二个分隔条
	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = rc3.left;
	m_HorSplitBarRect[1].bottom = rc1.bottom;

	//第三个分隔条
	m_VerSplitBarRect[0].left = rc2.left;
	m_VerSplitBarRect[0].top = rc2.bottom;
	m_VerSplitBarRect[0].right = nWidth;
	m_VerSplitBarRect[0].bottom = rc4.top;

	//计算交点位置
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 1;
	m_IntersectRect[0].nVerIndex = 0;

}

void CFiveWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{

	//移动第一个窗口位置
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP) / 2;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第二个窗口位置
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + rc1.Height();
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第三个窗口位置
	CRect rc3;
	rc3.left = rc1.left;
	rc3.top = rc1.bottom + DEFAULT_GAP;
	rc3.right = rc3.left + rc1.Width();
	rc3.bottom = rc3.top + rc1.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第四个窗口位置
	CRect rc4;
	rc4.left = rc3.right + DEFAULT_GAP;
	rc4.top = rc3.top;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第5个窗口位置
	CRect rc5;
	rc5.left = rc2.right + DEFAULT_GAP;
	rc5.top = rc2.top;
	rc5.right = nWidth;
	rc5.bottom = nHeight;
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}


	//计算分隔条的位置
	//第一个分隔条
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc2.left;
	m_HorSplitBarRect[0].bottom = nHeight;

	//第二个分隔条
	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = rc5.left;
	m_HorSplitBarRect[1].bottom = nHeight;

	//第三个分隔条
	m_VerSplitBarRect[0].left = rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = rc4.right;
	m_VerSplitBarRect[0].bottom = rc4.top;


	//计算交点位置
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 0;
	m_IntersectRect[0].nVerIndex = 0;
}

void CFiveWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{

	LayoutSizeArray.clear();

	//大窗口在前面
	if (m_bWndFront)
	{
		CRect Widget1;
		Widget1.left = 0;
		Widget1.top = m_HorSplitBarRect[0].top;
		Widget1.right = m_HorSplitBarRect[0].left;
		Widget1.bottom = m_HorSplitBarRect[0].bottom;
		LayoutSizeArray.push_back(Widget1);

		CRect Widget2;
		Widget2.left = m_HorSplitBarRect[0].right;
		Widget2.top = m_HorSplitBarRect[0].top;
		Widget2.right = m_HorSplitBarRect[1].left;
		Widget2.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget2);

		CRect Widget3;
		Widget3.left = m_HorSplitBarRect[1].right;
		Widget3.top = m_HorSplitBarRect[1].top;
		Widget3.right = nWidth;
		Widget3.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget3);

		CRect Widget4;
		Widget4.left = m_HorSplitBarRect[0].right;
		Widget4.top = m_VerSplitBarRect[0].bottom;
		Widget4.right = m_HorSplitBarRect[1].left;
		Widget4.bottom = nHeight;
		LayoutSizeArray.push_back(Widget4);

		CRect Widget5;
		Widget5.left = m_HorSplitBarRect[1].right;
		Widget5.top = m_VerSplitBarRect[0].bottom;
		Widget5.right = nWidth;
		Widget5.bottom = nHeight;
		LayoutSizeArray.push_back(Widget5);

	}
	else
	{
		CRect Widget1;
		Widget1.left = 0;
		Widget1.top = m_HorSplitBarRect[0].top;
		Widget1.right = m_HorSplitBarRect[0].left;
		Widget1.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget1);

		CRect Widget2;
		Widget2.left = m_HorSplitBarRect[0].right;
		Widget2.top = m_HorSplitBarRect[0].top;
		Widget2.right = m_HorSplitBarRect[1].left;
		Widget2.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget2);

		CRect Widget3;
		Widget3.left = m_VerSplitBarRect[0].left;
		Widget3.top = m_VerSplitBarRect[0].bottom;
		Widget3.right = m_HorSplitBarRect[0].left;
		Widget3.bottom = nHeight;
		LayoutSizeArray.push_back(Widget3);

		CRect Widget4;
		Widget4.left = m_HorSplitBarRect[0].right;
		Widget4.top = m_VerSplitBarRect[0].bottom;
		Widget4.right = m_HorSplitBarRect[1].left;
		Widget4.bottom = nHeight;
		LayoutSizeArray.push_back(Widget4);

		CRect Widget5;
		Widget5.left = m_HorSplitBarRect[1].right;
		Widget5.top = m_HorSplitBarRect[1].top;
		Widget5.right = nWidth;
		Widget5.bottom = nHeight;
		LayoutSizeArray.push_back(Widget5);

	}
}

MOUSE_STATE CFiveWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{
	
#if 0
	//窗口在前
	if (m_bWndFront)
	{
		for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
		{
			if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_ALL;
				return MOVE_ALL;
			}
		}

		//判断是否可以左右移动
		for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
		{
			if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_HOR;
				return MOVE_HOR;
			}
		}

		for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
		{
			if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_VER;
				return MOVE_VER;
			}
		}
	}
	//窗口在后
	else
	{
		for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
		{
			if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_ALL;
				return MOVE_ALL;
			}
		}

		//判断是否可以左右移动
		nNum = sizeof(m_SplitBarRect) / sizeof(m_SplitBarRect[0]);

		for (int i = 0; i < nNum - 1; ++i)
		{
			if (TRUE == m_SplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_HOR;
				return MOVE_HOR;
			}
		}


		if (TRUE == m_SplitBarRect[nNum - 1].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = nNum - 1;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}
#endif

	for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
	{
		if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_ALL;
			return MOVE_ALL;
		}
	}

	//判断是否可以左右移动
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	//是否可以上下移动
	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}
	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CFiveWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CFiveWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//取消光标限制
	ClipCursor(NULL);
	//参数合法性判断
	if (NULL == m_pLayout)
	{
		return;
	}
	//隐藏水平分隔条
	ShowHorSplitBar(false);
	//隐藏垂直分隔条
	ShowVerSplitBar(false);
	//进行坐标转换
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//重新计算交叉点位置
	ReCalculateIntersetRect();
	//重新改变布局
	ChangeLayout(nWidth,nHeight);
}

void CFiveWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{

	//左键移动
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			int nXMove = pt.x - m_StartPoint.x;

			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			int nYMove = pt.y - m_StartPoint.y;

			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			int nYMove = pt.y - m_StartPoint.y;
			int nXMove = pt.x - m_StartPoint.x;
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CFiveWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{

	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);


	//获取前一个窗口的索引
	int nPrexIndex = GetPrexHorSplitBarIndex(nCurrentIndex);

	//限制鼠标范围
	RECT CursorRect;
	//如果有前一个分隔条
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//如果没有分隔条
	else
	{
		CursorRect.left = DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}
	//获取水平分隔栏
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	
}

void CFiveWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}

	CRect rc = m_VerSplitBarRect[nCurrentIndex];

	rc.OffsetRect(0,nMoveDistance);

	//限制鼠标范围
	RECT CursorRect;
	CursorRect.left = 0;
	CursorRect.top = DEFAULT_GAP;
	CursorRect.bottom = nHeight - DEFAULT_GAP;
	CursorRect.right = nWidth;
	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CFiveWidgetSplitAlgorithm::HandleHorAndVerMove( int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect HorRect = *(m_IntersectRect[nCurrentIndex].pRect1);
	CRect VerRect = *(m_IntersectRect[nCurrentIndex].pRect2);
	HorRect.OffsetRect(nMoveX,0);
	VerRect.OffsetRect(0,nMoveY);

	//大窗口在前面
	if (m_bWndFront)
	{
		//限制鼠标范围
		RECT CursorRect;
		CursorRect.left = m_HorSplitBarRect[0].right + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
		//客户区左边转换为桌面坐标
		if (NULL != m_pHostWnd)
		{
			m_pHostWnd->ClientToScreen(&CursorRect);
			//限制光标
			ClipCursor(&CursorRect);
		}
	}
	else
	{
		//限制鼠标范围
		RECT CursorRect;
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = m_HorSplitBarRect[1].left - DEFAULT_GAP;
		//客户区左边转换为桌面坐标
		if (NULL != m_pHostWnd)
		{
			m_pHostWnd->ClientToScreen(&CursorRect);
			//限制光标
			ClipCursor(&CursorRect);
		}
	}


	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&VerRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}

	pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&HorRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	
}

void CFiveWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{

	//大窗口在后面
	if (m_bWndFront)
	{
		//如果是第一个位置
		if (0 == nCurrentIndex)
		{
			m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
			//垂直分隔条位置要改变
			m_VerSplitBarRect[0].left += nXMove;
			//计算后面的位置
			m_HorSplitBarRect[1].OffsetRect(nXMove,0);
		}
		else
		{
			m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
		}
	}
	else
	{
		//判断是否是最后一个位置
		if (nCurrentIndex == 1)
		{
			m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
			m_VerSplitBarRect[0].right += nXMove;
		}
		//如果不是最后一个
		else
		{
			m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
			//计算后面的位置
			m_HorSplitBarRect[1].OffsetRect(nXMove,0);
			m_VerSplitBarRect[0].right += nXMove;
		}
	}
}

void CFiveWidgetSplitAlgorithm::CalculateCoordinateAfterVerMove( int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_VerSplitBarRect[nCurrentIndex].OffsetRect(0,nYMove);
}

void CFiveWidgetSplitAlgorithm::CalculateCoordinateAfterHorAndVerMove( int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_IntersectRect[nCurrentIndex].pRect1->OffsetRect(nXMove,0);
	m_IntersectRect[nCurrentIndex].pRect2->OffsetRect(0,nYMove);
}

void CFiveWidgetSplitAlgorithm::ReCalculateIntersetRect( void )
{
	//重新计算交叉点
	if (m_bWndFront)
	{
		m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	}
	else
	{
		m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_HorSplitBarRect[0]);
	}
}

CSixWidgetSplitAlgorithm::CSixWidgetSplitAlgorithm( void )
{

}

CSixWidgetSplitAlgorithm::~CSixWidgetSplitAlgorithm( void )
{

}

void CSixWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 6)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CSixWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{

	//移动第一个窗口位置
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP) / 2;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第二个窗口位置
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + rc1.Height();
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第三个窗口位置
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc1.top;
	rc3.right = rc3.left + rc1.Width();
	rc3.bottom = rc3.top + rc1.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第四个窗口位置
	CRect rc4;
	rc4.left = rc1.left;
	rc4.top = rc1.bottom + DEFAULT_GAP;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第5个窗口位置
	CRect rc5;
	rc5.left = rc4.right + DEFAULT_GAP;
	rc5.top = rc4.top;
	rc5.right = rc5.left + rc4.Width();
	rc5.bottom = rc5.top + rc4.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第6个窗口位置
	CRect rc6;
	rc6.left = rc5.right + DEFAULT_GAP;
	rc6.top = rc5.top;
	rc6.right = rc6.left + rc5.Width();
	rc6.bottom = rc6.top + rc5.Height();
	pWidget = m_WidgetArray[5];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc6);
		pWidget->ShowWindow(SW_SHOW);
	}


	//计算分隔条位置
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc2.left;
	m_HorSplitBarRect[0].bottom = rc4.bottom;

	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = rc3.left;
	m_HorSplitBarRect[1].bottom = rc4.bottom;

	m_VerSplitBarRect[0].left = rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = rc3.right;
	m_VerSplitBarRect[0].bottom = rc4.top;

	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 0;
	m_IntersectRect[0].nVerIndex = 0;

	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[1].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[1].nHorIndex = 1;
	m_IntersectRect[1].nVerIndex = 0;

}

void CSixWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{
	AdjustLayoutFront(nWidth,nHeight);
}

void CSixWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	CRect Widget1;
	Widget1.left = 0;
	Widget1.top = m_HorSplitBarRect[0].top;
	Widget1.right = m_HorSplitBarRect[0].left;
	Widget1.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget1);

	CRect Widget2;
	Widget2.left = m_HorSplitBarRect[0].right;
	Widget2.top = m_HorSplitBarRect[0].top;
	Widget2.right = m_HorSplitBarRect[1].left;
	Widget2.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget2);

	CRect Widget3;
	Widget3.left = m_HorSplitBarRect[1].right;
	Widget3.top = m_HorSplitBarRect[1].top;
	Widget3.right = nWidth;
	Widget3.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget3);

	CRect Widget4;
	Widget4.left = 0;
	Widget4.top = m_VerSplitBarRect[0].bottom;
	Widget4.right = m_HorSplitBarRect[0].left;
	Widget4.bottom = nHeight;
	LayoutSizeArray.push_back(Widget4);

	CRect Widget5;
	Widget5.left = m_HorSplitBarRect[0].right;
	Widget5.top = m_VerSplitBarRect[0].bottom;
	Widget5.right = m_HorSplitBarRect[1].left;
	Widget5.bottom = nHeight;
	LayoutSizeArray.push_back(Widget5);

	CRect Widget6;
	Widget6.left = m_HorSplitBarRect[1].right;
	Widget6.top = m_VerSplitBarRect[0].bottom;
	Widget6.right = nWidth;
	Widget6.bottom = nHeight;
	LayoutSizeArray.push_back(Widget6);
}

MOUSE_STATE CSixWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{
	
	//判断是否鼠标位置在交叉点
	for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
	{
		if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_ALL;
			return MOVE_ALL;
		}
	}

	//判断是否可以左右移动
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	//判断是否可以垂直移动
	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}
	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

/*****************************************************************************
@FunctionName : 处理鼠标左键按下消息
@FunctionDescription : 处理鼠标左键按下消息
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

/*****************************************************************************
@FunctionName : 处理鼠标左键弹起消息
@FunctionDescription : 处理鼠标左键弹起消息
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//取消鼠标移动范围
	ClipCursor(NULL);
	//隐藏拖动条
	ShowHorSplitBar(false);
	ShowVerSplitBar(false);
	//计算拖动分隔条之后的布局
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//重新计算交叉点位置
	ReCalculateIntersetRect();
	//改变布局
	ChangeLayout(nWidth,nHeight);
}

/*****************************************************************************
@FunctionName : 处理鼠标移动消息
@FunctionDescription : 处理鼠标移动消息
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{

	//左键按下移动鼠标
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		//如果是水平移动
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			//计算水平方向移动的像素数
			int nXMove = pt.x - m_StartPoint.x;

			//处理水平移动
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//如果是垂直方向移动
		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			//计算垂直方向移动的像素数
			int nYMove = pt.y - m_StartPoint.y;

			//处理垂直移动
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//如果是垂直与水平移动
		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			//计算垂直移动的像素数
			int nYMove = pt.y - m_StartPoint.y;
			//计算水平移动的像素数
			int nXMove = pt.x - m_StartPoint.x;
			//计算垂直与水平移动的像素数
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

/*****************************************************************************
@FunctionName : 处理水平拖动
@FunctionDescription : 处理水平拖动
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{

	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);


	//获取前一个窗口的索引
	int nPrexIndex = GetPrexHorSplitBarIndex(nCurrentIndex);

	//限制鼠标范围
	RECT CursorRect;
	//如果有前一个分隔条
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//如果没有分隔条
	else
	{
		CursorRect.left = DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}
	//获取水平分隔栏
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

/*****************************************************************************
@FunctionName : 处理垂直移动
@FunctionDescription : 处理垂直移动
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{

	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_VerSplitBarRect[nCurrentIndex];
	rc.OffsetRect(0,nMoveDistance);
	//限制鼠标范围
	RECT CursorRect;
	CursorRect.left = 0;
	CursorRect.top = DEFAULT_GAP;
	CursorRect.bottom = nHeight - DEFAULT_GAP;
	CursorRect.right = nWidth;
	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}	
}

/*****************************************************************************
@FunctionName : 处理垂直与水平移动
@FunctionDescription : 处理垂直与水平移动
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSixWidgetSplitAlgorithm::HandleHorAndVerMove( int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect HorRect = *(m_IntersectRect[nCurrentIndex].pRect1);
	CRect VerRect = *(m_IntersectRect[nCurrentIndex].pRect2);
	//判断是否是最后一个点
	if (ARRAY_SIZE(m_IntersectRect) - 1 == nCurrentIndex)
	{
		//限制鼠标范围
		RECT CursorRect;
		CursorRect.left = m_HorSplitBarRect[0].right + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth;
		//客户区左边转换为桌面坐标
		if (NULL != m_pHostWnd)
		{
			m_pHostWnd->ClientToScreen(&CursorRect);
			//限制光标
			ClipCursor(&CursorRect);
		}
	}
	else
	{
		//限制鼠标范围
		RECT CursorRect;
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
		//客户区左边转换为桌面坐标
		if (NULL != m_pHostWnd)
		{
			m_pHostWnd->ClientToScreen(&CursorRect);
			//限制光标
			ClipCursor(&CursorRect);
		}
	}

	HorRect.OffsetRect(nMoveX,0);
	VerRect.OffsetRect(0,nMoveY);

	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&VerRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&HorRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CSixWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
}

void CSixWidgetSplitAlgorithm::CalculateCoordinateAfterVerMove( int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_VerSplitBarRect[nCurrentIndex].OffsetRect(0,nYMove);
}

void CSixWidgetSplitAlgorithm::CalculateCoordinateAfterHorAndVerMove( int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{

	CRect* pRect = m_IntersectRect[nCurrentIndex].pRect1;

	if (NULL != pRect)
	{
		pRect->OffsetRect(nXMove,0);
	}

	//其后的分隔栏也移动
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > m_IntersectRect[nCurrentIndex].nHorIndex)
		{
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
	
	pRect = m_IntersectRect[nCurrentIndex].pRect2;

	if (NULL != pRect)
	{
		pRect->OffsetRect(0,nYMove);
	}
	
}

void CSixWidgetSplitAlgorithm::ReCalculateIntersetRect( void )
{
	//重新计算交叉点
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
}

CSevenWidgetSplitAlgorithm::CSevenWidgetSplitAlgorithm( void )
{

}

CSevenWidgetSplitAlgorithm::~CSevenWidgetSplitAlgorithm( void )
{

}

void CSevenWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 7)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CSevenWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{
	//移动第一个窗口位置
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 3) / 4;
	rc1.bottom = rc1.top + nHeight;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第二个窗口位置
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + (nHeight - DEFAULT_GAP) / 2;
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第三个窗口位置
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc1.top;
	rc3.right = rc3.left + rc2.Width();
	rc3.bottom = rc3.top + rc2.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第四个窗口位置
	CRect rc4;
	rc4.left = rc3.right + DEFAULT_GAP;
	rc4.top = rc3.top;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第5个窗口位置
	CRect rc5;
	rc5.left = rc2.left;
	rc5.top = rc2.bottom + DEFAULT_GAP;
	rc5.right = rc5.left + rc2.Width();
	rc5.bottom = rc5.top + rc2.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第6个窗口位置
	CRect rc6;
	rc6.left = rc5.right + DEFAULT_GAP;
	rc6.top = rc5.top;
	rc6.right = rc6.left + rc5.Width();
	rc6.bottom = rc6.top + rc5.Height();
	pWidget = m_WidgetArray[5];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc6);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第7个窗口位置
	CRect rc7;
	rc7.left = rc6.right + DEFAULT_GAP;
	rc7.top = rc6.top;
	rc7.right = rc7.left + rc6.Width();
	rc7.bottom = rc7.top + rc6.Height();
	pWidget = m_WidgetArray[6];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc7);
		pWidget->ShowWindow(SW_SHOW);
	}

	//计算分割条的位置
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = rc2.left;
	m_HorSplitBarRect[0].bottom = rc1.bottom;

	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = rc3.left;
	m_HorSplitBarRect[1].bottom = rc1.bottom;

	m_HorSplitBarRect[2].left = rc3.right;
	m_HorSplitBarRect[2].top = rc3.top;
	m_HorSplitBarRect[2].right = rc4.left;
	m_HorSplitBarRect[2].bottom = rc1.bottom;

	m_VerSplitBarRect[0].left = rc2.left;
	m_VerSplitBarRect[0].top = rc2.bottom;
	m_VerSplitBarRect[0].right = rc4.right;
	m_VerSplitBarRect[0].bottom = rc5.top;

	//设置相交点
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 1;
	m_IntersectRect[0].nVerIndex = 0;

	//设置相交点
	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[2],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].pRect1 = &m_HorSplitBarRect[2];
	m_IntersectRect[1].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[1].nHorIndex = 2;
	m_IntersectRect[1].nVerIndex = 0;

}

void CSevenWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{

	//移动第一个窗口位置
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 3) / 4;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP) / 2;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第二个窗口位置
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + rc1.Height();
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第三个窗口位置
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc2.top;
	rc3.right = rc3.left + rc2.Width();
	rc3.bottom = rc3.top + rc2.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第四个窗口位置
	CRect rc4;
	rc4.left = rc1.left;
	rc4.top = rc1.bottom + DEFAULT_GAP;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第5个窗口位置
	CRect rc5;
	rc5.left = rc4.right + DEFAULT_GAP;
	rc5.top = rc4.top;
	rc5.right = rc5.left + rc4.Width();
	rc5.bottom = rc5.top + rc4.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第6个窗口位置
	CRect rc6;
	rc6.left = rc5.right + DEFAULT_GAP;
	rc6.top = rc5.top;
	rc6.right = rc6.left + rc5.Width();
	rc6.bottom = rc6.top + rc5.Height();
	pWidget = m_WidgetArray[5];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc6);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第7个窗口位置
	CRect rc7;
	rc7.left = rc6.right + DEFAULT_GAP;
	rc7.top = rc1.top;
	rc7.right = rc7.left + rc6.Width();
	rc7.bottom = rc7.top + nHeight;
	pWidget = m_WidgetArray[6];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc7);
		pWidget->ShowWindow(SW_SHOW);
	}



	//计算分割条的位置
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = m_HorSplitBarRect[0].left + DEFAULT_GAP;
	m_HorSplitBarRect[0].bottom = rc4.bottom;

	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = m_HorSplitBarRect[1].left + DEFAULT_GAP;
	m_HorSplitBarRect[1].bottom = rc5.bottom;

	m_HorSplitBarRect[2].left = rc3.right;
	m_HorSplitBarRect[2].top = rc3.top;
	m_HorSplitBarRect[2].right = m_HorSplitBarRect[2].left + DEFAULT_GAP;
	m_HorSplitBarRect[2].bottom = rc6.bottom;

	m_VerSplitBarRect[0].left = rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = rc3.right;
	m_VerSplitBarRect[0].bottom = rc4.top;

	//设置相交点
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 0;
	m_IntersectRect[0].nVerIndex = 0;

	//设置相交点
	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[1].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[1].nHorIndex = 1;
	m_IntersectRect[1].nVerIndex = 0;
}

void CSevenWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	if (m_bWndFront)
	{
		//第一个窗口的位置定位
		CRect Widget1;
		Widget1.left = 0;
		Widget1.top = m_HorSplitBarRect[0].top;
		Widget1.right = m_HorSplitBarRect[0].left;
		Widget1.bottom = m_HorSplitBarRect[0].bottom;
		LayoutSizeArray.push_back(Widget1);

		//第二个窗口的位置定位
		CRect Widget2;
		Widget2.left = m_HorSplitBarRect[0].right;
		Widget2.top = m_HorSplitBarRect[0].top;
		Widget2.right = m_HorSplitBarRect[1].left;
		Widget2.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget2);

		//第三个窗口的位置定位
		CRect Widget3;
		Widget3.left = m_HorSplitBarRect[1].right;
		Widget3.top = m_HorSplitBarRect[1].top;
		Widget3.right = m_HorSplitBarRect[2].left;
		Widget3.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget3);

		//第四个窗口的位置定位
		CRect Widget4;
		Widget4.left = m_HorSplitBarRect[2].right;
		Widget4.top = m_HorSplitBarRect[2].top;
		Widget4.right = nWidth;
		Widget4.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget4);

		//第五个窗口的位置定位
		CRect Widget5;
		Widget5.left = m_HorSplitBarRect[0].right;
		Widget5.top = m_VerSplitBarRect[0].bottom;
		Widget5.right = m_HorSplitBarRect[1].left;
		Widget5.bottom = nHeight;
		LayoutSizeArray.push_back(Widget5);

		//第六个窗口的位置定位
		CRect Widget6;
		Widget6.left = m_HorSplitBarRect[1].right;
		Widget6.top = m_VerSplitBarRect[0].bottom;
		Widget6.right = m_HorSplitBarRect[2].left;
		Widget6.bottom = nHeight;
		LayoutSizeArray.push_back(Widget6);

		//第七个窗口的位置定位
		CRect Widget7;
		Widget7.left = m_HorSplitBarRect[2].right;
		Widget7.top = m_VerSplitBarRect[0].bottom;
		Widget7.right = nWidth;
		Widget7.bottom = nHeight;
		LayoutSizeArray.push_back(Widget7);
	}
	else
	{
		//第一个窗口的位置定位
		CRect Widget1;
		Widget1.left = 0;
		Widget1.top = m_HorSplitBarRect[0].top;
		Widget1.right = m_HorSplitBarRect[0].left;
		Widget1.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget1);

		//第二个窗口的位置定位
		CRect Widget2;
		Widget2.left = m_HorSplitBarRect[0].right;
		Widget2.top = m_HorSplitBarRect[0].top;
		Widget2.right = m_HorSplitBarRect[1].left;
		Widget2.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget2);

		//第三个窗口的位置定位
		CRect Widget3;
		Widget3.left = m_HorSplitBarRect[1].right;
		Widget3.top = m_HorSplitBarRect[1].top;
		Widget3.right = m_HorSplitBarRect[2].left;
		Widget3.bottom = m_VerSplitBarRect[0].top;
		LayoutSizeArray.push_back(Widget3);

		//第四个窗口的位置定位
		CRect Widget4;
		Widget4.left = 0;
		Widget4.top = m_VerSplitBarRect[0].bottom;
		Widget4.right = m_HorSplitBarRect[0].left;
		Widget4.bottom = nHeight;
		LayoutSizeArray.push_back(Widget4);

		//第五个窗口的位置定位
		CRect Widget5;
		Widget5.left = m_HorSplitBarRect[0].right;
		Widget5.top = m_VerSplitBarRect[0].bottom;
		Widget5.right = m_HorSplitBarRect[1].left;
		Widget5.bottom = nHeight;
		LayoutSizeArray.push_back(Widget5);

		//第六个窗口的位置定位
		CRect Widget6;
		Widget6.left = m_HorSplitBarRect[1].right;
		Widget6.top = m_VerSplitBarRect[0].bottom;
		Widget6.right = m_HorSplitBarRect[2].left;
		Widget6.bottom = nHeight;
		LayoutSizeArray.push_back(Widget6);

		//第七个窗口的位置定位
		CRect Widget7;
		Widget7.left = m_HorSplitBarRect[2].right;
		Widget7.top = m_HorSplitBarRect[2].top;
		Widget7.right = nWidth;
		Widget7.bottom = nHeight;
		LayoutSizeArray.push_back(Widget7);

	}
}

MOUSE_STATE CSevenWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{

#if 0
	if (m_bWndFront)
	{
		//判断是否是交叉点
		for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
		{
			if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_ALL;
				return MOVE_ALL;
			}

		}

		//判断是否水平移动
		for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
		{
			if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_HOR;
				return MOVE_HOR;
			}
		}

		for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
		{
			//判断是否垂直移动
			if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_VER;
				return MOVE_VER;
			}
		}
	}
	else
	{
		//判断是否是交叉点
		int nNum = sizeof(m_IntersectRect) / sizeof(m_IntersectRect[0]);

		for (int i = 0; i < nNum; ++i)
		{
			if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_ALL;
				return MOVE_ALL;
			}

		}

		//判断是否水平移动
		nNum = sizeof(m_SplitBarRect) / sizeof(m_SplitBarRect[0]);

		for (int i = 0; i < nNum -1; ++i)
		{
			if (TRUE == m_SplitBarRect[i].PtInRect(pt))
			{
				m_SelectSplitBarInfo.nIndex = i;
				m_SelectSplitBarInfo.Type = MOVE_HOR;
				return MOVE_HOR;
			}
		}

		//判断是否垂直移动
		if (TRUE == m_SplitBarRect[nNum - 1].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = nNum - 1;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}

	}


#endif

	//判断是否是交叉点
	for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
	{
		if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_ALL;
			return MOVE_ALL;
		}

	}

	//判断是否水平移动
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		//判断是否垂直移动
		if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
		{
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}

	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

/*****************************************************************************
@FunctionName : 处理左键按下
@FunctionDescription : 处理左键按下
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CSevenWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CSevenWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	ClipCursor(NULL);

	//隐藏分隔条
	ShowHorSplitBar(false);
	ShowVerSplitBar(false);

	//计算拖动分隔条之后的布局
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//重新计算交叉点位置
	ReCalculateIntersetRect();
	//改变布局
	ChangeLayout(nWidth,nHeight);
}

void CSevenWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//左键按下移动鼠标
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		//如果是水平移动
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			//计算水平方向移动的像素数
			int nXMove = pt.x - m_StartPoint.x;

			//处理水平移动
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//如果是垂直方向移动
		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			//计算垂直方向移动的像素数
			int nYMove = pt.y - m_StartPoint.y;

			//处理垂直移动
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//如果是垂直与水平移动
		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			//计算垂直移动的像素数
			int nYMove = pt.y - m_StartPoint.y;
			//计算水平移动的像素数
			int nXMove = pt.x - m_StartPoint.x;
			//计算垂直与水平移动的像素数
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CSevenWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);


	//获取前一个窗口的索引
	int nPrexIndex = GetPrexHorSplitBarIndex(nCurrentIndex);

	//限制鼠标范围
	RECT CursorRect;
	//如果有前一个分隔条
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//如果没有分隔条
	else
	{
		CursorRect.left = DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}
	//获取水平分隔栏
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CSevenWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_VerSplitBarRect[nCurrentIndex];
	rc.OffsetRect(0,nMoveDistance);
	//限制鼠标范围
	RECT CursorRect;
	CursorRect.left = 0;
	CursorRect.top = DEFAULT_GAP;
	CursorRect.bottom = nHeight - DEFAULT_GAP;
	CursorRect.right = nWidth;
	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CSevenWidgetSplitAlgorithm::HandleHorAndVerMove( int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect HorRect = *(m_IntersectRect[nCurrentIndex].pRect1);
	CRect VerRect = *(m_IntersectRect[nCurrentIndex].pRect2);
	HorRect.OffsetRect(nMoveX,0);
	VerRect.OffsetRect(0,nMoveY);


	//限制鼠标范围
	RECT CursorRect;
	int nPreIndex = GetPrexHorSplitBarIndex(m_IntersectRect[nCurrentIndex].nHorIndex);
	if (nPreIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPreIndex].right + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	else
	{
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}
	
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&VerRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&HorRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CSevenWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{
	//大窗口在后面
	if (m_bWndFront)
	{
		m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
		//如果是第一个位置
		if (0 == nCurrentIndex)
		{
			//垂直分隔条位置要改变
			m_VerSplitBarRect[0].left += nXMove;
		}
		for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
		{
			if (i > nCurrentIndex)
			{
				//计算后面的位置
				m_HorSplitBarRect[i].OffsetRect(nXMove,0);
			}
		}
	}
	else
	{
		m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
		m_VerSplitBarRect[0].right += nXMove;
		//后续的分隔条相应后移
		for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
		{
			if (i > nCurrentIndex)
			{
				m_HorSplitBarRect[i].OffsetRect(nXMove,0);
			}
		}		
	}
}

void CSevenWidgetSplitAlgorithm::CalculateCoordinateAfterVerMove( int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_VerSplitBarRect[nCurrentIndex].OffsetRect(0,nYMove);
}

void CSevenWidgetSplitAlgorithm::CalculateCoordinateAfterHorAndVerMove( int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	CRect* pRect = m_IntersectRect[nCurrentIndex].pRect1;
	if (NULL != pRect)
	{
		pRect->OffsetRect(nXMove,0);
	}

	//向后移动后面的分隔条
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > m_IntersectRect[nCurrentIndex].nHorIndex)
		{
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
	pRect = m_IntersectRect[nCurrentIndex].pRect2;
	if (NULL != pRect)
	{
		pRect->OffsetRect(0,nYMove);
	}
}

void CSevenWidgetSplitAlgorithm::ReCalculateIntersetRect( void )
{
	//重新计算交叉点
	if (m_bWndFront)
	{
		m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
		m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[2],&m_VerSplitBarRect[0]);
	}
	else
	{
		m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
		m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	}
}

CEightWidgetSplitAlgorithm::CEightWidgetSplitAlgorithm( void )
{

}

CEightWidgetSplitAlgorithm::~CEightWidgetSplitAlgorithm( void )
{

}

void CEightWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 8)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CEightWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{

	//移动第一个窗口位置
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 3) / 4;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP) / 2;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第二个窗口位置
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + rc1.Height();
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第三个窗口位置
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc2.top;
	rc3.right = rc3.left + rc2.Width();
	rc3.bottom = rc3.top + rc2.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}


	//移动第四个窗口位置
	CRect rc4;
	rc4.left = rc3.right + DEFAULT_GAP;
	rc4.top = rc3.top;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第5个窗口位置
	CRect rc5;
	rc5.left = rc1.left;
	rc5.top = rc1.bottom + DEFAULT_GAP;
	rc5.right = rc5.left + rc4.Width();
	rc5.bottom = rc5.top + rc4.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第6个窗口位置
	CRect rc6;
	rc6.left = rc5.right + DEFAULT_GAP;
	rc6.top = rc5.top;
	rc6.right = rc6.left + rc5.Width();
	rc6.bottom = rc6.top + rc5.Height();
	pWidget = m_WidgetArray[5];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc6);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第7个窗口位置
	CRect rc7;
	rc7.left = rc6.right + DEFAULT_GAP;
	rc7.top = rc6.top;
	rc7.right = rc7.left + rc6.Width();
	rc7.bottom = rc7.top + rc6.Height();
	pWidget = m_WidgetArray[6];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc7);
		pWidget->ShowWindow(SW_SHOW);
	}
	//移动第8个窗口位置
	CRect rc8;
	rc8.left = rc7.right + DEFAULT_GAP;
	rc8.top = rc7.top;
	rc8.right = rc8.left + rc7.Width();
	rc8.bottom = rc8.top + rc7.Height();
	pWidget = m_WidgetArray[7];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc8);
		pWidget->ShowWindow(SW_SHOW);
	}

	//设置分隔带的位置
	m_HorSplitBarRect[0].left =rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = m_HorSplitBarRect[0].left + DEFAULT_GAP;
	m_HorSplitBarRect[0].bottom = nHeight;

	m_HorSplitBarRect[1].left =rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = m_HorSplitBarRect[1].left + DEFAULT_GAP;
	m_HorSplitBarRect[1].bottom = nHeight;

	m_HorSplitBarRect[2].left =rc3.right;
	m_HorSplitBarRect[2].top = rc3.top;
	m_HorSplitBarRect[2].right = m_HorSplitBarRect[2].left + DEFAULT_GAP;
	m_HorSplitBarRect[2].bottom = nHeight;

	m_VerSplitBarRect[0].left =rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = nWidth;
	m_VerSplitBarRect[0].bottom = m_VerSplitBarRect[0].top + DEFAULT_GAP;

	//计算相交的矩形
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 0;
	m_IntersectRect[0].nVerIndex = 0;

	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[1].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[1].nHorIndex = 1;
	m_IntersectRect[1].nVerIndex = 0;

	m_IntersectRect[2].InterSectRect.IntersectRect(&m_HorSplitBarRect[2],&m_VerSplitBarRect[0]);
	m_IntersectRect[2].pRect1 = &m_HorSplitBarRect[2];
	m_IntersectRect[2].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[2].nHorIndex = 2;
	m_IntersectRect[2].nVerIndex = 0;
}

void CEightWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{
	AdjustLayoutFront(nWidth,nHeight);
}

void CEightWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	CRect Widget1;
	Widget1.left = 0;
	Widget1.top = 0;
	Widget1.right = m_HorSplitBarRect[0].left;
	Widget1.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget1);

	CRect Widget2;
	Widget2.left = m_HorSplitBarRect[0].right;
	Widget2.top = m_HorSplitBarRect[0].top;
	Widget2.right = m_HorSplitBarRect[1].left;
	Widget2.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget2);

	CRect Widget3;
	Widget3.left = m_HorSplitBarRect[1].right;
	Widget3.top = m_HorSplitBarRect[1].top;
	Widget3.right = m_HorSplitBarRect[2].left;
	Widget3.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget3);

	CRect Widget4;
	Widget4.left = m_HorSplitBarRect[2].right;
	Widget4.top = m_HorSplitBarRect[2].top;
	Widget4.right = nWidth;
	Widget4.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget4);

	CRect Widget5;
	Widget5.left = 0;
	Widget5.top = m_VerSplitBarRect[0].bottom;
	Widget5.right = m_HorSplitBarRect[0].left;
	Widget5.bottom = nHeight;
	LayoutSizeArray.push_back(Widget5);

	CRect Widget6;
	Widget6.left = m_HorSplitBarRect[0].right;
	Widget6.top = m_VerSplitBarRect[0].bottom;
	Widget6.right = m_HorSplitBarRect[1].left;
	Widget6.bottom = nHeight;
	LayoutSizeArray.push_back(Widget6);

	CRect Widget7;
	Widget7.left = m_HorSplitBarRect[1].right;
	Widget7.top = m_VerSplitBarRect[0].bottom;
	Widget7.right = m_HorSplitBarRect[2].left;
	Widget7.bottom = nHeight;
	LayoutSizeArray.push_back(Widget7);

	CRect Widget8;
	Widget8.left = m_HorSplitBarRect[2].right;
	Widget8.top = m_VerSplitBarRect[0].bottom;
	Widget8.right = nWidth;
	Widget8.bottom = nHeight;
	LayoutSizeArray.push_back(Widget8);
}

MOUSE_STATE CEightWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{
	//判断是否是分割线交叉点

	for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
	{
		if (TRUE == m_IntersectRect[i].InterSectRect.PtInRect(pt))
		{
			//记录选择矩形的位置
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_ALL;
			return MOVE_ALL;
		}
	}

	//判断是否可以水平移动的
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (TRUE == m_HorSplitBarRect[i].PtInRect(pt))
		{
			//记录选择矩形的位置
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (TRUE == m_VerSplitBarRect[i].PtInRect(pt))
		{
			//记录选择矩形的位置
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}

	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CEightWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CEightWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	ClipCursor(NULL);
	//隐藏分隔条
	ShowHorSplitBar(false);
	ShowVerSplitBar(false);

	//计算拖动分隔条之后的布局
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//重新计算交叉点位置
	ReCalculateIntersetRect();
	//改变布局
	ChangeLayout(nWidth,nHeight);
}

/*****************************************************************************
@FunctionName : HandleMouseMove
@FunctionDescription : 判断8个窗口下的鼠标移动
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CEightWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//左键按下移动鼠标
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		//如果是水平移动
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			//计算水平方向移动的像素数
			int nXMove = pt.x - m_StartPoint.x;

			//处理水平移动
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//如果是垂直方向移动
		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			//计算垂直方向移动的像素数
			int nYMove = pt.y - m_StartPoint.y;

			//处理垂直移动
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//如果是垂直与水平移动
		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			//计算垂直移动的像素数
			int nYMove = pt.y - m_StartPoint.y;
			//计算水平移动的像素数
			int nXMove = pt.x - m_StartPoint.x;
			//计算垂直与水平移动的像素数
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

/*****************************************************************************
@FunctionName : HandleHorMove
@FunctionDescription : 处理8个窗口下的分隔条移动
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CEightWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	//参数合法性判断
	if (NULL == m_pLayout)
	{
		return;
	}
	//获取选中的分隔条的位置
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);
	//获取前一个窗口的索引
	int nPrexIndex = GetPrexHorSplitBarIndex(nCurrentIndex);

	//限制鼠标范围
	RECT CursorRect;
	//如果有前一个分隔条
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//如果没有分隔条
	else
	{
		CursorRect.left = DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}
	//获取水平分隔栏
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}

}

void CEightWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	//参数合法性判断
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect rc = m_VerSplitBarRect[nCurrentIndex];
	rc.OffsetRect(0,nMoveDistance);
	//限制鼠标范围
	RECT CursorRect;
	CursorRect.left = 0;
	CursorRect.top = DEFAULT_GAP;
	CursorRect.bottom = nHeight - DEFAULT_GAP;
	CursorRect.right = nWidth;
	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}	
}

/*****************************************************************************
@FunctionName : HandleHorAndVerMove
@FunctionDescription : 处理8个窗口的水平和垂直移动
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CEightWidgetSplitAlgorithm::HandleHorAndVerMove( int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight )
{
	if (NULL == m_pLayout)
	{
		return;
	}
	CRect HorRect = *(m_IntersectRect[nCurrentIndex].pRect1);
	CRect VerRect = *(m_IntersectRect[nCurrentIndex].pRect2);
	HorRect.OffsetRect(nMoveX,0);
	VerRect.OffsetRect(0,nMoveY);


	//限制鼠标范围
	RECT CursorRect;
	int nPreIndex = GetPrexHorSplitBarIndex(m_IntersectRect[nCurrentIndex].nHorIndex);
	if (nPreIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPreIndex].right + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	else
	{
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}

	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&VerRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&HorRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}


}

void CEightWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{

	m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
	//此分隔条后的分隔条都要依次偏移
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > nCurrentIndex)
		{
			//计算后面的位置
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}

}

void CEightWidgetSplitAlgorithm::CalculateCoordinateAfterVerMove( int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_VerSplitBarRect[nCurrentIndex].OffsetRect(0,nYMove);
}

void CEightWidgetSplitAlgorithm::CalculateCoordinateAfterHorAndVerMove( int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	CRect* pRect = m_IntersectRect[nCurrentIndex].pRect1;
	if (NULL != pRect)
	{
		pRect->OffsetRect(nXMove,0);
	}

	//向后移动后面的分隔条
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > m_IntersectRect[nCurrentIndex].nHorIndex)
		{
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
	pRect = m_IntersectRect[nCurrentIndex].pRect2;
	if (NULL != pRect)
	{
		pRect->OffsetRect(0,nYMove);
	}
}

void CEightWidgetSplitAlgorithm::ReCalculateIntersetRect( void )
{
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[2].InterSectRect.IntersectRect(&m_HorSplitBarRect[2],&m_VerSplitBarRect[0]);
}

CNineWidgetSplitAlgorithm::CNineWidgetSplitAlgorithm( void )
{

}

CNineWidgetSplitAlgorithm::~CNineWidgetSplitAlgorithm( void )
{

}

void CNineWidgetSplitAlgorithm::AdjustLayout( int nWidth,int nHeight )
{
	if (NULL == m_pHostWnd || m_WidgetArray.size() < 9)
	{
		return;
	}

	if (m_bWndFront)
	{
		AdjustLayoutFront(nWidth,nHeight);
	}
	else
	{
		AdjustLayoutBack(nWidth,nHeight);
	}
}

void CNineWidgetSplitAlgorithm::AdjustLayoutFront( int nWidth,int nHeight )
{

	//移动第一个窗口位置
	CRect rc1;
	rc1.left = 0;
	rc1.top = 0;
	rc1.right = rc1.left + (nWidth - DEFAULT_GAP * 2) / 3;
	rc1.bottom = rc1.top + (nHeight - DEFAULT_GAP * 2) / 3;
	CWnd* pWidget = m_WidgetArray[0];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc1);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第二个窗口位置
	CRect rc2;
	rc2.left = rc1.right + DEFAULT_GAP;
	rc2.top = rc1.top;
	rc2.right = rc2.left + rc1.Width();
	rc2.bottom = rc2.top + rc1.Height();
	pWidget = m_WidgetArray[1];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc2);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第三个窗口位置
	CRect rc3;
	rc3.left = rc2.right + DEFAULT_GAP;
	rc3.top = rc2.top;
	rc3.right = rc3.left + rc2.Width();
	rc3.bottom = rc3.top + rc2.Height();
	pWidget = m_WidgetArray[2];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc3);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第四个窗口位置
	CRect rc4;
	rc4.left = rc1.left;
	rc4.top = rc1.bottom + DEFAULT_GAP;
	rc4.right = rc4.left + rc3.Width();
	rc4.bottom = rc4.top + rc3.Height();
	pWidget = m_WidgetArray[3];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc4);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第5个窗口位置
	CRect rc5;
	rc5.left = rc4.right + DEFAULT_GAP;
	rc5.top = rc4.top;
	rc5.right = rc5.left + rc4.Width();
	rc5.bottom = rc5.top + rc4.Height();
	pWidget = m_WidgetArray[4];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc5);
		pWidget->ShowWindow(SW_SHOW);
	}
	//移动第6个窗口位置
	CRect rc6;
	rc6.left = rc5.right + DEFAULT_GAP;
	rc6.top = rc5.top;
	rc6.right = rc6.left + rc5.Width();
	rc6.bottom = rc6.top + rc5.Height();
	pWidget = m_WidgetArray[5];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc6);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第7个窗口位置
	CRect rc7;
	rc7.left = rc4.left;
	rc7.top = rc4.bottom + DEFAULT_GAP;
	rc7.right = rc7.left + rc6.Width();
	rc7.bottom = rc7.top + rc6.Height();
	pWidget = m_WidgetArray[6];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc7);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第8个窗口位置
	CRect rc8;
	rc8.left = rc7.right + DEFAULT_GAP;
	rc8.top = rc7.top;
	rc8.right = rc8.left + rc7.Width();
	rc8.bottom = rc8.top + rc7.Height();
	pWidget = m_WidgetArray[7];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc8);
		pWidget->ShowWindow(SW_SHOW);
	}

	//移动第9个窗口位置
	CRect rc9;
	rc9.left = rc8.right + DEFAULT_GAP;
	rc9.top = rc8.top;
	rc9.right = rc9.left + rc8.Width();
	rc9.bottom = rc9.top + rc8.Height();
	pWidget = m_WidgetArray[8];
	if (NULL != pWidget && NULL != pWidget->GetSafeHwnd())
	{
		pWidget->MoveWindow(&rc9);
		pWidget->ShowWindow(SW_SHOW);
	}

	//计算分割线的位置
	m_HorSplitBarRect[0].left = rc1.right;
	m_HorSplitBarRect[0].top = rc1.top;
	m_HorSplitBarRect[0].right = m_HorSplitBarRect[0].left + DEFAULT_GAP;
	m_HorSplitBarRect[0].bottom = nHeight;

	m_HorSplitBarRect[1].left = rc2.right;
	m_HorSplitBarRect[1].top = rc2.top;
	m_HorSplitBarRect[1].right = m_HorSplitBarRect[1].left + DEFAULT_GAP;
	m_HorSplitBarRect[1].bottom = nHeight;


	m_VerSplitBarRect[0].left = rc1.left;
	m_VerSplitBarRect[0].top = rc1.bottom;
	m_VerSplitBarRect[0].right = nWidth;
	m_VerSplitBarRect[0].bottom = m_VerSplitBarRect[0].top + DEFAULT_GAP;


	m_VerSplitBarRect[1].left = rc4.left;
	m_VerSplitBarRect[1].top = rc4.bottom;
	m_VerSplitBarRect[1].right = nWidth;
	m_VerSplitBarRect[1].bottom = m_VerSplitBarRect[1].top + DEFAULT_GAP;

	//计算相交的矩形
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[0].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[0].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[0].nHorIndex = 0;
	m_IntersectRect[0].nVerIndex = 0;

	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[1].pRect2 = &m_VerSplitBarRect[0];
	m_IntersectRect[1].nHorIndex = 1;
	m_IntersectRect[1].nVerIndex = 0;

	m_IntersectRect[2].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[1]);
	m_IntersectRect[2].pRect1 = &m_HorSplitBarRect[0];
	m_IntersectRect[2].pRect2 = &m_VerSplitBarRect[1];
	m_IntersectRect[2].nHorIndex = 0;
	m_IntersectRect[2].nVerIndex = 1;

	m_IntersectRect[3].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[1]);
	m_IntersectRect[3].pRect1 = &m_HorSplitBarRect[1];
	m_IntersectRect[3].pRect2 = &m_VerSplitBarRect[1];
	m_IntersectRect[3].nHorIndex = 1;
	m_IntersectRect[3].nVerIndex = 1;
}

void CNineWidgetSplitAlgorithm::AdjustLayoutBack( int nWidth,int nHeight )
{
	AdjustLayoutFront(nWidth,nHeight);
}

void CNineWidgetSplitAlgorithm::CalculateLayoutSize( int nWidth,int nHeight,std::vector<CRect>& LayoutSizeArray )
{
	LayoutSizeArray.clear();

	CRect Widget1;
	Widget1.left = 0;
	Widget1.top = m_HorSplitBarRect[0].top;
	Widget1.right = m_HorSplitBarRect[0].left;
	Widget1.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget1);

	CRect Widget2;
	Widget2.left = m_HorSplitBarRect[0].right;
	Widget2.top = m_HorSplitBarRect[0].top;
	Widget2.right = m_HorSplitBarRect[1].left;
	Widget2.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget2);

	CRect Widget3;
	Widget3.left = m_HorSplitBarRect[1].right;
	Widget3.top = m_HorSplitBarRect[1].top;
	Widget3.right = nWidth;
	Widget3.bottom = m_VerSplitBarRect[0].top;
	LayoutSizeArray.push_back(Widget3);

	CRect Widget4;
	Widget4.left = m_VerSplitBarRect[0].left;
	Widget4.top = m_VerSplitBarRect[0].bottom;
	Widget4.right = m_HorSplitBarRect[0].left;
	Widget4.bottom = m_VerSplitBarRect[1].top;
	LayoutSizeArray.push_back(Widget4);

	CRect Widget5;
	Widget5.left = m_HorSplitBarRect[0].right;
	Widget5.top = m_VerSplitBarRect[0].bottom;
	Widget5.right = m_HorSplitBarRect[1].left;
	Widget5.bottom = m_VerSplitBarRect[1].top;
	LayoutSizeArray.push_back(Widget5);

	CRect Widget6;
	Widget6.left = m_HorSplitBarRect[1].right;
	Widget6.top = m_VerSplitBarRect[0].bottom;
	Widget6.right = nWidth;
	Widget6.bottom = m_VerSplitBarRect[1].top;
	LayoutSizeArray.push_back(Widget6);

	CRect Widget7;
	Widget7.left = m_VerSplitBarRect[1].left;
	Widget7.top = m_VerSplitBarRect[1].bottom;
	Widget7.right = m_HorSplitBarRect[0].left;
	Widget7.bottom = nHeight;
	LayoutSizeArray.push_back(Widget7);

	CRect Widget8;
	Widget8.left = m_HorSplitBarRect[0].right;
	Widget8.top = m_VerSplitBarRect[1].bottom;
	Widget8.right = m_HorSplitBarRect[1].left;
	Widget8.bottom = nHeight;
	LayoutSizeArray.push_back(Widget8);

	CRect Widget9;
	Widget9.left = m_HorSplitBarRect[1].right;
	Widget9.top = m_VerSplitBarRect[1].bottom;
	Widget9.right = nWidth;
	Widget9.bottom = nHeight;
	LayoutSizeArray.push_back(Widget9);

}

MOUSE_STATE CNineWidgetSplitAlgorithm::QueryPosStatus( const CPoint& pt )
{
	//判断是否在4个交点处
	for (int i = 0; i < ARRAY_SIZE(m_IntersectRect); ++i)
	{
		if (m_IntersectRect[i].InterSectRect.PtInRect(pt) == TRUE)
		{
			//记录选择矩形的位置
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_ALL;
			return MOVE_ALL;
		}
	}

	//是否可以水平移动
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (m_HorSplitBarRect[i].PtInRect(pt) == TRUE)
		{
			//记录选择矩形的位置
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_HOR;
			return MOVE_HOR;
		}
	}

	//是否可以垂直移动
	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (m_VerSplitBarRect[i].PtInRect(pt) == TRUE)
		{
			//记录选择矩形的位置
			m_SelectSplitBarInfo.nIndex = i;
			m_SelectSplitBarInfo.Type = MOVE_VER;
			return MOVE_VER;
		}
	}
	return CAbstractSplitAlgorithm::QueryPosStatus(pt);
}

void CNineWidgetSplitAlgorithm::HandleLButtonDown( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	CAbstractSplitAlgorithm::HandleLButtonDown(nFlags,pt,nWidth,nHeight);
}

void CNineWidgetSplitAlgorithm::HandleLButtonUp( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	ClipCursor(NULL);
	//隐藏分隔条
	ShowHorSplitBar(false);
	ShowVerSplitBar(false);

	//计算拖动分隔条之后的布局
	switch (m_SelectSplitBarInfo.Type)
	{
	case MOVE_HOR:
		{
			int nXMove = pt.x - m_StartPoint.x;
			CalculateCoordinateAfterHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_VER:
		{
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	case MOVE_ALL:
		{
			int nXMove = pt.x - m_StartPoint.x;
			int nYMove = pt.y - m_StartPoint.y;
			CalculateCoordinateAfterHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		break;
	default:
		break;
	}

	//重新计算交叉点位置
	ReCalculateIntersetRect();
	//改变布局
	ChangeLayout(nWidth,nHeight);
}

void CNineWidgetSplitAlgorithm::HandleMouseMove( UINT nFlags,const CPoint& pt,int nWidth,int nHeight )
{
	//左键按下移动鼠标
	if (MK_LBUTTON == (MK_LBUTTON & nFlags))
	{
		//如果是水平移动
		if (MOVE_HOR == m_SelectSplitBarInfo.Type)
		{
			//计算水平方向移动的像素数
			int nXMove = pt.x - m_StartPoint.x;

			//处理水平移动
			HandleHorMove(nXMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//如果是垂直方向移动
		if (MOVE_VER == m_SelectSplitBarInfo.Type)
		{
			//计算垂直方向移动的像素数
			int nYMove = pt.y - m_StartPoint.y;

			//处理垂直移动
			HandleVerMove(nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}

		//如果是垂直与水平移动
		if (MOVE_ALL == m_SelectSplitBarInfo.Type)
		{
			//计算垂直移动的像素数
			int nYMove = pt.y - m_StartPoint.y;
			//计算水平移动的像素数
			int nXMove = pt.x - m_StartPoint.x;
			//计算垂直与水平移动的像素数
			HandleHorAndVerMove(nXMove,nYMove,m_SelectSplitBarInfo.nIndex,nWidth,nHeight);
		}
		return;
	}

	CAbstractSplitAlgorithm::HandleMouseMove(nFlags,pt,nWidth,nHeight);
}

void CNineWidgetSplitAlgorithm::HandleHorMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	//参数合法性判断
	if (NULL == m_pLayout)
	{
		return;
	}
	//获取选中的分隔条的位置
	CRect rc = m_HorSplitBarRect[nCurrentIndex];
	rc.OffsetRect(nMoveDistance,0);
	//获取前一个窗口的索引
	int nPrexIndex = GetPrexHorSplitBarIndex(nCurrentIndex);

	//限制鼠标范围
	RECT CursorRect;
	//如果有前一个分隔条
	if (nPrexIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPrexIndex].right + DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//如果没有分隔条
	else
	{
		CursorRect.left = DEFAULT_GAP;
		CursorRect.top = 0;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}

	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}
	//获取水平分隔栏
	CWnd* pLayout = m_pLayout->GetMoveHorSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CNineWidgetSplitAlgorithm::HandleVerMove( int nMoveDistance,int nCurrentIndex,int nWidth,int nHeight )
{
	//参数合法性判断
	if (NULL == m_pLayout)
	{
		return;
	}
	//获取选中的分隔条的位置
	CRect rc = m_VerSplitBarRect[nCurrentIndex];
	rc.OffsetRect(0,nMoveDistance);
	//获取前一个窗口的索引
	int nPrexIndex = GetPrexVerSplitBarIndex(nCurrentIndex);

	//限制鼠标范围
	RECT CursorRect;
	//如果有前一个分隔条
	if (nPrexIndex >= 0)
	{
		CursorRect.left = 0;
		CursorRect.top = m_VerSplitBarRect[nPrexIndex].bottom + DEFAULT_GAP;
		CursorRect.bottom = nHeight;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	//如果没有分隔条
	else
	{
		CursorRect.left = 0;
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
		CursorRect.right = nWidth;
	}

	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}
	//获取水平分隔栏
	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();

	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&rc,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
}

void CNineWidgetSplitAlgorithm::HandleHorAndVerMove( int nMoveX,int nMoveY,int nCurrentIndex,int nWidth,int nHeight )
{

	if (NULL == m_pLayout)
	{
		return;
	}
	CRect HorRect = *(m_IntersectRect[nCurrentIndex].pRect1);
	CRect VerRect = *(m_IntersectRect[nCurrentIndex].pRect2);
	HorRect.OffsetRect(nMoveX,0);
	VerRect.OffsetRect(0,nMoveY);


	//限制鼠标范围
	RECT CursorRect;
	int nPreIndex = GetPrexHorSplitBarIndex(m_IntersectRect[nCurrentIndex].nHorIndex);
	if (nPreIndex >= 0)
	{
		CursorRect.left = m_HorSplitBarRect[nPreIndex].right + DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}
	else
	{
		CursorRect.left = 0 + DEFAULT_GAP;
		CursorRect.right = nWidth - DEFAULT_GAP;
	}


	nPreIndex = GetPrexVerSplitBarIndex(m_IntersectRect[nCurrentIndex].nVerIndex);
	if (nPreIndex >= 0)
	{	
		CursorRect.top = m_VerSplitBarRect[nPreIndex].bottom + DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
	}
	else
	{
		CursorRect.top = DEFAULT_GAP;
		CursorRect.bottom = nHeight - DEFAULT_GAP;
	}

	//客户区左边转换为桌面坐标
	if (NULL != m_pHostWnd)
	{
		m_pHostWnd->ClientToScreen(&CursorRect);
		//限制光标
		ClipCursor(&CursorRect);
	}

	CWnd* pLayout = m_pLayout->GetMoveVerSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&VerRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}
	pLayout = m_pLayout->GetMoveHorSplitBar();
	if (NULL != pLayout && NULL != pLayout->GetSafeHwnd())
	{
		pLayout->MoveWindow(&HorRect,TRUE);
		pLayout->Invalidate(TRUE);
		pLayout->ShowWindow(SW_SHOW);
	}

}

void CNineWidgetSplitAlgorithm::CalculateCoordinateAfterHorMove( int nXMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_HorSplitBarRect[nCurrentIndex].OffsetRect(nXMove,0);
	//此分隔条后的分隔条都要依次偏移
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > nCurrentIndex)
		{
			//计算后面的位置
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
}

void CNineWidgetSplitAlgorithm::CalculateCoordinateAfterVerMove( int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	m_VerSplitBarRect[nCurrentIndex].OffsetRect(0,nYMove);
	//此分隔条后的分隔条都要依次偏移
	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (i > nCurrentIndex)
		{
			//计算后面的位置
			m_VerSplitBarRect[i].OffsetRect(0,nYMove);
		}
	}
}

void CNineWidgetSplitAlgorithm::CalculateCoordinateAfterHorAndVerMove( int nXMove,int nYMove,int nCurrentIndex,int nWidth,int nHeight )
{
	CRect* pRect = m_IntersectRect[nCurrentIndex].pRect1;
	if (NULL != pRect)
	{
		pRect->OffsetRect(nXMove,0);
	}

	//向后移动后面的分隔条
	for (int i = 0; i < ARRAY_SIZE(m_HorSplitBarRect); ++i)
	{
		if (i > m_IntersectRect[nCurrentIndex].nHorIndex)
		{
			m_HorSplitBarRect[i].OffsetRect(nXMove,0);
		}
	}
	pRect = m_IntersectRect[nCurrentIndex].pRect2;
	if (NULL != pRect)
	{
		pRect->OffsetRect(0,nYMove);
	}

	for (int i = 0; i < ARRAY_SIZE(m_VerSplitBarRect); ++i)
	{
		if (i > m_IntersectRect[nCurrentIndex].nVerIndex)
		{
			m_VerSplitBarRect[i].OffsetRect(0,nYMove);
		}
	}

}

void CNineWidgetSplitAlgorithm::ReCalculateIntersetRect( void )
{
	m_IntersectRect[0].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[0]);
	m_IntersectRect[1].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[0]);
	m_IntersectRect[2].InterSectRect.IntersectRect(&m_HorSplitBarRect[0],&m_VerSplitBarRect[1]);
	m_IntersectRect[3].InterSectRect.IntersectRect(&m_HorSplitBarRect[1],&m_VerSplitBarRect[1]);
}
