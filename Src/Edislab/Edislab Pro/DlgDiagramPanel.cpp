// DlgDiagramPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgDiagramPanel.h"
#include "ChartFigureDlg.h"

// CDlgDiagramPanel 对话框

IMPLEMENT_DYNAMIC(CDlgDiagramPanel, CBaseDialog)

CDlgDiagramPanel::CDlgDiagramPanel(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgDiagramPanel::IDD, pParent)
	, m_WidgetLayout(this, true)
{
	
}

CDlgDiagramPanel::~CDlgDiagramPanel()
{
	for (UINT i = 0; i < m_vecPanel.size(); ++i)
	{
		if (NULL != m_vecPanel[i])
		{
			delete m_vecPanel[i];
			m_vecPanel[i] = NULL;
		}
	}
	m_vecPanel.clear();
}

void CDlgDiagramPanel::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDiagramPanel, CBaseDialog)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgDiagramPanel 消息处理程序
BOOL CDlgDiagramPanel::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//CreatePanel();
	m_WidgetLayout.InitLayout(this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgDiagramPanel::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustPanelLayout(cx,cy);
}

void CDlgDiagramPanel::CreatePanel( void )
{
	ChartFigureDlg* pDiagramPanel = new ChartFigureDlg;
	if (NULL != pDiagramPanel)
	{
		pDiagramPanel->Create(ChartFigureDlg::IDD,this);
		m_vecPanel.push_back(pDiagramPanel);
		m_WidgetLayout.AddWidget(pDiagramPanel);
		m_dataManager.addChartDlg(pDiagramPanel);
		CRect rc;
		GetClientRect(&rc);
		m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
	}
}
void CDlgDiagramPanel::addPanel()
{
#if 0
	if(m_vecPanel.size()>=MAX_WIDGET_NUM)
	{
		return;
	}
#endif
	ChartFigureDlg* pDiagramPanel = new ChartFigureDlg;
	if (NULL != pDiagramPanel)
	{
		pDiagramPanel->Create(ChartFigureDlg::IDD,this);
		m_vecPanel.push_back(pDiagramPanel);
		m_WidgetLayout.AddWidget(pDiagramPanel);
		m_dataManager.addChartDlg(pDiagramPanel);
		CRect rc;
		GetClientRect(&rc);
		m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
	}
}
void CDlgDiagramPanel::delPanel(CWnd* pDlg)
{
	if(pDlg)
	{
		m_WidgetLayout.DelWidget(pDlg);
		CRect rc;
		GetClientRect(&rc);
		m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
		for(std::vector<CBaseDialog*>::iterator itr = m_vecPanel.begin();
			itr!=m_vecPanel.end(); ++itr)
		{
			if(pDlg==*itr)
			{
				if (pDlg->GetSafeHwnd() != NULL)
				{
					pDlg->DestroyWindow();
				}
				m_vecPanel.erase(itr);
				break;
			}
		}
		m_WidgetLayout.DelWidget(pDlg);
		m_dataManager.delChartDlg(dynamic_cast<ChartFigureDlg*>(pDlg));
		delete pDlg;
		pDlg = NULL;
	}
}
void CDlgDiagramPanel::DestroyPanel( void )
{
	for (UINT i = 0; i < m_vecPanel.size(); ++i)
	{
		if (NULL != m_vecPanel[i])
		{
			if (m_vecPanel[i]->GetSafeHwnd() != NULL)
			{
				m_vecPanel[i]->DestroyWindow();
			}
		}
	}
}

void CDlgDiagramPanel::AdjustPanelLayout( int nWidth,int nHeight )
{
	m_WidgetLayout.AdjustLayout(nWidth,nHeight);
}

void CDlgDiagramPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonDown(nFlags,point,rc.Width(),rc.Height());

	CBCGPDialog::OnLButtonDown(nFlags, point);
}


void CDlgDiagramPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleMouseMove(nFlags,point,rc.Width(),rc.Height());
	CBCGPDialog::OnMouseMove(nFlags, point);
}


void CDlgDiagramPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonUp(nFlags,point,rc.Width(),rc.Height());
	ReleaseCapture();
	CBCGPDialog::OnLButtonUp(nFlags, point);
}

int CDlgDiagramPanel::GetWidgetNum( void ) const
{
	return m_WidgetLayout.GetWidgetNum();
}
