
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "BaseRibbonBar.h"
class CMainFrame : public CBCGPFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CBCGPMenuBar       m_wndMenuBar;
	CBCGPToolBar       m_wndToolBar;
	CBCGPStatusBar     m_wndStatusBar;
	CBCGPToolBarImages m_UserImages;
	//CBCGPRibbonStatusBar m_wndStatusBar;
protected:
	void CreateRibbonBar(void);
	//增加开始Category
	void AddStartCategory(void);
	//增加扩展Category
	void AddExternCategory(void);
	//增加页面Category
	void AddPageCategory(void);
	//增加仪表Category
	void AddDeviceCategory(void);
	//增加表格的category
	void AddGridCategory(void);
	//增加图表的category
	void AddDiagramCategory(void);
	//添加传感器和数据的面板
	void AddSensorDataPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//添加页面元素面板
	void AddStartPageElementPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//添加页面元素面板
	void AddPageElementPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//添加进行试验
	void AddRunTestPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//添加回访试验
	void AddReplyTestPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//添加其他软件面板
	void AddOtherSoftWarePanel(CBCGPRibbonCategory* pCategory);
	//添加页面面板
	void AddPagePanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//增加选项面板
	void AddOptionPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//增加校准面板
	void AddFixedPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//增加表格选项面板
	void AddGridOptionPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//增加剪贴板面板
	void AddGridClipBoardPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//增加格子面板
	void AddGridCellPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//增加数据面板
	void AddGridDataPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//增加报告面板
	void AddGridReportPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//增加图标选项面板
	void AddDiagramOptionPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//增加缩放面板
	void AddDiagramZoomPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//增加分析面板
	void AddDiagramAnalyzePanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//增加报告面板
	void AddDiagramReportPanel(CBCGPRibbonCategory* pCategory,int& nBaseImageIndex);
	//添加额外的内容
	void AddAdditionalContent(void);
	//创建状态栏
	void CreateStatusBar(void);
	//加载位图
	void LoadRibbonBitmap(void);
	//加载开始Ribbon页的图片
	void LoadStartPageRibbonBitmap(void);
	//加载页面的Ribbon页的图片
	void LoadPageRibbonBitmap(void);
	//加载表格的Ribbon页的图片
	void LoadGridPageRibbonBitmap(void);
	//加载仪表的Ribbon页的图片
	void LoadDevicePageRibbonBitmap(void);
	//加载图标的Ribbon页的突变
	void LoadDiagramPageRibbonBitmap(void);
	//加载MainCategory图标
	void LoadMainCategoryBitmap(void);
protected:
	CBaseRibbonBar m_wndRibbonBar;
	CBCGPRibbonMainButton m_MainButton;
	//仪表的Category
	CBCGPRibbonCategory* m_pDeviceCategory;
	//图表的Category
	CBCGPRibbonCategory* m_pDiagramCategory;
	//表格的Category
	CBCGPRibbonCategory* m_pGridCategory;
	//开始Ribbon页的Large位图
	CBCGPToolBarImages m_RibbonStartPageLargeBitmap;
	//开始Ribbon页的Small位图
	CBCGPToolBarImages m_RibbonStartPageSmallBitmap;
	//页面Ribbon页的Large位图
	CBCGPToolBarImages m_RibbonPageLargeBitmap;
	//页面Ribbon页的Small位图
	CBCGPToolBarImages m_RibbonPageSmallBitmap;
	//表格Ribbon页的Large位图
	CBCGPToolBarImages m_RibbonGridPageLargeBitmap;
	//表格Ribbon页的Small位图
	CBCGPToolBarImages m_RibbonGridPageSmallBitmap;
	//仪表Ribbon页的Large位图
	CBCGPToolBarImages m_RibbonDevicePageLargeBitmap;
	//仪表Ribbon页的Small位图
	CBCGPToolBarImages m_RibbonDevicePageSmallBitmap;
	//图表Ribbon页的Large位图
	CBCGPToolBarImages m_RibbonDiagramPageLargeBitmap;
	//图表Ribbon页的Small位图
	CBCGPToolBarImages m_RibbonDiagramPageSmallBitmap;
	//MainCategory的Large位图
	CBCGPToolBarImages m_MainCategoryLargeBitmap;
	//MainCategory的Small位图
	CBCGPToolBarImages m_MainCategorySmallBitmap;
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	afx_msg void HandleRadioClick(UINT nID);
	afx_msg void OnUpdateRadioButton(CCmdUI* pCmdUI);
#if 0
protected:
	//Ribbon消息相应函数
	//选传感器
	afx_msg void HandleSelctSensor(void) {}
	//采集参数
	afx_msg void HandleCaptureParam(void) {}
	//数据配置
	afx_msg void HandleConfigData(void) {}
	
	//添加表格
	afx_msg void HandleAddTable(void) {}
	//添加图
	afx_msg void HandleAddImage(void) {}
	//添加仪表
	afx_msg void HandleAddDevice(void) {}
	//删除元素
	afx_msg void HandleDelElement(void) {}


	//开始
	afx_msg void HandleStart(void) {}
	//记点
	afx_msg void HandleRecordPoint(void) {}
	//调零
	afx_msg void HandleFixedZero(void) {}
	//校准
	afx_msg void HandleCalibration(void) {}
	//取消调零和校准
	afx_msg void HandleCancelZeroAndCalibration(void) {}

	//开始回放
	afx_msg void HandleStartReplay(void) {}
	//暂停回放
	afx_msg void HandlePauseReplay(void) {}
	//选择数据组
	afx_msg void HandleSelectDataArray(void) {}
	//选择速度
	afx_msg void HandleSelectSpeed(void) {}

	//示波器
	afx_msg void HandleOscilloscope(void) {}
	//2维运动合成与分解
	afx_msg void Handle2DComposeAndDecompose(void) {}

	//添加页面
	afx_msg void HandleAddPage(void) {}
	//删除页面
	afx_msg void HandleDelPage(void) {}
	//页面名称
	afx_msg void HandlePageName(void) {}
	//跳转页面
	afx_msg void HandleGotoPage(void) {}

	//选项
	afx_msg void HandleOption(void) {}
	//数值风格
	afx_msg void HandleNumStyle(void) {}
	//表盘风格
	afx_msg void HandleWatchStyle(void) {}
	//下一数据列
	afx_msg void HandleNextDataColumn(void) {}
#endif
protected:
	UINT m_nCurrentRadioID;
public:
	afx_msg void OnMenuEnglish();
	afx_msg void OnUpdateMenuEnglish(CCmdUI *pCmdUI);
	afx_msg void OnMenuChinese();
	afx_msg void OnUpdateMenuChinese(CCmdUI *pCmdUI);


protected:
	CBCGPSplitterWnd m_SpliterWnd;
	CBCGPSplitterWnd m_SpliterWndSecond;
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	CCreateContext* m_pContext;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();


	afx_msg void EnableStatusBar(void) {}

	afx_msg LRESULT NotifyActiveWnd(WPARAM wp,LPARAM lp);


private:
	void ShowRibbonCatagory(CBCGPRibbonCategory* pCatagory,bool bShow = false);

	//设备上线的回调
	afx_msg LRESULT NotifyDeviceOnOrOff(WPARAM wp,LPARAM lp);

};


