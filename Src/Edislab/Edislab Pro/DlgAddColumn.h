#pragma once


// CDlgAddColumn 对话框
#include "BaseDialog.h"
#include "ConcreteTabWnd.h"
class CDlgAddColumn : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAddColumn)

public:
	CDlgAddColumn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddColumn();

// 对话框数据
	enum { IDD = IDD_DLG_ADD_COLUMN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


private:
	void InitDisplay(void);

private:
	//Tab空间
	CConcreteTabWnd m_TabWnd;
};
