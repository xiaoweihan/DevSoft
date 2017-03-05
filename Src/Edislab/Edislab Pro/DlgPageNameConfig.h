#pragma once

#include "BaseDialog.h"
#include "afxwin.h"
// CDlgPageNameConfig 对话框

class CDlgPageNameConfig : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgPageNameConfig)

public:
	CDlgPageNameConfig(const CString& strPageName,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPageNameConfig();

// 对话框数据
	enum { IDD = IDD_DLG_PAGE_NAME_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CBCGPEdit m_EditPageName;

	CString m_strPageName;
public:
	virtual BOOL OnInitDialog();

private:
	void InitDisplay(void);
public:
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();

public:
	CString GetPageName(void) const;
};
