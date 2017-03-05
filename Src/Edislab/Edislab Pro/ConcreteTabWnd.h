#ifndef CONCRETE_TAB_WND_H
#define CONCRETE_TAB_WND_H
#include "BaseTabWnd.h"


class CConcreteTabWnd : public CBaseTabWnd
{
	DECLARE_DYNCREATE(CConcreteTabWnd)
public:
	CConcreteTabWnd(CWnd* pParenWnd = NULL);
	~CConcreteTabWnd(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};



#endif
