#pragma once
#include "BaseDialog.h"
class GaugeDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(GaugeDlg)

public:
	GaugeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~GaugeDlg();

// Dialog Data
	enum { IDD = IDD_DLG_GAUGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	void OnUpdateGauge();
	CBCGPCircularGaugeCtrl m_wndGauge;
	CBCGPTextGaugeImpl* m_pValueGauge;
	CBCGPTextGaugeImpl* m_pNameGauge;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

	void NextColumn();
	void setRange(double min, double max);
	void getRange(double& min, double& max);
	void setWarningValue(double min, double max);
	void getWarningValue(double& min, double& max);
	void setShowDataColumn(int ColumnID);
	int getShowDataColumn();
	void setValue(double value);
	double getValue();
	void setEnableWarning(bool warning);
	bool isEnableWarning();

	//保存数据
	int saveData();
	//读取数据
	int readData();
	void updateData(class CGlobalDataManager* dbMgr);
private:
	//上下限
	double minRange;
	double maxRange;
	double maxWarningValue;
	double minWarningValue;
	//显示数据列的ID号
	int dataColumnID;
	bool enableWarning;
	CString m_strTitle;
	CString m_strUnit;
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnGaugeSet();
	afx_msg void OnGaugeDelete();
	afx_msg void OnGaugeZero();
	afx_msg void OnUpdateGaugeZero(CCmdUI *pCmdUI);
	afx_msg void OnGaugeCorrect();
	afx_msg void OnUpdateGaugeCorrect(CCmdUI *pCmdUI);
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//begin add by xiaowei.han

	afx_msg LRESULT NotifyActive(WPARAM wp,LPARAM lp);
	
private:
	BOOL m_bActiveFlag;
	//end add by xiaowei.han
};
