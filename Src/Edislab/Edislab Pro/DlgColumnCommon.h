#pragma once


// CDlgColumnCommon 对话框
#include "BaseDialog.h"
class CDlgColumnCommon : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgColumnCommon)

public:
	CDlgColumnCommon(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgColumnCommon();

// 对话框数据
	enum { IDD = IDD_DLG_COLUMN_COMMON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
