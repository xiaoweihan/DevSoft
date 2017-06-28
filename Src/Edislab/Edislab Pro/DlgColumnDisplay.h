#pragma once


// CDlgColumnDisplay 对话框
#include "BaseDialog.h"
class CDlgColumnDisplay : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgColumnDisplay)

public:
	CDlgColumnDisplay(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgColumnDisplay();

// 对话框数据
	enum { IDD = IDD_DLG_COLUMN_DISPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
