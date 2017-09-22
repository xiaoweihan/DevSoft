#pragma once
#include "BaseDialog.h"
#include "WidgetLayout.h"
#include "Type.h"
// CDlgDiagramPanel 对话框
class ChartFigureDlg;
class CDlgDiagramPanel : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgDiagramPanel)

public:
	CDlgDiagramPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDiagramPanel();

// 对话框数据
	enum { IDD = IDD_DLG_DIAGRAM };
	void addPanel();
	void delPanel(CWnd* pDlg);

	//获取Grid的个数
	int GetWidgetNum(void) const;
private:
	//布局器
	CWidgetLayout m_WidgetLayout;
	//窗口
	std::vector<ChartFigureDlg*> m_vecPanel;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	//调整面板位置
	void AdjustPanelLayout(int nWidth,int nHeight);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//修改显示面板个数的改变
	void NotifyDisplayPanelChange(const LP_SENSOR_TYPE_KEY pSensor,int nFlag);
	//通知相应的控件开始刷新
	void NotifyControlsStartRefresh();
	//通知相应的控件停止刷新
	void NotifyControlsStopRefresh();
};
