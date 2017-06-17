#pragma once

// DlgChartSet dialog
#include "ChartXY.h"
#include "BaseDialog.h"
class DlgChartSet : public CBaseDialog
{
	DECLARE_DYNAMIC(DlgChartSet)

public:
	DlgChartSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgChartSet();

// Dialog Data
	enum { IDD = IDD_DLG_CHART_SET };
	int m_nXID;//x轴ID
	std::set<int> m_setShowID;//显示的ID
	ChartType	m_eChartType;//显示方式
	LineStyle	m_eLineStyle;//线的方式
	MoveStyle	m_eMoveStyle;//实验时的方式

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CBCGPComboBox m_combX;
	CBCGPTreeCtrl m_treeY;
	CBCGPComboBox m_combMoveStyle;
	CBCGPComboBox m_combChartType;
	CBCGPComboBox m_combLineStyle;
	afx_msg void OnBnClickedOk();
};
