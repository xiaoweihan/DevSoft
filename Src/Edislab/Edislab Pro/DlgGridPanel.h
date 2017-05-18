#pragma once

#include "BaseDialog.h"
#include "WidgetLayout.h"
// CDlgGridPanel 对话框

class CDlgGridPanel : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgGridPanel)

public:
	CDlgGridPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGridPanel();

// 对话框数据
	enum { IDD = IDD_DLG_GRID };

	void addPanel();
	void delPanel(CWnd* pDlg);

	//获取Grid的个数
	int GetWidgetNum(void) const;

private:
	//布局器
	CWidgetLayout m_WidgetLayout;
	//窗口
	std::vector<CWnd*> m_vecPanel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
protected:
#if 0
	//创建面板
	void CreatePanel(void);
	//销毁面板
	void DestroyPanel(void);
#endif
	//调整面板位置
	void AdjustPanelLayout(int nWidth,int nHeight);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
