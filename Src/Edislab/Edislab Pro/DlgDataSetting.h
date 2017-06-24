#pragma once
#include <vector>
#include "BaseDialog.h"
#include "CustomTreeCtrl.h"
typedef struct _default_group_name
{
	CString strName;
	bool bUse;

	_default_group_name()
	{
		bUse = false;
	}
}DEFAULT_GROUP_NAME;

class CDlgDataSetting : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgDataSetting)

public:
	CDlgDataSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDataSetting();

// 对话框数据
	enum { IDD = IDD_DLG_DATA_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	void InitDisplay(void);

private:
	CCustomTreeCtrl m_Tree;
	CStatic m_TreeRect;
public:
	afx_msg void OnBnClickedBtnQuit();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnAddDataGroup();
	afx_msg void OnBnClickedBtnAddDataColumn();

	int GetTotalRootNum(void);

private:
	std::vector<DEFAULT_GROUP_NAME> m_strDefaultGroupNameArray;
public:
	afx_msg void OnBnClickedBtnOpt();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	afx_msg LRESULT NotifyTreeDBClick(WPARAM wp,LPARAM lp);
};
