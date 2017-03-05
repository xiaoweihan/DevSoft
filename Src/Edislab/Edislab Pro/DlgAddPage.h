#pragma once
#include "BaseDialog.h"
#include "Type.h"
class CDlgAddPage : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAddPage)

public:
	CDlgAddPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddPage();

// 对话框数据
	enum { IDD = IDD_DLG_ADD_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	ADD_PAGE_ELEMENT GetAddPageElement(void) const;

private:
	void InitDisplay(void);
	CBCGPEdit m_PageNameEdit;
	CComboBox m_GridCombox;
	CComboBox m_DiagramCombox;
	CComboBox m_DeviceCombox;
public:
	afx_msg void OnBnClickedRadioEmptyPage();
	afx_msg void OnBnClickedRadioCopyPage();
	afx_msg void OnBnClickedRadioCustomPage();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();

private:
	//处理增加
	bool HandleAddPage(void);

	//处理新增空白页
	bool HandleAddEmptyPage(const CString& strPageName);

	//处理新增复制当前页
	bool HandleAddCopyPage(const CString& strPageName);

	//处理新增自定义页
	bool HandleCustomPage(const CString& strPageName);

private:
	ADD_PAGE_ELEMENT m_AddPageElement;
};
