#pragma once


// CDlgTabPanel 对话框
#include "WidgetLayout.h"
class CBaseDialog;
class CDlgTabPanel : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgTabPanel)

public:
	CDlgTabPanel(const CString&strPageName,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTabPanel();

	static const int PANEL_NUM = 3;
// 对话框数据
	enum { IDD = IDD_DLG_PANEL };

	enum 
	{
		GRID_INDEX = 0,
		DEVICE_INDEX,
		DIAGRAM_INDEX
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void SetPageName(const CString& strPageName);

	CString GetPageName(void) const;

	//获取Grid数量
	int GetGridNum(void) const;

	void SetGridNum(int nGridNum);

	//获取Device数量
	int GetDeviceNum(void) const;

	void SetDeviceNum(int nDeviceNum);

	//获取Diagram数量
	int GetDiagramNum(void) const;

	void SetDiagramNum(int DiagramNum);

	//添加表格
	void AddGrid();
	//添加图表
	void AddChart();
	//添回仪表
	void AddGauge();
	//删除窗格
	void DelWnd(CWnd* dlg);
	void SetActive(int type);
	//
	void SetActiveDlg(CWnd* dlg);
	CWnd* GetActiveDlg();
	//准备显示元素
	void PrepareDisplayElement(void);
protected:
	//创建面板
	void CreatePanel(void);

	//销毁面板
	void DestroyPanel(void);

	//调整面板位置
	void AdjustPanelLayout(int nWidth,int nHeight);

private:
	//面板
	CBaseDialog* m_pPanel[PANEL_NUM];

	//布局器
	CWidgetLayout m_WidgetLayout;

	//页的名称
	CString m_strPageName;

	//Grid的数量
	int m_nGridNum;

	//Device的数量
	int m_nDeviceNum;

	//Diagram的数量
	int m_nDiagramNum;

	class CWnd* m_pActiveDlg;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void PostNcDestroy();
};
