/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:BaseDialog.cpp
Description:Dialog的基类,所有对话框都从此派生
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#include "stdafx.h"
#include "BaseDialog.h"
IMPLEMENT_DYNAMIC(CBaseDialog,CBCGPDialog)
CBaseDialog::CBaseDialog( UINT nIDTemplate, CWnd* pParentWnd,BOOL bEnableLayout):
CBCGPDialog(nIDTemplate,pParentWnd)
{
	EnableLayout(bEnableLayout);
}

CBaseDialog::~CBaseDialog( void )
{
	//CBCGPMaskEdit
}


BOOL CBaseDialog::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	EnableVisualManagerStyle(TRUE,TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BEGIN_MESSAGE_MAP(CBaseDialog, CBCGPDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CBaseDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return CBCGPDialog::PreTranslateMessage(pMsg);
}

void CBaseDialog::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	Invalidate(TRUE);
}


