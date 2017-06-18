#include "StdAfx.h"
#include "CustomTreeCtrl.h"
#include "Msg.h"
IMPLEMENT_DYNAMIC(CCustomTreeCtrl, CBCGPTreeCtrlEx)

CCustomTreeCtrl::CCustomTreeCtrl(void)
{
}


CCustomTreeCtrl::~CCustomTreeCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CCustomTreeCtrl, CBCGPTreeCtrlEx)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CCustomTreeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	HTREEITEM hDBClikeItem = HitTest(point);

	if (NULL != hDBClikeItem)
	{
		CWnd* pWnd = GetParent();

		if (nullptr != pWnd)
		{
			pWnd->PostMessage(WM_NOTIFY_TREE_CTRL_DBCLICK,(WPARAM)hDBClikeItem,0);
		}
	}

	CBCGPTreeCtrlEx::OnLButtonDblClk(nFlags, point);
}
