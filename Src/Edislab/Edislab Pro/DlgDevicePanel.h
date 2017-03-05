#pragma once
#include "BaseDialog.h"
#include "WidgetLayout.h"
#include "GaugeManager.h"
// CDlgDevicePanel 对话框

class CDlgDevicePanel : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgDevicePanel)

public:
	CDlgDevicePanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDevicePanel();

// 对话框数据
	enum { IDD = IDD_DLG_DEVICE };
	void addPanel();
	void delPanel(CWnd* pDlg);

	//获取Grid的个数
	int GetWidgetNum(void) const;

private:
	//数据理器
	GaugeManager m_dataManager;
	//布局器
	CWidgetLayout m_WidgetLayout;
	//窗口
	std::vector<CBaseDialog*> m_vecPanel;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	//创建面板
	void CreatePanel(void);
	//销毁面板
	void DestroyPanel(void);
	//调整面板位置
	void AdjustPanelLayout(int nWidth,int nHeight);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
