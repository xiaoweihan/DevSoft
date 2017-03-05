// SplitBar.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "SplitBar.h"
#include "Utility.h"

// CSplitBar 对话框

//背景色
const COLORREF DEFAULT_BK_COLOR = RGB(200,200,200);

IMPLEMENT_DYNAMIC(CSplitBar, CBaseDialog)

CSplitBar::CSplitBar(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CSplitBar::IDD, pParent),
	m_BkColor(DEFAULT_BK_COLOR)
{

}

CSplitBar::~CSplitBar()
{
}

void CSplitBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplitBar, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSplitBar 消息处理程序


void CSplitBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(&rc,m_BkColor);
	
}

void CSplitBar::SetSplitBarBkColor( COLORREF BkColor )
{
	m_BkColor = BkColor;
}

