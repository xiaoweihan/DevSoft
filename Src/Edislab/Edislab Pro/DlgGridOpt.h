#pragma once
#include "BaseDialog.h"
class CDlgGridOpt : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgGridOpt)

public:
	CDlgGridOpt(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGridOpt();

// 对话框数据
	enum { IDD = IDD_DLG_GRID_OPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	CBCGPTreeCtrl m_GridDisplayItemTree;

private:
	//初始化显示
	void InitDisplay(void);
public:
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnCancel();

	//设置显示选项
	//void SetDisplayInfo(const std::vector<GRID_DISPLAY_COLUMN_INFO>& GridDisplayArray);
	//获取显示选项
	//void GetDisplayInfo(std::vector<GRID_DISPLAY_COLUMN_INFO>& GridDisplayArray);
private:
	//std::vector<GRID_DISPLAY_COLUMN_INFO> m_GridDisplayArray;
};
