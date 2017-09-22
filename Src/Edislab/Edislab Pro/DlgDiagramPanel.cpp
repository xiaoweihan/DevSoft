// DlgDiagramPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgDiagramPanel.h"
#include <algorithm>
#include <boost/checked_delete.hpp>
#include <boost/foreach.hpp>
#include "ChartFigureDlg.h"
#include "Macro.h"
#include "SensorManager.h"
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
		boost::checked_delete(m_vecPanel[i]);
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

void CDlgDiagramPanel::addPanel()
{
	ChartFigureDlg* pDiagramPanel = new ChartFigureDlg;
	if (nullptr != pDiagramPanel)
	{
		pDiagramPanel->Create(ChartFigureDlg::IDD,this);
		m_vecPanel.push_back(pDiagramPanel);
		m_WidgetLayout.AddWidget(pDiagramPanel);
		//m_dataManager.addChartDlg(pDiagramPanel);
		CRect rc;
		GetClientRect(&rc);
		m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
	}
}
void CDlgDiagramPanel::delPanel(CWnd* pDlg)
{
	if(nullptr != pDlg)
	{
		auto FindPred = [pDlg](const ChartFigureDlg* pElement)->bool
		{
			if (pDlg == pElement)
			{
				return true;
			}
			return false;
		};

		auto Iter = std::find_if(m_vecPanel.begin(),m_vecPanel.end(),FindPred);
		if (Iter != m_vecPanel.end())
		{		
			if ((*Iter)->GetSafeHwnd() != NULL)
			{
				(*Iter)->DestroyWindow();
			}
			m_WidgetLayout.DelWidget(*Iter);
			boost::checked_delete(*Iter);
			m_vecPanel.erase(Iter);	

			CRect rc;
			GetClientRect(&rc);
			m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
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

void CDlgDiagramPanel::NotifyDisplayPanelChange(const LP_SENSOR_TYPE_KEY pSensor,int nFlag)
{
	if (nullptr == pSensor)
	{
		return;
	}

	//获取显示面板的个数
	int nPanelNum = (int)m_vecPanel.size();

	//增加面板
	if (1 == nFlag)
	{
		//超过最大个数
		if (nPanelNum >= MAX_DIAGRAM_PANEL_NUM)
		{
			return;
		}

		//如果只有一个面板
		if (1 == nPanelNum)
		{
			if (nullptr != m_vecPanel[0])
			{
				SENSOR_TYPE_KEY KeyID = m_vecPanel[0]->GetXAxisSensorID();
				//非法传感器
				if (!KeyID.IsValid())
				{
					m_vecPanel[0]->SetXAxisSensorID(CSensorManager::CreateInstance().GetSpecialSensorID());
					m_vecPanel[0]->SetYAxisSensorID(*pSensor);
				}
				//新增一个面板
				else
				{
					ChartFigureDlg* pDiagramPanel = new ChartFigureDlg;
					if (nullptr != pDiagramPanel)
					{		
						pDiagramPanel->Create(ChartFigureDlg::IDD,this);
						pDiagramPanel->SetXAxisSensorID(CSensorManager::CreateInstance().GetSpecialSensorID());
						pDiagramPanel->SetYAxisSensorID(*pSensor);
						m_vecPanel.push_back(pDiagramPanel);
						m_WidgetLayout.AddWidget(pDiagramPanel);
					}
				}
			}
		}
		else
		{
			ChartFigureDlg* pDiagramPanel = new ChartFigureDlg;
			if (nullptr != pDiagramPanel)
			{
				pDiagramPanel->Create(ChartFigureDlg::IDD,this);
				pDiagramPanel->SetXAxisSensorID(CSensorManager::CreateInstance().GetSpecialSensorID());
				pDiagramPanel->SetYAxisSensorID(*pSensor);
				m_vecPanel.push_back(pDiagramPanel);
				m_WidgetLayout.AddWidget(pDiagramPanel);
			}
		}
	}
	//删除面板
	else
	{
		if (1 == nPanelNum)
		{
			if (nullptr != m_vecPanel[0])
			{
				m_vecPanel[0]->SetXAxisSensorID(SENSOR_TYPE_KEY());
				m_vecPanel[0]->RestYAxisSensorID();
			}
		}
		else
		{

			auto FindPred = [pSensor](const ChartFigureDlg* pElement)->bool
			{
				if (pElement->IsYAxisSensorIDExist(*pSensor))
				{
					return true;
				}
				return false;
			};

			auto Iter = std::find_if(m_vecPanel.begin(),m_vecPanel.end(),FindPred);
			if (Iter != m_vecPanel.end())
			{		
				if ((*Iter)->GetSafeHwnd() != NULL)
				{
					(*Iter)->DestroyWindow();
				}
				m_WidgetLayout.DelWidget(*Iter);
				boost::checked_delete(*Iter);
				m_vecPanel.erase(Iter);	
			}		
		}	
	}
	CRect rc;
	GetClientRect(&rc);
	m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
}

void CDlgDiagramPanel::NotifyControlsStartRefresh()
{
	BOOST_FOREACH(auto& pGridWnd,m_vecPanel)
	{
		if (pGridWnd != nullptr)
		{
			pGridWnd->NotifyControlsStartRefresh();
		}
	}
}

void CDlgDiagramPanel::NotifyControlsStopRefresh()
{
	BOOST_FOREACH(auto& pGridWnd,m_vecPanel)
	{
		if (pGridWnd != nullptr)
		{
			pGridWnd->NotifyControlsStopRefresh();
		}
	}
}
