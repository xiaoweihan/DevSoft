/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:BaseDockingBar.cpp
Description:停靠栏基类，程序中所有的停靠栏都从此派生
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#include "stdafx.h"
#include "BaseDockingBar.h"
IMPLEMENT_DYNCREATE(CBaseDockingBar, CBCGPDockingControlBar)
CBaseDockingBar::CBaseDockingBar( void )
{
	m_bVisualManagerStyle = TRUE;
}

CBaseDockingBar::~CBaseDockingBar( void )
{

}

BEGIN_MESSAGE_MAP(CBaseDockingBar, CBCGPDockingControlBar)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

void CBaseDockingBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	COLORREF BKColor = ::GetSysColor (COLOR_3DFACE);
	GetClientRect (rect);
	dc.FillSolidRect(rect,BKColor);

}


void CBaseDockingBar::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
}


int CBaseDockingBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}


void CBaseDockingBar::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	// TODO: 在此处添加消息处理程序代码
}

unsigned int CBaseDockingBar::s_DockingBarID = 0;