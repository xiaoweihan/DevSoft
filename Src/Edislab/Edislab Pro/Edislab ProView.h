
// Edislab ProView.h : CEdislabProView 类的接口
//

#pragma once
#include <string>
#include "ConcreteTabWnd.h"

class CDlgTabPanel;
class CEdislabProDoc;
class CEdislabProView : public CView
{
protected: // 仅从序列化创建
	CEdislabProView();
	DECLARE_DYNCREATE(CEdislabProView)


// 特性
public:
	CEdislabProDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CEdislabProView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	void CreateDisplayTabWnd(void);


protected:
	CConcreteTabWnd m_TabWnd;
public:
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();

	afx_msg void HandleAllRibbonCommand(UINT nCommand);
	afx_msg void HandleUpdateAllRibbonCommand(CCmdUI *pCmdUI);

public:
	//增加新的页
	bool AddNewTabWnd(const CString& strTabWndName,int nGridNum,int nDiagramNum,int nDeviceNum);
	//获取当前活动页的指针
	CDlgTabPanel* GetCurrentPage(void);
	//获取当前活动页的名称
	CString GetCurrentPageName(void);
	//设置当前活动页的名称
	void SetCurrentPageName(const CString& strPageName);
	//获取页面数
	int GetPageNum(void);
	//设置最后一页为活动页
	void SetActivePage(int nPageIndex);
	//删除当前页
	void DeleteCurrentPage(void);
	//获取表的个数
	int GetGridNum(void);
	//获取仪表的个数
	int GetDeviceNum(void);
	//获取图的个数
	int GetDiagramNum(void);

	void AddGrid(void);

	void AddDiagram(void);

	void AddDevice(void);
public:
	static CEdislabProView* GetCurrentView(void);



	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();

	//通知刷新所有Tab页中的grid信息
	void NotifyDetectDevice(const std::string& strDeviceName,int nOnFlag);

	//通知grid刷新行
	void NotifyGridChangeRows(int nRows);

	//通知相应的控件开始刷新
	void NotifyControlsStartRefresh();

	//通知相应的控件停止刷新
	void NotifyControlsStopRefresh();

	//删除元素
	void DeleteElement();

	//通知Grid的分组信息改变
	void NotifyGridGroupInfoChange();
};

#ifndef _DEBUG  // Edislab ProView.cpp 中的调试版本
inline CEdislabProDoc* CEdislabProView::GetDocument() const
   { return reinterpret_cast<CEdislabProDoc*>(m_pDocument); }
#endif

