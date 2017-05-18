
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "MainFrm.h"
//#include "LogImpl.h"
#include "Msg.h"
#include "Utility.h"
//#include "GlobalDataManager.h"
#include "SerialPortService.h"
#include "Global.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define HAVE_PICTURE (1)
//Combox下拉框的宽度
const int DEFAULT_COMBOX_WIDTH = 75;
//定时器间隔
const int TIMER_GAP = 1000;
//定时器ID
const int TIMER_ID = 1;
//大图标的长度和宽度
const int LARGE_ICON_WIDTH = 32;
const int LARGE_ICON_HEIGHT = 32;
//小图标的长度和宽度
const int SMALL_ICON_WIDTH = 16;
const int SMALL_ICON_HEIGHT = 16;
//默认的透明色
const COLORREF TRANS_COLOR = RGB(0,0,0);

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_NOTIFY_ACTIVE_WND_TYPE,&CMainFrame::NotifyActiveWnd)
#if 0
	ON_COMMAND(ID_SELECT_SENSOR, &CMainFrame::HandleSelctSensor)
	ON_COMMAND(ID_COLLECT_PARAM, &CMainFrame::HandleCaptureParam)
	ON_COMMAND(ID_CONFIG_DATA, &CMainFrame::HandleConfigData)

	ON_COMMAND(ID_ADD_TABLE, &CMainFrame::HandleAddTable)
	ON_COMMAND(ID_ADD_IMAGE, &CMainFrame::HandleAddImage)
	ON_COMMAND(ID_ADD_DEVICE, &CMainFrame::HandleAddDevice)
	ON_COMMAND(ID_DEL_ELEMENT, &CMainFrame::HandleDelElement)

	ON_COMMAND(ID_START, &CMainFrame::HandleStart)
	ON_COMMAND(ID_RECORD_POINT, &CMainFrame::HandleRecordPoint)
	ON_COMMAND(ID_ZERO, &CMainFrame::HandleFixedZero)
	ON_COMMAND(ID_CALIBRATION, &CMainFrame::HandleCalibration)
	ON_COMMAND(ID_CANCEL_ZERO_AND_CALIBRATION, &CMainFrame::HandleCancelZeroAndCalibration)

	ON_COMMAND(ID_START_REPLAY, &CMainFrame::HandleStartReplay)
	ON_COMMAND(ID_PAUSE_REPLAY, &CMainFrame::HandlePauseReplay)
	ON_COMMAND(ID_SELECT_ARRAY, &CMainFrame::HandleSelectDataArray)
	ON_COMMAND(ID_SELECT_SPEED, &CMainFrame::HandleSelectSpeed)

	ON_COMMAND(ID_ADD_PAGE, &CMainFrame::HandleAddPage)
	ON_COMMAND(ID_DEL_PAGE, &CMainFrame::HandleDelPage)
	ON_COMMAND(ID_SET_PAGE_NAME, &CMainFrame::HandlePageName)
	ON_COMMAND(ID_GOTO_PAGE, &CMainFrame::HandleGotoPage)

	ON_COMMAND(ID_OPTION, &CMainFrame::HandleOption)
	ON_COMMAND(ID_NUM_STYLE, &CMainFrame::HandleNumStyle)
	ON_COMMAND(ID_WATCH_STYLE, &CMainFrame::HandleWatchStyle)
	ON_COMMAND(ID_NEXT_DATA_COLUMN, &CMainFrame::HandleNextDataColumn)


	ON_COMMAND(ID_OSCILLOSCOPE, &CMainFrame::HandleOscilloscope)
	ON_COMMAND(ID_2D_COMPOSE_AND_DECOMPOSE, &CMainFrame::Handle2DComposeAndDecompose)
#endif
	ON_COMMAND_RANGE(ID_AUTO_SELECT,ID_MANUAL_SELECT,&CMainFrame::HandleRadioClick)
	ON_UPDATE_COMMAND_UI_RANGE(ID_AUTO_SELECT,ID_MANUAL_SELECT,&CMainFrame::OnUpdateRadioButton)
	ON_COMMAND(ID_MENU_ENGLISH, &CMainFrame::OnMenuEnglish)
	ON_UPDATE_COMMAND_UI(ID_MENU_ENGLISH, &CMainFrame::OnUpdateMenuEnglish)
	ON_COMMAND(ID_MENU_CHINESE, &CMainFrame::OnMenuChinese)
	ON_UPDATE_COMMAND_UI(ID_MENU_CHINESE, &CMainFrame::OnUpdateMenuChinese)
	ON_COMMAND(ID_DISPLAY_CURRENT_TIME,&CMainFrame::EnableStatusBar)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器

	ID_STATUS_1,
	//ID_STATUS_2,
	ID_DISPLAY_CURRENT_TIME,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame():
m_nCurrentRadioID(0),
m_pDeviceCategory(NULL),
m_pDiagramCategory(NULL),
m_pGridCategory(NULL)
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
#if 0	
	CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
	CBCGPDockManager* pDockManager = GetDockManager();
	if (pDockManager != NULL)
	{
		pDockManager->AdjustBarFrames();
	}
	CBCGPTabbedControlBar::ResetTabs();
	RecalcLayout ();
	RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);

	CBCGPToolBar::SetSizes(CSize(32,32),CSize(24,24));
	CBCGPToolBar::SetMenuSizes(CSize (24,21),CSize(16,15));
	CBCGPMenuBar::EnableMenuShadows(FALSE);
	if (FALSE == m_wndMenuBar.Create(this))
	{
		CLogImpl::CreateInstance().Error_Log("未能创建菜单栏");
		return -1;     
	}
	if (NULL != m_wndMenuBar.GetSafeHwnd())
	{
		m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);
	}
	// 防止菜单栏在激活时获得焦点
	CBCGPPopupMenu::SetForceMenuFocus(FALSE);
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		CLogImpl::CreateInstance().Error_Log("未能创建工具栏");
		return -1;
	}
	CString strToolBarName;
	BOOL bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);
	// 允许用户定义的工具栏操作:
	CBCGPToolBar::EnableQuickCustomization();
	if (FALSE == m_wndStatusBar.Create(this))
	{
		CLogImpl::CreateInstance().Error_Log("未能创建状态栏");
		return -1;
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
#endif
	LoadRibbonBitmap();
	//创建Ribbon风格的状态栏
	CreateStatusBar();
	//创建RibbonBar
	CreateRibbonBar();
	SetTimer(TIMER_ID,TIMER_GAP,NULL);
	if (g_bAutoSelect)
	{	
		CSerialPortService::CreateInstance().StartSerialPortService();
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}
#endif //_DEBUG

//创建RibbonBar
void CMainFrame::CreateRibbonBar( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		m_wndRibbonBar.Create(this);
	}
	m_MainButton.SetImage(IDB_RIBBON_LOGO);
	m_MainButton.SetToolTipText (_T("File"));
	m_MainButton.SetText (_T("\nf"));
	m_wndRibbonBar.SetMainButton(&m_MainButton,CSize(48, 48));
	CBCGPRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(_T("文件"),IDB_RIBBON_BUTTON_SMALL,IDB_RIBBON_BUTTON_LARGE);
	if (NULL != pMainPanel)
	{
		int nIndex = 0;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_NEW, _T("新建(&N)"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_TYPICAL_TEST, _T("典型实验"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_OPEN, _T("打开(&O)"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_SAVE, _T("保存(&S)"), m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_SAVE_AS, _T("另存为(&A)"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_OUTPUT_TEST_REPORT, _T("输出实验报告"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_PRINT, _T("打印(&P)"),m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_PRINT_PREVIEW, _T("打印预览(&V)"), m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
		++nIndex;
		pMainPanel->Add (new CBCGPRibbonButton (ID_APP_EXIT, _T("退出(&E)"), m_MainCategoryLargeBitmap.ExtractIcon(nIndex),FALSE,m_MainCategorySmallBitmap.ExtractIcon(nIndex)));
	}
	pMainPanel->EnableCommandSearch (TRUE, _T("Search Command"), _T("Q"), 150);
	AddStartCategory();
	AddExternCategory();
	AddPageCategory();
	AddDeviceCategory();
	AddGridCategory();
	AddDiagramCategory();
	AddAdditionalContent();
}

//增加开始
void CMainFrame::AddStartCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}

	int nBaseImageIndex = 0;
	CBCGPRibbonCategory* pStartCategory = m_wndRibbonBar.AddCategory(_T("开始"),IDB_RIBBON_START_SMALL,IDB_RIBBON_START_LARGE);
	if (NULL != pStartCategory)
	{
		//添加传感器与数据面板
		AddSensorDataPanel(pStartCategory,nBaseImageIndex);
		//添加页面元素数据面板
		AddStartPageElementPanel(pStartCategory,nBaseImageIndex);
		//添加进行试验面板
		AddRunTestPanel(pStartCategory,nBaseImageIndex);
		//添加回放试验面板、
		AddReplyTestPanel(pStartCategory,nBaseImageIndex);
	}


}

//增加扩展
void CMainFrame::AddExternCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}
	CBCGPRibbonCategory* pExternCategory = m_wndRibbonBar.AddCategory(_T("扩展"),IDB_RIBBON_START_SMALL,IDB_RIBBON_START_LARGE);
	if (NULL != pExternCategory)
	{
		AddOtherSoftWarePanel(pExternCategory);
	}
}

//增加页面
void CMainFrame::AddPageCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}
	int nBaseImageIndex = 0;
	CBCGPRibbonCategory* pExternCategory = m_wndRibbonBar.AddCategory(_T("页面"),IDB_RIBBON_PAGE_ELEMENT_SMALL,IDB_RIBBON_PAGE_ELEMENT_LARGE);
	if (NULL != pExternCategory)
	{
		AddPagePanel(pExternCategory,nBaseImageIndex);
		AddPageElementPanel(pExternCategory,nBaseImageIndex);
	}
}

void CMainFrame::HandleRadioClick( UINT nID )
{
	m_nCurrentRadioID = nID;
}

void CMainFrame::AddSensorDataPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{

	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pSensorAndDataPanel = pCategory->AddPanel (_T("传感器和数据"));

		if (NULL != pSensorAndDataPanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SELECT_SENSOR, _T("选传感器"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SELECT_SENSOR, _T("选传感器"),1,1);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("选传感器"));
				pBtn->SetAlwaysLargeImage();
				pSensorAndDataPanel->Add(pBtn);
			}

			pSensorAndDataPanel->AddSeparator();
			CBCGPRibbonRadioButton* pRadioBtn = new CBCGPRibbonRadioButton(ID_AUTO_SELECT, _T("自动识别"));
			if (NULL != pRadioBtn)
			{
				pRadioBtn->SetToolTipText(_T("自动识别"));
				pSensorAndDataPanel->Add(pRadioBtn);
			}
			
			pRadioBtn = new CBCGPRibbonRadioButton(ID_MANUAL_SELECT, _T("手动选择"));
			if (NULL != pRadioBtn)
			{
				pRadioBtn->SetToolTipText(_T("手动选择"));
				pSensorAndDataPanel->Add(pRadioBtn);
			}
			
			pSensorAndDataPanel->AddSeparator();
#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_COLLECT_PARAM, _T("采集参数"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_COLLECT_PARAM, _T("采集参数"),1,1);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("采集参数"));
				pBtn->SetAlwaysLargeImage ();
				pSensorAndDataPanel->Add(pBtn);
			}
#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_CONFIG_DATA, _T("数据配置"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_CONFIG_DATA, _T("数据配置"),1,1);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("数据配置"));
				pBtn->SetAlwaysLargeImage ();
				pSensorAndDataPanel->Add(pBtn);
			}
		}
	}
}

void CMainFrame::AddStartPageElementPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pPageElementPanel = pCategory->AddPanel (_T("页面元素"));

		if (NULL != pPageElementPanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_TABLE, _T("添加表格"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_TABLE, _T("添加表格"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("添加表格"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_ADD_IMAGE, _T("添加图"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_ADD_IMAGE, _T("添加图"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("添加图"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_ADD_DEVICE, _T("添加仪表"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_ADD_DEVICE, _T("添加仪表"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("添加仪表"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_DEL_ELEMENT, _T("删除元素"),m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_DEL_ELEMENT, _T("删除元素"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("删除元素"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}
		}
	}
}

//添加页面元素
void CMainFrame::AddPageElementPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{


	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pPageElementPanel = pCategory->AddPanel (_T("页面元素"));

		if (NULL != pPageElementPanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_TABLE, _T("添加表格"),m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_TABLE, _T("添加表格"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("添加表格"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_ADD_IMAGE, _T("添加图"),m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_ADD_IMAGE, _T("添加图"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("添加图"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_ADD_DEVICE, _T("添加仪表"),m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_ADD_DEVICE, _T("添加仪表"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("添加仪表"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_DEL_ELEMENT, _T("删除元素"),m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_DEL_ELEMENT, _T("删除元素"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("删除元素"));
				pBtn->SetAlwaysLargeImage ();
				pPageElementPanel->Add(pBtn);
			}
		}
	}
}

void CMainFrame::AddRunTestPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{

	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pRunTestPanel = pCategory->AddPanel (_T("进行实验"));

		if (NULL != pRunTestPanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_START, _T("开始"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_START, _T("开始"),5,5);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("开始"));
				pBtn->SetAlwaysLargeImage ();
				pRunTestPanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_RECORD_POINT, _T("记点"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_RECORD_POINT, _T("记点"),5,5);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("记点"));
				pBtn->SetAlwaysLargeImage ();
				pRunTestPanel->Add(pBtn);
			}

			pRunTestPanel->AddSeparator();
#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_ZERO, _T("调零"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_ZERO, _T("调零"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("调零"));
				pBtn->SetAlwaysLargeImage ();
				pRunTestPanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_CALIBRATION, _T("校准"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_CALIBRATION, _T("校准"),6,6);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("校准"));
				pBtn->SetAlwaysLargeImage ();
				pRunTestPanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_CANCEL_ZERO_AND_CALIBRATION, _T("取消调零和校准"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_CANCEL_ZERO_AND_CALIBRATION, _T("取消调零和校准"),6,6);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("取消调零和校准"));
				pBtn->SetAlwaysLargeImage ();
				pRunTestPanel->Add(pBtn);
			}

		}
	}

}

void CMainFrame::AddReplyTestPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pReplayTestPanel = pCategory->AddPanel (_T("回放实验"));

		if (NULL != pReplayTestPanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_START_REPLAY, _T("开始回放"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_START_REPLAY, _T("开始回放"),7,7);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("开始回放"));
				pBtn->SetAlwaysLargeImage ();
				pReplayTestPanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_PAUSE_REPLAY, _T("暂停回放"), m_RibbonStartPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonStartPageSmallBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_PAUSE_REPLAY, _T("暂停回放"),5,5);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("暂停回放"));
				pBtn->SetAlwaysLargeImage ();
				pReplayTestPanel->Add(pBtn);
			}

			//添加回放设置
			CBCGPRibbonLabel* pLabel = new CBCGPRibbonLabel(_T("回放设置"));
			if (NULL != pLabel)
			{
				pReplayTestPanel->Add(pLabel);
			}


			CBCGPRibbonComboBox* pCombox = new CBCGPRibbonComboBox(ID_SELECT_ARRAY,FALSE,DEFAULT_COMBOX_WIDTH);

			if (NULL != pCombox)
			{
				pCombox->SetPrompt(_T("数据组"));

				pCombox->AddItem(_T("当前"));

				pReplayTestPanel->Add(pCombox);
			}

			pCombox = new CBCGPRibbonComboBox(ID_SELECT_SPEED,FALSE,DEFAULT_COMBOX_WIDTH);
			if (NULL != pCombox)
			{

				pCombox->SetPrompt(_T("速度"));

				CString strElement = _T("1/100x");;
				pCombox->AddItem(strElement);

				strElement = _T("1/50x");;
				pCombox->AddItem(strElement);

				strElement = _T("1/10x");;
				pCombox->AddItem(strElement);

				strElement = _T("1/6x");;
				pCombox->AddItem(strElement);

				strElement = _T("1/4x");;
				pCombox->AddItem(strElement);

				strElement = _T("1x");;
				pCombox->AddItem(strElement);

				strElement = _T("2x");;
				pCombox->AddItem(strElement);

				strElement = _T("4x");;
				pCombox->AddItem(strElement);

				strElement = _T("6x");;
				pCombox->AddItem(strElement);

				strElement = _T("8x");;
				pCombox->AddItem(strElement);

				strElement = _T("10x");;
				pCombox->AddItem(strElement);

				strElement = _T("50x");;
				pCombox->AddItem(strElement);

				strElement = _T("100x");;
				pCombox->AddItem(strElement);

				pReplayTestPanel->Add(pCombox);
			}
		
		}
	}
}

void CMainFrame::OnUpdateRadioButton( CCmdUI* pCmdUI )
{
	if (NULL != pCmdUI)
	{
		pCmdUI->SetRadio(m_nCurrentRadioID == pCmdUI->m_nID);
	}
	
}

void CMainFrame::AddOtherSoftWarePanel( CBCGPRibbonCategory* pCategory )
{

	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pOtherSoftWarePanel = pCategory->AddPanel (_T("其它软件"));

		if (NULL != pOtherSoftWarePanel)
		{
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OSCILLOSCOPE,_T("示波器"));

			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("示波器"));
				pOtherSoftWarePanel->Add(pBtn);
			}

			pBtn = new CBCGPRibbonButton(ID_2D_COMPOSE_AND_DECOMPOSE,_T("二维运动合成与分解"));
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("二维运动合成与分解"));
				pOtherSoftWarePanel->Add(pBtn);
			}

		}
	}
}


void CMainFrame::AddPagePanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL != pCategory)
	{
		CBCGPRibbonPanel* pPagePanel = pCategory->AddPanel (_T("页面"));

		if (NULL != pPagePanel)
		{
#ifdef HAVE_PICTURE
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_PAGE, _T("添加页面"), m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_PAGE, _T("添加页面"),1,1);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("添加页面"));
				pBtn->SetAlwaysLargeImage ();
				pPagePanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_DEL_PAGE, _T("删除页面"), m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_DEL_PAGE, _T("删除页面"),2,2);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("删除页面"));
				pBtn->SetAlwaysLargeImage ();
				pPagePanel->Add(pBtn);
			}

#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_SET_PAGE_NAME, _T("页面名称"), m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_SET_PAGE_NAME, _T("页面名称"),4,4);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("页面名称"));
				pBtn->SetAlwaysLargeImage ();
				pPagePanel->Add(pBtn);
			}


#ifdef HAVE_PICTURE
			pBtn = new CBCGPRibbonButton(ID_GOTO_PAGE, _T("跳转页面"), m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE,m_RibbonPageLargeBitmap.ExtractIcon(nBaseImageIndex));
			++nBaseImageIndex;
#else
			pBtn = new CBCGPRibbonButton(ID_GOTO_PAGE, _T("跳转页面"),3,3);
#endif
			if (NULL != pBtn)
			{
				pBtn->SetToolTipText(_T("跳转页面"));
				pBtn->SetAlwaysLargeImage ();
				pPagePanel->Add(pBtn);
			}
		}
	}
}

//增加仪表
void CMainFrame::AddDeviceCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}

	int nBaseImageIndex = 0;
	CBCGPRibbonCategory* pDeviceCategory = m_wndRibbonBar.AddCategory(_T("仪表"),IDB_RIBBON_DEVICE_SMALL,IDB_RIBBON_DEVICE_LARGE);
	if (NULL != pDeviceCategory)
	{
		m_pDeviceCategory = pDeviceCategory;
		AddOptionPanel(pDeviceCategory,nBaseImageIndex);
		AddFixedPanel(pDeviceCategory,nBaseImageIndex);
	}
}

//增加表格
void CMainFrame::AddGridCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}

	int nBaseImageIndex = 0;
	CBCGPRibbonCategory* pGridCategory = m_wndRibbonBar.AddCategory(_T("表格"),IDB_RIBBON_GRID_SMALL,IDB_RIBBON_GRID_LARGE);
	if (NULL != pGridCategory)
	{
		m_pGridCategory = pGridCategory;
		AddGridOptionPanel(pGridCategory,nBaseImageIndex);
		AddGridClipBoardPanel(pGridCategory,nBaseImageIndex);
		AddGridCellPanel(pGridCategory,nBaseImageIndex);
		AddGridDataPanel(pGridCategory,nBaseImageIndex);
		AddGridReportPanel(pGridCategory,nBaseImageIndex);
	}
}

//增加图表
void CMainFrame::AddDiagramCategory( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}

	int nBaseImageIndex = 0;
	CBCGPRibbonCategory* pDigramCategory = m_wndRibbonBar.AddCategory(_T("图表"),IDB_RIBBON_DIAGRAM_SMALL,IDB_RIBBON_DIAGRAM_LARGE);
	if (NULL != pDigramCategory)
	{
		m_pDiagramCategory = pDigramCategory;
		AddDiagramOptionPanel(pDigramCategory,nBaseImageIndex);
		AddDiagramZoomPanel(pDigramCategory,nBaseImageIndex);
		AddDiagramAnalyzePanel(pDigramCategory,nBaseImageIndex);
		AddDiagramReportPanel(pDigramCategory,nBaseImageIndex);
	}
}

//选项面板
void CMainFrame::AddOptionPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pOptionPanel = pCategory->AddPanel (_T("选项"));

	if (NULL != pOptionPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("选项"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("选项"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("选项"));
			pBtn->SetAlwaysLargeImage ();
			pOptionPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_NUM_STYLE, _T("数值风格"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_NUM_STYLE, _T("数值风格"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("数值风格"));
			pBtn->SetAlwaysLargeImage ();
			pOptionPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_WATCH_STYLE, _T("表盘风格"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_WATCH_STYLE, _T("表盘风格"),4,4);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("表盘风格"));
			pBtn->SetAlwaysLargeImage ();
			pOptionPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_NEXT_DATA_COLUMN, _T("下一数据列"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_NEXT_DATA_COLUMN, _T("下一数据列"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("下一数据列"));
			pBtn->SetAlwaysLargeImage ();
			pOptionPanel->Add(pBtn);
		}
	}
	
}

//校准面板
void CMainFrame::AddFixedPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pFixedPanel = pCategory->AddPanel (_T("校准"));

	if (NULL != pFixedPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ZERO, _T("调零"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ZERO, _T("调零"),0,0);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("调零"));
			pBtn->SetAlwaysLargeImage ();
			pFixedPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_CALIBRATION, _T("校准"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_CALIBRATION, _T("校准"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("校准"));
			pBtn->SetAlwaysLargeImage ();
			pFixedPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_CANCEL_ZERO_AND_CALIBRATION, _T("取消调零和校准"), m_RibbonDevicePageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDevicePageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_CANCEL_ZERO_AND_CALIBRATION, _T("取消调零和校准"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("取消调零和校准"));
			pBtn->SetAlwaysLargeImage ();
			pFixedPanel->Add(pBtn);
		}
	}

}

//选项面板
void CMainFrame::AddGridOptionPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pGridOptionPanel = pCategory->AddPanel(_T("选项"));

	if (NULL != pGridOptionPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("选项"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("选项"),0,0);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("选项"));
			pBtn->SetAlwaysLargeImage();
			pGridOptionPanel->Add(pBtn);
		}
	}

}

//剪贴板面板
void CMainFrame::AddGridClipBoardPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{

	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pGridClipBoardPanel = pCategory->AddPanel(_T("剪贴板"));

	if (NULL != pGridClipBoardPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_COPY, _T("复制"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_COPY, _T("复制"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("复制"));
			pBtn->SetAlwaysLargeImage();
			pGridClipBoardPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_PASTE, _T("黏贴"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_PASTE, _T("黏贴"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("黏贴"));
			pBtn->SetAlwaysLargeImage();
			pGridClipBoardPanel->Add(pBtn);
		}
	}
}

//格子面板
void CMainFrame::AddGridCellPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pGridCellPanel = pCategory->AddPanel(_T("格子"));

	if (NULL != pGridCellPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_FIRST_ROW, _T("第一行"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_FIRST_ROW, _T("第一行"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("第一行"));
			pBtn->SetAlwaysLargeImage();
			pGridCellPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_LAST_ROW, _T("最后行"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_LAST_ROW, _T("最后行"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("最后行"));
			pBtn->SetAlwaysLargeImage();
			pGridCellPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_INSERT_ROW, _T("插入行"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_INSERT_ROW, _T("插入行"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("插入行"));
			pBtn->SetAlwaysLargeImage();
			pGridCellPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_DEL_CELL, _T("删除格子"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_DEL_CELL, _T("删除格子"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("删除格子"));
			pBtn->SetAlwaysLargeImage();
			pGridCellPanel->Add(pBtn);
		}

	}
}

//数据面板
void CMainFrame::AddGridDataPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pGridDataPanel = pCategory->AddPanel(_T("数据"));

	if (NULL != pGridDataPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_DATA_COLUMN, _T("增加数据列"),m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_ADD_DATA_COLUMN, _T("增加数据列"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("增加数据列"));
			pBtn->SetAlwaysLargeImage();
			pGridDataPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_YIELD_DATA, _T("生成数据"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_YIELD_DATA, _T("生成数据"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("生成数据"));
			pBtn->SetAlwaysLargeImage();
			pGridDataPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_CALCULATE, _T("运算"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_CALCULATE, _T("运算"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("运算"));
			pBtn->SetAlwaysLargeImage();
			pGridDataPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_CLEAR_CHILD_DATA, _T("清除子数据"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_CLEAR_CHILD_DATA, _T("清除子数据"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("清除子数据"));
			pBtn->SetAlwaysLargeImage();
			pGridDataPanel->Add(pBtn);
		}
	}
}

//报告面板
void CMainFrame::AddGridReportPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pGridReportPanel = pCategory->AddPanel(_T("报告"));

	if (NULL != pGridReportPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SAVE_EXCEL, _T("存储为Excel"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SAVE_EXCEL, _T("存储为Excel"),4,4);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("存储为Excel"));
			pBtn->SetAlwaysLargeImage();
			pGridReportPanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_PRINT, _T("打印"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_PRINT, _T("打印"),4,4);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("打印"));
			pBtn->SetAlwaysLargeImage();
			pGridReportPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_PRINT_PREVIEW, _T("打印预览"), m_RibbonGridPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonGridPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_PRINT_PREVIEW, _T("打印预览"),4,4);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("打印预览"));
			pBtn->SetAlwaysLargeImage();
			pGridReportPanel->Add(pBtn);
		}
	}
}

//增加选项面板
void CMainFrame::AddDiagramOptionPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}
	CBCGPRibbonPanel* pDiagramOptionPanel = pCategory->AddPanel(_T("选项"));

	if (NULL != pDiagramOptionPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("选项"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_OPTION, _T("选项"),0,0);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("选项"));
			pBtn->SetAlwaysLargeImage();
			pDiagramOptionPanel->Add(pBtn);
		}

		//增加分离器
		pDiagramOptionPanel->AddSeparator();

		CBCGPRibbonRadioButton* pRadioBtn = new CBCGPRibbonRadioButton(ID_DRAG_MODE, _T("拖动模式"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("拖动模式"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pRadioBtn = new CBCGPRibbonRadioButton(ID_SELECT_MODE, _T("选择模式"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("选择模式"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pDiagramOptionPanel->AddSeparator();

		pRadioBtn = new CBCGPRibbonRadioButton(ID_AUTO_SCROLL, _T("自动滚屏"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("自动滚屏"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pRadioBtn = new CBCGPRibbonRadioButton(ID_AUTO_ZOOM, _T("自动缩屏"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("自动缩屏"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pRadioBtn = new CBCGPRibbonRadioButton(ID_NO_SCROLL, _T("不滚屏"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("不滚屏"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pDiagramOptionPanel->AddSeparator();

		pRadioBtn = new CBCGPRibbonRadioButton(ID_LINE, _T("连线"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("连线"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pRadioBtn = new CBCGPRibbonRadioButton(ID_POINT, _T("点"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("点"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

		pRadioBtn = new CBCGPRibbonRadioButton(ID_POINT_TO_LINE, _T("点连线"));
		if (NULL != pRadioBtn)
		{
			pRadioBtn->SetToolTipText(_T("点连线"));
			pDiagramOptionPanel->Add(pRadioBtn);
		}

	}

}

//增加缩放面板
void CMainFrame::AddDiagramZoomPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pDiagramZoomPanel = pCategory->AddPanel(_T("缩放"));

	if (NULL != pDiagramZoomPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_100_PERCENT, _T("100%"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_100_PERCENT, _T("100%"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("100%"));
			pBtn->SetAlwaysLargeImage();
			pDiagramZoomPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_ZOOM_IN, _T("放大"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_ZOOM_IN, _T("放大"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("放大"));
			pBtn->SetAlwaysLargeImage();
			pDiagramZoomPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_ZOOM_OUT, _T("缩小"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_ZOOM_OUT, _T("缩小"),1,1);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("缩小"));
			pBtn->SetAlwaysLargeImage();
			pDiagramZoomPanel->Add(pBtn);
		}
	}
}

//增加分析面板
void CMainFrame::AddDiagramAnalyzePanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pDiagramAnalyzePanel = pCategory->AddPanel(_T("分析"));

	if (NULL != pDiagramAnalyzePanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_VIEW, _T("查看"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_VIEW, _T("查看"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("查看"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_TANGENT, _T("切线"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_TANGENT, _T("切线"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("切线"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_INTEGRAL, _T("积分"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_INTEGRAL, _T("积分"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("积分"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_STATISTICS, _T("统计"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_STATISTICS, _T("统计"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("统计"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}


#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_LINEAR_FITTING, _T("直线拟合"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_LINEAR_FITTING, _T("直线拟合"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("直线拟合"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_FITTING, _T("拟合"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_FITTING, _T("拟合"),2,2);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("拟合"));
			pBtn->SetAlwaysLargeImage();
			pDiagramAnalyzePanel->Add(pBtn);
		}
	}
}

//增加报告面板
void CMainFrame::AddDiagramReportPanel( CBCGPRibbonCategory* pCategory,int& nBaseImageIndex )
{
	if (NULL == pCategory)
	{
		return;
	}

	CBCGPRibbonPanel* pDiagramReportPanel = pCategory->AddPanel(_T("报告"));

	if (NULL != pDiagramReportPanel)
	{
#ifdef HAVE_PICTURE
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SAVE_IMAGE, _T("保存图片"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_SAVE_IMAGE, _T("保存图片"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("保存图片"));
			pBtn->SetAlwaysLargeImage();
			pDiagramReportPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_EXPORT_EXPERIMENT_REPORT, _T("导出到实验报告"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_EXPORT_EXPERIMENT_REPORT, _T("导出到实验报告"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("导出到实验报告"));
			pBtn->SetAlwaysLargeImage();
			pDiagramReportPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_PRINT, _T("打印"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_PRINT, _T("打印"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("打印"));
			pBtn->SetAlwaysLargeImage();
			pDiagramReportPanel->Add(pBtn);
		}

#ifdef HAVE_PICTURE
		pBtn = new CBCGPRibbonButton(ID_PRINT_PREVIEW, _T("打印预览"), m_RibbonDiagramPageLargeBitmap.ExtractIcon(nBaseImageIndex),FALSE, m_RibbonDiagramPageSmallBitmap.ExtractIcon(nBaseImageIndex));
		++nBaseImageIndex;
#else
		pBtn = new CBCGPRibbonButton(ID_PRINT_PREVIEW, _T("打印预览"),3,3);
#endif
		if (NULL != pBtn)
		{
			pBtn->SetToolTipText(_T("打印预览"));
			pBtn->SetAlwaysLargeImage();
			pDiagramReportPanel->Add(pBtn);
		}
	}
}

void CMainFrame::AddAdditionalContent( void )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}

	CBCGPRibbonButton* pBtn = new CBCGPRibbonButton(ID_EXPERIMENT_GUIDE, _T("实验指导"));

	if (NULL != pBtn)
	{
		pBtn->SetToolTipText(_T("实验指导"));
		m_wndRibbonBar.AddToTabs(pBtn);
	}

	//添加语言

	pBtn = new CBCGPRibbonButton((UINT)-1,_T("语言"));

	if (NULL != pBtn)
	{
		pBtn->SetMenu(IDR_MENU_LANGUAGE,TRUE);
		pBtn->SetToolTipText(_T("语言"));
		m_wndRibbonBar.AddToTabs(pBtn);
	}
	

	pBtn = new CBCGPRibbonButton (ID_APP_ABOUT,_T("关于"));

	if (NULL != pBtn)
	{
		pBtn->SetToolTipText(_T("关于"));
		m_wndRibbonBar.AddToTabs(pBtn);
	}	
}

void CMainFrame::CreateStatusBar( void )
{

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		//CLogImpl::CreateInstance().Error_Log("Create Status Bar failed.");
		return;
	}

	CTime CurrentTime = CTime::GetCurrentTime();
	CString strTime = CurrentTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	int nIndex = m_wndStatusBar.CommandToIndex(ID_DISPLAY_CURRENT_TIME);
	m_wndStatusBar.SetPaneText(nIndex,strTime);
	m_wndStatusBar.SetPaneWidth(nIndex,110);
	m_wndStatusBar.SetPaneTextColor(nIndex,RGB(255,0,0));
}

/*****************************************************************************
@FunctionName : 加载Ribbon需要的位图资源
@FunctionDescription : 加载Ribbon需要的位图资源
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadRibbonBitmap( void )
{
	//加载开始页的图标
	LoadStartPageRibbonBitmap();
	//加载页面页的图标
	LoadPageRibbonBitmap();
	//加载表格页面的图标
	LoadGridPageRibbonBitmap();
	//加载仪表页面的图标
	LoadDevicePageRibbonBitmap();
	//加载图标页面的图标
	LoadDiagramPageRibbonBitmap();
	//加载MainCategory图标
	LoadMainCategoryBitmap();
}

/*****************************************************************************
@FunctionName : 加载开始Ribbon界面图标
@FunctionDescription : 加载开始Ribbon界面图标
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadStartPageRibbonBitmap( void )
{
	//Ribbon开始界面的位图
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonStartLarge.bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_RibbonStartPageLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_RibbonStartPageLargeBitmap.AddImage(hLargeBitmap,FALSE);
		m_RibbonStartPageLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonStartSmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_RibbonStartPageSmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_RibbonStartPageSmallBitmap.AddImage(hSmallBitmap,FALSE);
		m_RibbonStartPageSmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}
}

/*****************************************************************************
@FunctionName : 加载页面Ribbon界面图标
@FunctionDescription : 加载页面Ribbon界面图标
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadPageRibbonBitmap( void )
{
	//Ribbon页面界面的位图
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonPageElementLarge .bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_RibbonPageLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_RibbonPageLargeBitmap.AddImage(hLargeBitmap,FALSE);
		m_RibbonPageLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonPageElementSmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_RibbonPageSmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_RibbonPageSmallBitmap.AddImage(hSmallBitmap,FALSE);
		m_RibbonPageSmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}
}

/*****************************************************************************
@FunctionName : 加载表格Ribbon界面图标
@FunctionDescription : 加载页面Ribbon界面图标
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadGridPageRibbonBitmap( void )
{
	//表格Ribbon页面界面的位图
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonGridPageLarge.bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_RibbonGridPageLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_RibbonGridPageLargeBitmap.AddImage(hLargeBitmap,FALSE);
		m_RibbonGridPageLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonGridPageSmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_RibbonGridPageSmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_RibbonGridPageSmallBitmap.AddImage(hSmallBitmap,FALSE);
		m_RibbonGridPageSmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}
}

/*****************************************************************************
@FunctionName : 加载仪表Ribbon界面图标
@FunctionDescription : 加载页面Ribbon界面图标
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadDevicePageRibbonBitmap( void )
{
	//仪表Ribbon页面界面的位图
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonDevicePageLarge.bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_RibbonDevicePageLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_RibbonDevicePageLargeBitmap.AddImage(hLargeBitmap,FALSE);
		m_RibbonDevicePageLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonGridPageSmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_RibbonDevicePageSmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_RibbonDevicePageSmallBitmap.AddImage(hSmallBitmap,FALSE);
		m_RibbonDevicePageSmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}
}

/*****************************************************************************
@FunctionName : 加载图表Ribbon界面图标
@FunctionDescription : 加载页面Ribbon界面图标
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CMainFrame::LoadDiagramPageRibbonBitmap( void )
{
	//仪表Ribbon页面界面的位图
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonDiagramPageLarge.bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_RibbonDiagramPageLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_RibbonDiagramPageLargeBitmap.AddImage(hLargeBitmap,FALSE);
		m_RibbonDiagramPageLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\RibbonDiagramPageSmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_RibbonDiagramPageSmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_RibbonDiagramPageSmallBitmap.AddImage(hSmallBitmap,FALSE);
		m_RibbonDiagramPageSmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}
}

void CMainFrame::LoadMainCategoryBitmap( void )
{
	//仪表Ribbon页面界面的位图
	std::string strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\MainCategoryLarge.bmp");
	HBITMAP hLargeBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hLargeBitmap)
	{
		m_MainCategoryLargeBitmap.SetImageSize(CSize(LARGE_ICON_WIDTH,LARGE_ICON_HEIGHT));
		m_MainCategoryLargeBitmap.AddImage(hLargeBitmap,FALSE);
		//m_MainCategoryLargeBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hLargeBitmap);
	}
	strBitmap = Utility::GetExeDirecory() + std::string("\\bmp\\MainCategorySmall.bmp");
	HBITMAP hSmallBitmap = Utility::LoadBitmapFromFile(strBitmap.c_str());
	if (NULL != hSmallBitmap)
	{
		m_MainCategorySmallBitmap.SetImageSize(CSize(SMALL_ICON_WIDTH,SMALL_ICON_HEIGHT));
		m_MainCategorySmallBitmap.AddImage(hSmallBitmap,FALSE);
		//m_MainCategorySmallBitmap.SetTransparentColor(TRANS_COLOR);
		DeleteObject(hSmallBitmap);
	}

}

void CMainFrame::OnMenuEnglish()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnUpdateMenuEnglish(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
}


void CMainFrame::OnMenuChinese()
{
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnUpdateMenuChinese(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (NULL != pCmdUI)
	{
		pCmdUI->SetCheck(TRUE);
	}
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CBCGPFrameWnd::OnCreateClient(lpcs,pContext);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFrameWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	if (nIDEvent == TIMER_ID)
	{

		CTime CurrentTime = CTime::GetCurrentTime();
		CString strTime = CurrentTime.Format(_T("%Y-%m-%d %H:%M:%S"));
		int nIndex = m_wndStatusBar.CommandToIndex(ID_DISPLAY_CURRENT_TIME);
		m_wndStatusBar.SetPaneText(nIndex,strTime,TRUE);

		//刷新Grid
		//GLOBAL_DATA.SetChanged();
		//GLOBAL_DATA.Notify();
	}

	CBCGPFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer(TIMER_ID);

	CSerialPortService::CreateInstance().StopSerialPortService();
	CBCGPFrameWnd::OnClose();
}

LRESULT CMainFrame::NotifyActiveWnd( WPARAM wp,LPARAM lp )
{
	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return 0L;
	}
	
	//获取激活类型
	int nActiveType = (int)wp;

	switch (nActiveType)
	{
		//表格
	case 0:
		{
			ShowRibbonCatagory(m_pGridCategory,true);
			ShowRibbonCatagory(m_pDeviceCategory);
			ShowRibbonCatagory(m_pDiagramCategory);

			CBCGPRibbonCategory* pActiveCategory = m_wndRibbonBar.GetActiveCategory();

			if (pActiveCategory == m_pDeviceCategory || pActiveCategory == m_pDiagramCategory)
			{
				m_wndRibbonBar.SetActiveCategory(m_pGridCategory);
			}
		}
		break;
		//仪表
	case 1:
		{
			ShowRibbonCatagory(m_pGridCategory);
			ShowRibbonCatagory(m_pDeviceCategory,true);
			ShowRibbonCatagory(m_pDiagramCategory);

			CBCGPRibbonCategory* pActiveCategory = m_wndRibbonBar.GetActiveCategory();

			if (pActiveCategory == m_pGridCategory || pActiveCategory == m_pDiagramCategory)
			{
				m_wndRibbonBar.SetActiveCategory(m_pDeviceCategory);
			}
		}
		break;
		//图表
	case 2:
		{
			ShowRibbonCatagory(m_pGridCategory);
			ShowRibbonCatagory(m_pDeviceCategory);
			ShowRibbonCatagory(m_pDiagramCategory,true);	

			CBCGPRibbonCategory* pActiveCategory = m_wndRibbonBar.GetActiveCategory();

			if (pActiveCategory == m_pGridCategory || pActiveCategory == m_pDeviceCategory)
			{
				m_wndRibbonBar.SetActiveCategory(m_pDiagramCategory);
			}
		}
		break;
	default:
		break;
	}

	m_wndRibbonBar.RecalcLayout();
	return 0L;
}

void CMainFrame::ShowRibbonCatagory( CBCGPRibbonCategory* pCatagory,bool bShow /*= false*/ )
{

	if (NULL == m_wndRibbonBar.GetSafeHwnd())
	{
		return;
	}
	int nCategoryNum = m_wndRibbonBar.GetCategoryCount();

	if (NULL != pCatagory)
	{
		int nIndex = m_wndRibbonBar.GetCategoryIndex(pCatagory);

		if (nIndex >= 0 && nIndex < nCategoryNum)
		{
			if (bShow)
			{
				m_wndRibbonBar.ShowCategory(nIndex,TRUE);
			}
			else
			{
				m_wndRibbonBar.ShowCategory(nIndex,FALSE);
			}		
		}
	}
}


