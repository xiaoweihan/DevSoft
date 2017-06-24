#pragma once

#include "BaseDialog.h"
// CDlgDataGroupProperty 对话框

class CDlgDataGroupProperty : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgDataGroupProperty)

public:
	CDlgDataGroupProperty(const CString& strGroupName,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDataGroupProperty();

// 对话框数据
	enum { IDD = IDD_DLG_DATA_GROUP_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	void InitDisplay();

	CString m_strGroupName;
public:
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();

	CString GetGroupName(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
