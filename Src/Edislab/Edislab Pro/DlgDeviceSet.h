#pragma once
#include "DoubleEdit.h"
#include "afxwin.h"


// DlgDeviceSet dialog

class DlgDeviceSet : public CDialog
{
	DECLARE_DYNAMIC(DlgDeviceSet)

public:
	DlgDeviceSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgDeviceSet();
	//获取要显示的数据列ID
	int getDataColumnID();
	void setDataColumnID(int ID);
	//警告启用?
	bool getWarningState();
	void setWarningState(bool warning);
	//最大最小0:超过 1:降到
	int getWarningType();
	void setWarningType(int type);
	//警告值
	double getWarningValue();
	void setWarningValue(double val);
// Dialog Data
	enum { IDD = IDD_DLG_GAUGE_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DoubleEdit m_edtValue;
//	afx_msg void OnBnHotItemChangeCheckWarning(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckWarning();
	double m_warningValue;
	BOOL m_bWarning;
	int m_nWarningType;
	int m_nDataID;
	CComboBox m_combWarningTYpe;
	CComboBox m_combDataID;
	afx_msg void OnBnClickedOk();
};
