/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:BaseDockingBar.h
Description:停靠栏基类，程序中所有的停靠栏都从此派生
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#ifndef BASE_DOCKING_BAR_H
#define BASE_DOCKING_BAR_H
class CBaseDockingBar : public CBCGPDockingControlBar
{
public:
	CBaseDockingBar(void);
	~CBaseDockingBar(void);

	DECLARE_DYNCREATE(CBaseDockingBar)

	DECLARE_MESSAGE_MAP()

public:
	static unsigned int s_DockingBarID;
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


#endif