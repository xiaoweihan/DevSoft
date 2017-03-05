#pragma once

#include "BaseDialog.h"
// CSplitBar 对话框

class CSplitBar : public CBaseDialog
{
	DECLARE_DYNAMIC(CSplitBar)

public:
	CSplitBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSplitBar();

// 对话框数据
	enum { IDD = IDD_DLG_SPLI_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	void SetSplitBarBkColor(COLORREF BkColor);

private:
	//背景色
	COLORREF m_BkColor;
};
