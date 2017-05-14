// DlgGridPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgGridPanel.h"
//#include "GridControlFactory.h"
#include "DlgGridContainer.h"
#include "Log.h"
// CDlgGridPanel 对话框

IMPLEMENT_DYNAMIC(CDlgGridPanel, CBaseDialog)

CDlgGridPanel::CDlgGridPanel(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgGridPanel::IDD, pParent)
	, m_WidgetLayout(this, true)
{

}

CDlgGridPanel::~CDlgGridPanel()
{
	for (int i = 0; i < (int)m_vecPanel.size(); ++i)
	{
		if (NULL != m_vecPanel[i])
		{
			delete m_vecPanel[i];
			m_vecPanel[i] = NULL;
		}
	}
	m_vecPanel.clear();
}

void CDlgGridPanel::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgGridPanel, CBaseDialog)
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgGridPanel 消息处理程序


void CDlgGridPanel::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBaseDialog::OnActivate(nState, pWndOther, bMinimized);
}

BOOL CDlgGridPanel::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_WidgetLayout.InitLayout(this);
	//CreatePanel();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgGridPanel::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	AdjustPanelLayout(cx,cy);
}

#if 0
void CDlgGridPanel::CreatePanel( void )
{
	CWnd* pWnd = m_Grid.AddGridCtrl(this);
	m_vecPanel.push_back(pWnd);
	m_WidgetLayout.AddWidget(pWnd);
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
}
#endif

//添加新的想
void CDlgGridPanel::addPanel()
{
#if 0
	CWnd* pWnd = m_Grid.AddGridCtrl(this);
	m_vecPanel.push_back(pWnd);
#endif
	CDlgGridContainer* pWnd = new CDlgGridContainer(this);
	if (nullptr == pWnd)
	{
		return;
	}
	pWnd->Create(CDlgGridContainer::IDD,this);
	pWnd->ShowWindow(SW_SHOW);
	m_WidgetLayout.AddWidget(pWnd);
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
}
void CDlgGridPanel::delPanel(CWnd* pDlg)
{
	if(nullptr != pDlg)
	{
		m_WidgetLayout.DelWidget(pDlg);
		CRect rc;
		GetClientRect(&rc);
		m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
		for(std::vector<CWnd*>::iterator itr = m_vecPanel.begin();
			itr!=m_vecPanel.end(); ++itr)
		{
			if(pDlg == *itr)
			{
				if (pDlg->GetSafeHwnd() != NULL)
				{
					pDlg->DestroyWindow();
				}
				m_vecPanel.erase(itr);
				break;
			}
		}
		
		//m_Grid.RemoveGridCtrl(pDlg);
		delete pDlg;
		pDlg = nullptr;
	}
}

#if 0
void CDlgGridPanel::DestroyPanel( void )
{
#if 1
	for (int i = 0; i < (int)m_vecPanel.size(); ++i)
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
#endif

void CDlgGridPanel::AdjustPanelLayout( int nWidth,int nHeight )
{
	m_WidgetLayout.AdjustLayout(nWidth,nHeight);
}

void CDlgGridPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonDown(nFlags,point,rc.Width(),rc.Height());

	CBCGPDialog::OnLButtonDown(nFlags, point);
}


void CDlgGridPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleMouseMove(nFlags,point,rc.Width(),rc.Height());
	CBCGPDialog::OnMouseMove(nFlags, point);
}


void CDlgGridPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.HandleLButtonUp(nFlags,point,rc.Width(),rc.Height());
	ReleaseCapture();
	CBCGPDialog::OnLButtonUp(nFlags, point);
}

int CDlgGridPanel::GetWidgetNum( void ) const
{
	return m_WidgetLayout.GetWidgetNum();
}
