#include "stdafx.h"
#include "ConcreteTabWnd.h"
#include "Msg.h"
IMPLEMENT_DYNCREATE(CConcreteTabWnd,CBaseTabWnd)
CConcreteTabWnd::CConcreteTabWnd( CWnd* pParenWnd /*= NULL*/ ):
CBaseTabWnd(pParenWnd)
{
	m_bAutoDestoyWindow = FALSE;
}

CConcreteTabWnd::~CConcreteTabWnd( void )
{

}
BEGIN_MESSAGE_MAP(CConcreteTabWnd, CBaseTabWnd)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void CConcreteTabWnd::OnSize(UINT nType, int cx, int cy)
{
	CBaseTabWnd::OnSize(nType, cx, cy);
}


void CConcreteTabWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CWnd* pWnd = AfxGetMainWnd();

	//if (NULL != pWnd)
	//{
	//	pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,2,0);
	//}
	CBaseTabWnd::OnLButtonDown(nFlags, point);
}
