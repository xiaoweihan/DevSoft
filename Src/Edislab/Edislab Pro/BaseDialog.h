/*******************************************************************************
Copyright(C):Envision. Co., Ltd.
FileName:BaseDialog.h
Description:Dialog的基类,所有对话框都从此派生
Author:xiaowei.han
Date:2016-9-11
Others:
Histroy:
*******************************************************************************/
#pragma once


class CBaseDialog : public CBCGPDialog
{
	DECLARE_DYNAMIC(CBaseDialog)
public:
	CBaseDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL,BOOL bEnableLayout = FALSE);
	~CBaseDialog(void);

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

