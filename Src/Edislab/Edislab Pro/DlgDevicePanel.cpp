// DlgDevicePanel.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgDevicePanel.h"
#include "GaugeDlg.h"
#include "Global.h"
// CDlgDevicePanel 对话框

IMPLEMENT_DYNAMIC(CDlgDevicePanel, CBaseDialog)

CDlgDevicePanel::CDlgDevicePanel(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgDevicePanel::IDD, pParent)
	, m_WidgetLayout(this, true)
{
}

CDlgDevicePanel::~CDlgDevicePanel()
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

void CDlgDevicePanel::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDevicePanel, CBaseDialog)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgDevicePanel 消息处理程序
BOOL CDlgDevicePanel::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_WidgetLayout.InitLayout(this);
	//CreatePanel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgDevicePanel::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustPanelLayout(cx,cy);
}

void CDlgDevicePanel::CreatePanel( void )
{
	GaugeDlg* pDevicePanel = new GaugeDlg;
	if (NULL != pDevicePanel)
	{
		pDevicePanel->Create(GaugeDlg::IDD,this);
		m_vecPanel.push_back(pDevicePanel);
		m_WidgetLayout.AddWidget(pDevicePanel);
		m_dataManager.addGaugeDlg(pDevicePanel);
	}
}
void CDlgDevicePanel::addPanel()
{
#if 0
	if(m_vecPanel.size()>=MAX_WIDGET_NUM)
	{
		return;
	}

	GaugeDlg* pDevicePanel = new GaugeDlg;
	if (NULL != pDevicePanel)
	{
		pDevicePanel->Create(GaugeDlg::IDD,this);
		m_vecPanel.push_back(pDevicePanel);
		m_WidgetLayout.AddWidget(pDevicePanel);
		m_dataManager.addGaugeDlg(pDevicePanel);
	}
#endif

	GaugeDlg* pDevicePanel = new GaugeDlg;
	if (NULL != pDevicePanel)
	{
		pDevicePanel->Create(GaugeDlg::IDD,this);
		m_vecPanel.push_back(pDevicePanel);
		m_WidgetLayout.AddWidget(pDevicePanel);
		m_dataManager.addGaugeDlg(pDevicePanel);
		CRect rc;
		GetClientRect(&rc);
		m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
	}

}
void CDlgDevicePanel::delPanel(CWnd* pDlg)
{
#if 1
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
		m_dataManager.delGaugeDlg(dynamic_cast<GaugeDlg*>(pDlg));
		delete pDlg;
		pDlg = NULL;
	}
#endif
	
}
void CDlgDevicePanel::DestroyPanel( void )
{
#if 0
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
#endif
}

void CDlgDevicePanel::AdjustPanelLayout( int nWidth,int nHeight )
{
	m_WidgetLayout.AdjustLayout(nWidth,nHeight);
}

void CDlgDevicePanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonDown(nFlags,point,rc.Width(),rc.Height());

	CBCGPDialog::OnLButtonDown(nFlags, point);
}


void CDlgDevicePanel::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleMouseMove(nFlags,point,rc.Width(),rc.Height());
	CBCGPDialog::OnMouseMove(nFlags, point);
}


void CDlgDevicePanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonUp(nFlags,point,rc.Width(),rc.Height());
	ReleaseCapture();
	CBCGPDialog::OnLButtonUp(nFlags, point);
}

int CDlgDevicePanel::GetWidgetNum( void ) const
{
	return m_WidgetLayout.GetWidgetNum();
}
