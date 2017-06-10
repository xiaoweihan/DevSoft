// DlgTabPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgTabPanel.h"
#include "DlgDevicePanel.h"
#include "DlgDiagramPanel.h"
#include "DlgGridPanel.h"
#include "Type.h"
#include "ChartFigureDlg.h"
#include "GaugeDlg.h"
#include "CustomGrid.h"
#include "Msg.h"
// CDlgTabPanel 对话框

IMPLEMENT_DYNAMIC(CDlgTabPanel, CBCGPDialog)

CDlgTabPanel::CDlgTabPanel( const CString&strPageName,CWnd* pParent /*= NULL*/ )
	: CBCGPDialog(CDlgTabPanel::IDD, pParent),
	m_WidgetLayout(this,true),
	m_strPageName(strPageName),
	m_nDiagramNum(1),
	m_nGridNum(1),
	m_nDeviceNum(1),
	m_pActiveDlg(nullptr)
{
	// TODO: 在此处添加构造代码
	for (int i = 0; i < PANEL_NUM; ++i)
	{
		m_pPanel[i] = nullptr;
	}
}


CDlgTabPanel::~CDlgTabPanel()
{
	for (int i = 0; i < PANEL_NUM; ++i)
	{
		if (nullptr != m_pPanel[i])
		{
			delete m_pPanel[i];
			m_pPanel[i] = nullptr;
		}
	}
}

void CDlgTabPanel::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTabPanel, CBCGPDialog)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgTabPanel 消息处理程序


BOOL CDlgTabPanel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam || VK_ESCAPE == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return CBCGPDialog::PreTranslateMessage(pMsg);
}


void CDlgTabPanel::SetPageName( const CString& strPageName )
{
	m_strPageName = strPageName;
}

CString CDlgTabPanel::GetPageName( void ) const
{
	return m_strPageName;
}

int CDlgTabPanel::GetGridNum( void ) const
{
	
	CDlgGridPanel* panle = dynamic_cast<CDlgGridPanel*>(m_pPanel[GRID_INDEX]);
	if(nullptr != panle)
	{
		return panle->GetWidgetNum();
	}

	return 0;
}

void CDlgTabPanel::SetGridNum( int nGridNum )
{
	m_nGridNum = nGridNum;
	
}

int CDlgTabPanel::GetDeviceNum( void ) const
{
	//return m_nDeviceNum;
	CDlgDevicePanel* panle = dynamic_cast<CDlgDevicePanel*>(m_pPanel[DEVICE_INDEX]);
	if(nullptr != panle)
	{
		return panle->GetWidgetNum();
	}

	return 0;
}

void CDlgTabPanel::SetDeviceNum( int nDeviceNum )
{
	m_nDeviceNum = nDeviceNum;
}

int CDlgTabPanel::GetDiagramNum( void ) const
{
	CDlgDiagramPanel* panle = dynamic_cast<CDlgDiagramPanel*>(m_pPanel[DIAGRAM_INDEX]);
	if(nullptr != panle)
	{
		return panle->GetWidgetNum();
	}

	return 0;
}

void CDlgTabPanel::SetDiagramNum( int DiagramNum )
{
	m_nDiagramNum = DiagramNum;
}

BOOL CDlgTabPanel::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CreatePanel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgTabPanel::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustPanelLayout(cx,cy);
}

//添加表格
void CDlgTabPanel::AddGrid()
{
	CDlgGridPanel* panle = dynamic_cast<CDlgGridPanel*>(m_pPanel[GRID_INDEX]);
	if(nullptr != panle)
	{
		panle->addPanel();
	}
}
//添加图表
void CDlgTabPanel::AddChart()
{
	CDlgDiagramPanel* panle = dynamic_cast<CDlgDiagramPanel*>(m_pPanel[DIAGRAM_INDEX]);
	if(nullptr != panle)
	{
		panle->addPanel();
	}
}
//添回仪表
void CDlgTabPanel::AddGauge()
{
	CDlgDevicePanel* panle = dynamic_cast<CDlgDevicePanel*>(m_pPanel[DEVICE_INDEX]);
	if(nullptr != panle)
	{
		panle->addPanel();
	}
}

void CDlgTabPanel::NotifyDectectSensor( const std::string& strDeviceName,int nOnFlag )
{
	CDlgGridPanel* panle = dynamic_cast<CDlgGridPanel*>(m_pPanel[GRID_INDEX]);
	if(nullptr != panle)
	{
		panle->NotifyDetectSensor(strDeviceName,nOnFlag);
	}
}

void CDlgTabPanel::NotifyGridChangeRows(int nRows)
{
	CDlgGridPanel* panle = dynamic_cast<CDlgGridPanel*>(m_pPanel[GRID_INDEX]);
	if(nullptr != panle)
	{
		panle->NotifyGridChangeRows(nRows);
	}
}

void CDlgTabPanel::NotifyControlsStartRefresh()
{
	//通知Grid开始刷新
	CDlgGridPanel* panle = dynamic_cast<CDlgGridPanel*>(m_pPanel[GRID_INDEX]);
	if(nullptr != panle)
	{
		panle->NotifyControlsStartRefresh();
	}
}

void CDlgTabPanel::NotifyControlsStopRefresh()
{
	CDlgGridPanel* panle = dynamic_cast<CDlgGridPanel*>(m_pPanel[GRID_INDEX]);
	if(nullptr != panle)
	{
		panle->NotifyControlsStopRefresh();
	}
}

void CDlgTabPanel::DelWnd( void )
{
	switch (m_nActiveWndType)
	{
	case DEVICE_INDEX:
		{
			CDlgDevicePanel* pDevicePanel = dynamic_cast<CDlgDevicePanel*>(m_pPanel[DEVICE_INDEX]);

			if (nullptr != pDevicePanel)
			{
				pDevicePanel->delPanel(m_pActiveDlg);
			}
		}
		break;
	case GRID_INDEX:
		{
			CDlgGridPanel* pGridPanel = dynamic_cast<CDlgGridPanel*>(m_pPanel[GRID_INDEX]);

			if (nullptr != pGridPanel)
			{
				pGridPanel->delPanel(m_pActiveDlg);
			}
		}
		break;
	case DIAGRAM_INDEX:
		{
			CDlgDiagramPanel* pDiagramPanel = dynamic_cast<CDlgDiagramPanel*>(m_pPanel[DIAGRAM_INDEX]);

			if (nullptr != pDiagramPanel)
			{
				pDiagramPanel->delPanel(m_pActiveDlg);
			}
		}
		break;
	default:
		break;
	}

	m_pActiveDlg = nullptr;
}

void CDlgTabPanel::SetActive( int nType,CWnd* pActiveWnd )
{
	CWnd* pOldActiveWnd = m_pActiveDlg;

	//如果当前为同一个窗口
	if (pOldActiveWnd == pActiveWnd)
	{
		return;
	}
	m_nActiveWndType = nType;
	m_pActiveDlg = pActiveWnd;
	//刷新当前活动窗口
	if (m_pActiveDlg)
	{
		m_pActiveDlg->SendMessage(WM_SET_DLG_ACTIVE,1,0);
		m_pActiveDlg->Invalidate(TRUE);
	}
	//刷新之前的活动窗口
	if (nullptr != pOldActiveWnd)
	{
		pOldActiveWnd->SendMessage(WM_SET_DLG_ACTIVE,0,0);
		pOldActiveWnd->Invalidate(TRUE);
	}
	
}

void CDlgTabPanel::CreatePanel( void )
{
	m_WidgetLayout.InitLayout(this);

	CDlgGridPanel* pGridPanel = new CDlgGridPanel;

	if (nullptr != pGridPanel)
	{
		pGridPanel->Create(CDlgGridPanel::IDD,this);
		m_pPanel[GRID_INDEX] = pGridPanel;

		if (m_nGridNum > 0)
		{
			m_WidgetLayout.AddWidget(pGridPanel);
		}	
	}
	CDlgDiagramPanel* pDiagramPanel = new CDlgDiagramPanel;
	if (NULL != pDiagramPanel)
	{
		pDiagramPanel->Create(CDlgDiagramPanel::IDD,this);
		m_pPanel[DIAGRAM_INDEX] = pDiagramPanel;

		if (m_nDiagramNum > 0)
		{
			m_WidgetLayout.AddWidget(pDiagramPanel);
		}		
	}

	CDlgDevicePanel* pDevicePanel = new CDlgDevicePanel;
	if (NULL != pDevicePanel)
	{
		pDevicePanel->Create(CDlgDevicePanel::IDD,this);
		m_pPanel[DEVICE_INDEX] = pDevicePanel;

		if (m_nDeviceNum > 0)
		{
			m_WidgetLayout.AddWidget(pDevicePanel);
		}

	}
}

void CDlgTabPanel::DestroyPanel( void )
{
	for (int i = 0; i < PANEL_NUM; ++i)
	{
		if (nullptr != m_pPanel[i])
		{
			if (m_pPanel[i]->GetSafeHwnd() != NULL)
			{
				m_pPanel[i]->DestroyWindow();
			}
		}
	}
}

void CDlgTabPanel::AdjustPanelLayout( int nWidth,int nHeight )
{
	m_WidgetLayout.AdjustLayout(nWidth,nHeight);
}

void CDlgTabPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonDown(nFlags,point,rc.Width(),rc.Height());
	CBCGPDialog::OnLButtonDown(nFlags, point);
}


void CDlgTabPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleMouseMove(nFlags,point,rc.Width(),rc.Height());
	CBCGPDialog::OnMouseMove(nFlags, point);
}


void CDlgTabPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonUp(nFlags,point,rc.Width(),rc.Height());
	ReleaseCapture();
	CBCGPDialog::OnLButtonUp(nFlags, point);
}

void CDlgTabPanel::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;
	CBCGPDialog::PostNcDestroy();
}

CWnd* CDlgTabPanel::GetActiveDlg()
{
	return m_pActiveDlg;
}

void CDlgTabPanel::PrepareDisplayElement( void )
{
	for (int i = 0; i < m_nDiagramNum; ++i)
	{
		AddChart();
	}

	for (int i = 0; i < m_nGridNum; ++i)
	{
		AddGrid();
	}

	for (int i = 0; i < m_nDeviceNum; ++i)
	{
		AddGauge();
	}
}


void CDlgTabPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBCGPDialog::OnPaint()
}
