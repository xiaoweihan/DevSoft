// DlgDevicePanel.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgDevicePanel.h"
#include <boost/checked_delete.hpp>
#include <boost/foreach.hpp>
#include "GaugeDlg.h"
#include "Macro.h"
// CDlgDevicePanel 对话框

IMPLEMENT_DYNAMIC(CDlgDevicePanel, CBaseDialog)

CDlgDevicePanel::CDlgDevicePanel(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgDevicePanel::IDD, pParent)
	, m_WidgetLayout(this, true)
{
}

CDlgDevicePanel::~CDlgDevicePanel()
{
	for (auto i = 0; i < (int)m_vecPanel.size(); ++i)
	{
		if (NULL != m_vecPanel[i])
		{
			boost::checked_delete(m_vecPanel[i]);
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
		//m_dataManager.addGaugeDlg(pDevicePanel);
	}
}
void CDlgDevicePanel::addPanel()
{
	GaugeDlg* pDevicePanel = new GaugeDlg;
	if (nullptr != pDevicePanel)
	{
		pDevicePanel->Create(GaugeDlg::IDD,this);
		m_vecPanel.push_back(pDevicePanel);
		m_WidgetLayout.AddWidget(pDevicePanel);
		CRect rc;
		GetClientRect(&rc);
		m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
	}

}
void CDlgDevicePanel::delPanel(CWnd* pDlg)
{
	if(nullptr != pDlg)
	{
		m_WidgetLayout.DelWidget(pDlg);
		CRect rc;
		GetClientRect(&rc);
		m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
		for(auto Iter = m_vecPanel.begin();Iter != m_vecPanel.end(); ++Iter)
		{
			if(pDlg == *Iter)
			{
				if (pDlg->GetSafeHwnd() != NULL)
				{
					pDlg->DestroyWindow();
				}
				m_vecPanel.erase(Iter);
				break;
			}
		}
		boost::checked_delete(pDlg);
	}	
}
void CDlgDevicePanel::DestroyPanel( void )
{

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


void CDlgDevicePanel::NotifyDisplayPanelChange(const LP_SENSOR_TYPE_KEY pSensor,int nFlag)
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
		if (nPanelNum >= MAX_DEVICE_PANEL_NUM)
		{
			return;
		}

		//如果只有一个面板，查看是否是正常使用的
		if (1 == nPanelNum)
		{
			if (nullptr != m_vecPanel[0])
			{
				//获取已经存在的显示传感器的ID
				SENSOR_TYPE_KEY KeyID = m_vecPanel[0]->getShowDataColumn();

				//非法的传感器ID
				if (KeyID.nSensorID < 0 && KeyID.nSensorSerialID < 0)
				{
					m_vecPanel[0]->setShowDataColumn(SENSOR_TYPE_KEY(pSensor->nSensorID,pSensor->nSensorSerialID));
				}
				//如果是合法的传感器，那么就新增一个显示面板
				else
				{
					GaugeDlg* pDevicePanel = new GaugeDlg;
					if (nullptr != pDevicePanel)
					{
						pDevicePanel->setShowDataColumn(*pSensor);
						pDevicePanel->Create(GaugeDlg::IDD,this);
						m_vecPanel.push_back(pDevicePanel);
						m_WidgetLayout.AddWidget(pDevicePanel);
						CRect rc;
						GetClientRect(&rc);
						m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
					}
				}
			}
		}
		else
		{
			GaugeDlg* pDevicePanel = new GaugeDlg;
			if (nullptr != pDevicePanel)
			{
				pDevicePanel->setShowDataColumn(*pSensor);
				pDevicePanel->Create(GaugeDlg::IDD,this);
				m_vecPanel.push_back(pDevicePanel);
				m_WidgetLayout.AddWidget(pDevicePanel);
				CRect rc;
				GetClientRect(&rc);
				m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
			}
		}
	}
	//删除面板
	else
	{
		//如果只剩最后一个
		if (1 == nPanelNum)
		{
			if (nullptr != m_vecPanel[0])
			{
				m_vecPanel[0]->setShowDataColumn(SENSOR_TYPE_KEY());
			}
		}
		else
		{	
			auto FindPred = [&pSensor](const GaugeDlg* pElement)->bool
			{
				if (pElement->getShowDataColumn() == *pSensor)
				{
					return true;
				}
				return false;
			};
			//找到对话框
			auto Iter = std::find_if(m_vecPanel.begin(),m_vecPanel.end(),FindPred);
			if (Iter != m_vecPanel.end())
			{
				m_WidgetLayout.DelWidget(*Iter);
				if ((*Iter)->GetSafeHwnd() != NULL)
				{
					(*Iter)->DestroyWindow();
				}
				boost::checked_delete((*Iter));
				m_vecPanel.erase(Iter);
				CRect rc;
				GetClientRect(&rc);
				m_WidgetLayout.AdjustLayout(rc.Width(),rc.Height());
			}
		}
	}
}
