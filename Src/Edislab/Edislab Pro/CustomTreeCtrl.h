#pragma once
class CCustomTreeCtrl : public CBCGPTreeCtrlEx
{
	DECLARE_DYNAMIC(CCustomTreeCtrl)
public:
	CCustomTreeCtrl(void);
	virtual ~CCustomTreeCtrl(void);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

