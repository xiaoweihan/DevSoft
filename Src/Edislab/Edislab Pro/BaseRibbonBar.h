#pragma once
class CBaseRibbonBar : public CBCGPRibbonBar
{
public:
	CBaseRibbonBar(void);
	~CBaseRibbonBar(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

