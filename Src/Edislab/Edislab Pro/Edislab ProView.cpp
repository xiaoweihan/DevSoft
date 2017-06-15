
// Edislab ProView.cpp : CEdislabProView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Edislab Pro.h"
#endif
#include "Edislab ProDoc.h"
#include "Edislab ProView.h"
//#include "LogImpl.h"
#include "Msg.h"
#include "CommandEntry.h"
#include "DlgTabPanel.h"
#include "MainFrm.h"
#include "Macro.h"
#include "DlgDevicePanel.h"
#include "DlgDiagramPanel.h"
#include "DlgGridPanel.h"
#include "Type.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEdislabProView
const int DEFAULT_GAP = 5;
IMPLEMENT_DYNCREATE(CEdislabProView, CView)

BEGIN_MESSAGE_MAP(CEdislabProView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEdislabProView::OnFilePrintPreview)
	ON_COMMAND_RANGE(ID_MIN_COMMAND,ID_MAX_COMMAND,&CEdislabProView::HandleAllRibbonCommand)
	ON_UPDATE_COMMAND_UI_RANGE(ID_MIN_COMMAND,ID_MAX_COMMAND,&CEdislabProView::HandleUpdateAllRibbonCommand)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_OPEN, &CEdislabProView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CEdislabProView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CEdislabProView::OnFileSaveAs)
END_MESSAGE_MAP()

// CEdislabProView 构造/析构

CEdislabProView::CEdislabProView()
{

}

CEdislabProView::~CEdislabProView()
{

}

BOOL CEdislabProView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CEdislabProView 绘制

void CEdislabProView::OnDraw(CDC* /*pDC*/)
{
	CEdislabProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CEdislabProView 打印


void CEdislabProView::OnFilePrintPreview()
{
#if 0
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
#endif
}

BOOL CEdislabProView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CEdislabProView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CEdislabProView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CEdislabProView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEdislabProView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CEdislabProView 诊断

#ifdef _DEBUG
void CEdislabProView::AssertValid() const
{
	CView::AssertValid();
}

void CEdislabProView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEdislabProDoc* CEdislabProView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEdislabProDoc)));
	return (CEdislabProDoc*)m_pDocument;
}
#endif //_DEBUG


// CEdislabProView 消息处理程序


int CEdislabProView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CreateDisplayTabWnd();
	return 0;
}


void CEdislabProView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);


	// TODO: 在此处添加消息处理程序代码
	if (NULL != m_TabWnd.GetSafeHwnd())
	{
		m_TabWnd.MoveWindow(0,0,cx,cy);
	}


}

/*****************************************************************************
@FunctionName : CreateDisplayTabWnd
@FunctionDescription : 创建TabWnd
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CEdislabProView::CreateDisplayTabWnd( void )
{
	if (NULL == m_TabWnd.GetSafeHwnd())
	{
		if (FALSE == m_TabWnd.Create(CBCGPTabWnd::STYLE_3D,CRect(0,0,0,0),this,CBaseTabWnd::s_nTabBaseID++
			,CBCGPBaseTabWnd::LOCATION_BOTTOM))
		{
			//CLogImpl::CreateInstance().Error_Log("Create TabWnd failed");
			return;
		}
	}
}


void CEdislabProView::OnDestroy()
{
	CView::OnDestroy();
}


void CEdislabProView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	AddNewTabWnd(DEFAULT_PAGE_NAME,1,1,1);


}

/*****************************************************************************
@FunctionName : HandleAllRibbonCommand
@FunctionDescription : 处理所有Ribbon消息响应
@inparam  : nCommand:Ribbon的ID
@outparam :  
@return : 
*****************************************************************************/
void CEdislabProView::HandleAllRibbonCommand( UINT nCommand )
{

	pCommandEntry pEntry = CCommandEntry::CreateInstance().GetCommandEntryByCommanID(nCommand);

	if (NULL != pEntry)
	{
		pEntry(this);
	}

}

/*****************************************************************************
@FunctionName : HandleUpdateAllRibbonCommand
@FunctionDescription : 处理所有Ribbon的更新响应
@inparam  : 
@outparam :  
@return : 
*****************************************************************************/
void CEdislabProView::HandleUpdateAllRibbonCommand( CCmdUI *pCmdUI )
{
	pUpdateCommandEntry pEntry = CCommandEntry::CreateInstance().GetUpdateCommandEntryByCommanID(pCmdUI->m_nID);

	if (NULL != pEntry)
	{
		pEntry(this,pCmdUI);
	}
}

/*****************************************************************************
@FunctionName : AddNewTabWnd
@FunctionDescription : 添加新的Tab页
@inparam  : strTabWndName:Tab页名称 nGridNum:图表个数 nDiagramNum:个数 nDeviceNum:仪表个数
@outparam :  
@return : true:成功 false:失败
*****************************************************************************/
bool CEdislabProView::AddNewTabWnd( const CString& strTabWndName,int nGridNum,int nDiagramNum,int nDeviceNum )
{
	if (strTabWndName.IsEmpty() || NULL == m_TabWnd.GetSafeHwnd())
	{
		return false;
	}

	if (nGridNum < 0 || nDeviceNum < 0 || nDiagramNum < 0)
	{
		return false;
	}

	CDlgTabPanel* pWnd = new CDlgTabPanel(strTabWndName);

	if (NULL != pWnd)
	{
		pWnd->SetDeviceNum(nDeviceNum);
		pWnd->SetDiagramNum(nDiagramNum);
		pWnd->SetGridNum(nGridNum);

		pWnd->Create(CDlgTabPanel::IDD,&m_TabWnd);

		pWnd->PrepareDisplayElement();

		m_TabWnd.AddTab(pWnd,strTabWndName);

		return true;

	}

	return false;
}

/*****************************************************************************
@FunctionName : GetCurrentPage
@FunctionDescription : 获取当前活动的Tab页
@inparam  : 
@outparam :  
@return : 当前活动页指针
*****************************************************************************/
CDlgTabPanel* CEdislabProView::GetCurrentPage( void )
{
	if (NULL == m_TabWnd.GetSafeHwnd())
	{
		return NULL;
	}

	CDlgTabPanel* pActiveWnd = NULL;

	pActiveWnd = reinterpret_cast<CDlgTabPanel*>(m_TabWnd.GetActiveWnd());

	return pActiveWnd;
}

/*****************************************************************************
@FunctionName : GetCurrentPageName
@FunctionDescription : 获取当前页的名称
@inparam  : 
@outparam :  
@return : 当前页的名称
*****************************************************************************/
CString CEdislabProView::GetCurrentPageName( void )
{

	CString strPageName;

	CDlgTabPanel* pWnd = GetCurrentPage();

	if (NULL == pWnd)
	{
		return strPageName;
	}

	strPageName = pWnd->GetPageName();

	return strPageName;
}

void CEdislabProView::SetCurrentPageName( const CString& strPageName )
{
	
	CDlgTabPanel* pWnd = GetCurrentPage();

	if (NULL != pWnd)
	{
		pWnd->SetPageName(strPageName);
	}

	if (NULL != m_TabWnd.GetSafeHwnd())
	{
		int nActiveTab = m_TabWnd.GetActiveTab();

		if (nActiveTab >= 0)
		{
			m_TabWnd.SetTabLabel(nActiveTab,strPageName);
		}
	}
}

/*****************************************************************************
@FunctionName : GetPageNum
@FunctionDescription : 获取页的总数
@inparam  : 
@outparam :  
@return : 页的总数
*****************************************************************************/
int CEdislabProView::GetPageNum( void )
{
	if (NULL == m_TabWnd.GetSafeHwnd())
	{
		return 0;
	}

	return m_TabWnd.GetTabsNum();
}

void CEdislabProView::SetActivePage( int nPageIndex )
{
	if (m_TabWnd.GetSafeHwnd() == NULL)
	{
		return;
	}

	if (nPageIndex < 0 || nPageIndex >= m_TabWnd.GetTabsNum())
	{
		return;
	}

	m_TabWnd.SetActiveTab(nPageIndex);
}

void CEdislabProView::DeleteCurrentPage( void )
{
	if (NULL != m_TabWnd.GetSafeHwnd())
	{
		int nActiveTab = m_TabWnd.GetActiveTab();
		CDlgTabPanel* pActiveWnd = dynamic_cast<CDlgTabPanel*>(m_TabWnd.GetActiveWnd());

		if (nullptr != pActiveWnd)
		{
			pActiveWnd->DestroyWindow();
		}

		if (nActiveTab >= 0)
		{
			m_TabWnd.RemoveTab(nActiveTab,TRUE);
			if (nActiveTab >= m_TabWnd.GetTabsNum() && m_TabWnd.GetTabsNum() > 0)
			{
				m_TabWnd.SetActiveTab(m_TabWnd.GetTabsNum() - 1);
			}	
		}	
	}
}

/*****************************************************************************
@FunctionName : GetCurrentView
@FunctionDescription : 获取视图指针
@inparam  : 
@outparam :  
@return : 视图指针
*****************************************************************************/
CEdislabProView* CEdislabProView::GetCurrentView( void )
{

	CMainFrame* pWnd = (CMainFrame*)AfxGetMainWnd();

	if (NULL == pWnd)
	{
		return NULL;
	}

	CEdislabProView* pView = (CEdislabProView*)pWnd->GetActiveView();

	return pView;

}

int CEdislabProView::GetGridNum( void )
{
	CDlgTabPanel* pWnd = GetCurrentPage();

	if (NULL != pWnd)
	{
		return pWnd->GetGridNum();
	}
	return 0;
}

int CEdislabProView::GetDeviceNum( void )
{
	CDlgTabPanel* pWnd = GetCurrentPage();

	if (NULL != pWnd)
	{
		return pWnd->GetDeviceNum();
	}
	return 0;
}

int CEdislabProView::GetDiagramNum( void )
{
	CDlgTabPanel* pWnd = GetCurrentPage();

	if (NULL != pWnd)
	{
		return pWnd->GetDiagramNum();
	}
	return 0;
}

void CEdislabProView::AddGrid( void )
{
	CDlgTabPanel* pWnd = GetCurrentPage();

	if (NULL != pWnd)
	{
		pWnd->AddGrid();
	}
}

void CEdislabProView::AddDiagram( void )
{
	CDlgTabPanel* pWnd = GetCurrentPage();

	if (NULL != pWnd)
	{
		pWnd->AddChart();
	}
}

void CEdislabProView::AddDevice( void )
{
	CDlgTabPanel* pWnd = GetCurrentPage();

	if (NULL != pWnd)
	{
		pWnd->AddGauge();
	}
}


void CEdislabProView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog OpenDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Editlab实验文件(*.edisp;*.edis)|*.edisp; *.edis||"),this);

	if (IDOK == OpenDlg.DoModal())
	{

	}
}


void CEdislabProView::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog SaveDlg(FALSE,_T("edisp"),_T("无标题实验.edisp"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("Editlab实验文件(*.edisp;*.edis)|*.edisp; *.edis||"),this);

	if (IDOK == SaveDlg.DoModal())
	{
	}
}


void CEdislabProView::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码

	OnFileSave();
}

void CEdislabProView::NotifyDetectDevice( const std::string& strDeviceName,int nOnFlag )
{
	if (NULL == m_TabWnd.GetSafeHwnd())
	{
		return;
	}
	int nTabWndNum = m_TabWnd.GetTabsNum();
	CDlgTabPanel* pPanel = nullptr;
	for (int i = 0; i < nTabWndNum; ++i)
	{
		pPanel = dynamic_cast<CDlgTabPanel*>(m_TabWnd.GetTabWnd(i));

		if (nullptr != pPanel)
		{
			pPanel->NotifyDectectSensor(strDeviceName,nOnFlag);
		}
	}
}

void CEdislabProView::NotifyGridChangeRows(int nRows)
{
	if (NULL == m_TabWnd.GetSafeHwnd())
	{
		return;
	}
	int nTabWndNum = m_TabWnd.GetTabsNum();
	CDlgTabPanel* pPanel = nullptr;
	for (int i = 0; i < nTabWndNum; ++i)
	{
		pPanel = dynamic_cast<CDlgTabPanel*>(m_TabWnd.GetTabWnd(i));

		if (nullptr != pPanel)
		{
			pPanel->NotifyGridChangeRows(nRows);
		}
	}
}

void CEdislabProView::NotifyControlsStartRefresh()
{
	if (NULL == m_TabWnd.GetSafeHwnd())
	{
		return;
	}
	int nTabWndNum = m_TabWnd.GetTabsNum();
	CDlgTabPanel* pPanel = nullptr;
	for (int i = 0; i < nTabWndNum; ++i)
	{
		pPanel = dynamic_cast<CDlgTabPanel*>(m_TabWnd.GetTabWnd(i));

		if (nullptr != pPanel)
		{
			pPanel->NotifyControlsStartRefresh();
		}
	}
}

void CEdislabProView::NotifyControlsStopRefresh()
{
	if (NULL == m_TabWnd.GetSafeHwnd())
	{
		return;
	}
	int nTabWndNum = m_TabWnd.GetTabsNum();
	CDlgTabPanel* pPanel = nullptr;
	for (int i = 0; i < nTabWndNum; ++i)
	{
		pPanel = dynamic_cast<CDlgTabPanel*>(m_TabWnd.GetTabWnd(i));

		if (nullptr != pPanel)
		{
			pPanel->NotifyControlsStopRefresh();
		}
	}
}

void CEdislabProView::DeleteElement()
{
	CDlgTabPanel* pTabPanel = GetCurrentPage();
	if(nullptr != pTabPanel)
	{
		pTabPanel->DelWnd();
	}
}
