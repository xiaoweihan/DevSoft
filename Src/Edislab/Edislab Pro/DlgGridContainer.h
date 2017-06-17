#pragma once
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/atomic.hpp>
#include "CustomGrid.h"
// CDlgGridContainer 对话框

class CDlgGridContainer : public CBCGPDialog
{
	DECLARE_DYNAMIC(CDlgGridContainer)

public:
	CDlgGridContainer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgGridContainer();

// 对话框数据
	enum { IDD = IDD_DLG_GRID_CONTAINER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CCustomGrid m_DisplayGrid;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	void NotifyDetectSensor(const std::string& strDeviceName,int nOnFlag);
	//通知行数改变
	void NotifyGridChangeRows(int nRows);
	//通知相应的控件开始刷新
	void NotifyControlsStartRefresh();
	//通知相应的控件停止刷新
	void NotifyControlsStopRefresh();
	//通知Grid的分组信息改变
	void NotifyGridGroupInfoChange();
	afx_msg LRESULT NotifyGridClickRButton(WPARAM wp,LPARAM lp);
private:
	void RefreshGrid(void);
public:
	afx_msg void OnPaint();

private:
	//模拟产生数据的线程
	boost::shared_ptr<boost::thread> m_pYieldDataThread;
	boost::atomic_bool m_bLoop;
	void YieldDataProc(void);
public:
	afx_msg void OnMenuGridOption();
	//begin add by xiaowei.han
	afx_msg LRESULT NotifyActive(WPARAM wp,LPARAM lp);
private:
	BOOL m_bActiveFlag;
	//end add by xiaowei.han
public:
	afx_msg void OnMenuGridCopy();
	afx_msg void OnMenuGridPaste();
	afx_msg void OnMenuGridDelCell();
	afx_msg void OnMenuGridFirstRow();
	afx_msg void OnMenuGridLastRow();
	afx_msg void OnMenuGridAddColumn();


	afx_msg LRESULT NotifyGridOption(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridCopy(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridPaste(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridFirstRow(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridLastRow(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridInsertRow(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridDelCell(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridAddDataColumn(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridYieldData(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridCalculate(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridClearCellData(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridSaveAsExcel(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridPrint(WPARAM wp,LPARAM lp);
	afx_msg LRESULT NotifyGridPrintPreview(WPARAM wp,LPARAM lp);
	afx_msg void OnMenuGridInsertRow();
	afx_msg void OnMenuGridMoveForwardColumn();
	afx_msg void OnMenuGridMoveBackColumn();
	afx_msg void OnMenuGridYieldData();
	afx_msg void OnMenuGridSum();
	afx_msg void OnMenuGridAvg();
	afx_msg void OnMenuGridMax();
	afx_msg void OnMenuGridMin();
	afx_msg void OnMenuGridClearData();
	afx_msg void OnMenuGridDelElement();
	afx_msg void OnMenuGridSaveAsExcel();
	afx_msg void OnMenuGridPrintView();
	afx_msg void OnMenuGridPrint();
};
