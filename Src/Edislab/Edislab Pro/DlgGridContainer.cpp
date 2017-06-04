// DlgGridContainer.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgGridContainer.h"
#include <boost/random.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include "Log.h"
#include "GridDisplayColumnInfo.h"
#include "SensorIDGenerator.h"
#include "SensorData.h"
#include "SensorDataManager.h"
#include "DlgTabPanel.h"
#include "Msg.h"
#include "Global.h"
#include "Utility.h"
#include "DlgGridOpt.h"
const int TIMER_ID = 1;
const int TIMER_GAP = 1000;
// CDlgGridContainer 对话框
#pragma warning(disable:4244)
static BOOL CALLBACK GridCallback (BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	if (nullptr != pdi)
	{
		CCustomGrid* pGridCtrl = (CCustomGrid*)lp;
		if (nullptr != pGridCtrl)
		{
			//行索引
			int nRow = pdi->item.nRow;
			//列索引
			int nCol = pdi->item.nCol;

			if (nCol >= 0 && nRow >= 0)
			{
				//获取列名称
				CString strTempColumnName = pGridCtrl->GetColumnName(nCol);
				std::string strColumnName = Utility::WideChar2MultiByte(strTempColumnName.GetBuffer(0));

				int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(strColumnName);
				if (nSensorID >= 0)
				{
					//根据传感器ID获取传感器数据
					CSensorData* pData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(nSensorID);
					if (nullptr != pData)
					{
						float fValue = 0.0f;
						if (pData->GetSensorData(nRow,fValue))
						{
							pdi->item.varValue = fValue;
						}		
					}
				}
			}
		}
	}
	return TRUE;
}

IMPLEMENT_DYNAMIC(CDlgGridContainer, CBCGPDialog)

CDlgGridContainer::CDlgGridContainer(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgGridContainer::IDD, pParent),
     m_bLoop(false),
	 m_bActiveFlag(FALSE)
{
}

CDlgGridContainer::~CDlgGridContainer()
{
}

void CDlgGridContainer::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgGridContainer, CBCGPDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_MESSAGE(WM_NOTIFY_RBUTTON_DOWN,&CDlgGridContainer::NotifyGridClickRButton)
	ON_COMMAND(ID_MENU_GRID_OPTION, &CDlgGridContainer::OnMenuGridOption)
	ON_MESSAGE(WM_SET_DLG_ACTIVE,&CDlgGridContainer::NotifyActive)
END_MESSAGE_MAP()


// CDlgGridContainer 消息处理程序


BOOL CDlgGridContainer::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	EnableVisualManagerStyle(TRUE,TRUE);
	//if (!m_pYieldDataThread)
	//{
	//	m_bLoop = true;

	//	m_pYieldDataThread = boost::make_shared<boost::thread>(boost::bind(&CDlgGridContainer::YieldDataProc,this));
	//}
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		std::vector<HEADRER_INFO> HeaderInfoArray;
		std::vector<GRID_DISPLAY_INFO> GridDisplayInfoArray;
		//获取要显示的列信息
		CGridDisplayColumnInfo::CreateInstance().GetGridDisplayInfo(GridDisplayInfoArray);
		//显示信息为空则返回
		if (!GridDisplayInfoArray.empty())
		{
			HEADRER_INFO TempHeaderInfo;
			BOOST_FOREACH(auto& V,GridDisplayInfoArray)
			{
				TempHeaderInfo.strHeadName = V.strHeadName;
				BOOST_FOREACH(auto& ColumnElement,V.ContainColumnIndexArray)
				{
					TempHeaderInfo.ContainColumnIndexArray.push_back(ColumnElement.strColumnName);
				}
				HeaderInfoArray.push_back(TempHeaderInfo);
				TempHeaderInfo.Reset();
			}
			m_DisplayGrid.SetHeaderInfoArray(HeaderInfoArray);
			m_DisplayGrid.SetDisplayVirtualRows(600);
			m_DisplayGrid.SetCallBack(GridCallback);
			m_DisplayGrid.Create(WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,CCustomGrid::s_GridID++);
		}
	}
	SetTimer(TIMER_ID,TIMER_GAP,NULL);
	return TRUE;
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgGridContainer::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (NULL != m_DisplayGrid.GetSafeHwnd())
	{
		CRect rc(0,0,cx,cy);
		rc.DeflateRect(5,5);
		m_DisplayGrid.MoveWindow(&rc);
	}
}


BOOL CDlgGridContainer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//告诉布局管理器当前活动的窗口
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			CWnd* pGridPanel = GetParent();

			if (nullptr != pGridPanel)
			{
				if (nullptr != pGridPanel)
				{
					CDlgTabPanel* pTabWnd = dynamic_cast<CDlgTabPanel*>(pGridPanel->GetParent());

					if (nullptr != pTabWnd)
					{
						pTabWnd->SetActive(CDlgTabPanel::GRID_INDEX,this);
					}
				}
			}
		}
		break;
	default:
		break;
	}

	//向主窗口发送消息告诉主窗口当前活动页
	if (WM_LBUTTONDOWN == pMsg->message|| WM_RBUTTONDOWN == pMsg->message)
	{
		CWnd* pWnd = AfxGetMainWnd();
		if (NULL != pWnd)
		{
			pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,0,0);
		}
	}

	//屏蔽Enter和ESC
	if (pMsg->message == WM_KEYDOWN)
	{
		if (VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam)
		{
			return TRUE;
		}
	}

	return CBCGPDialog::PreTranslateMessage(pMsg);
}


void CDlgGridContainer::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//定时刷新数据
	if (nIDEvent == TIMER_ID)
	{
		RefreshGrid();
	}

	CBCGPDialog::OnTimer(nIDEvent);
}


void CDlgGridContainer::OnDestroy()
{
	CBCGPDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	m_bLoop = false;

	if (m_pYieldDataThread)
	{
		m_pYieldDataThread->join();
	}
	KillTimer(TIMER_ID);
}

void CDlgGridContainer::NotifyDetectSensor(const std::string& strDeviceName,int nOnFlag)
{
	if (m_DisplayGrid.GetSafeHwnd() == NULL)
	{
		return;
	}

	if (strDeviceName.empty())
	{
		return;
	}

	//上线
	if (nOnFlag)
	{
		m_DisplayGrid.AddColumnInfo(_T("当前"),CString(strDeviceName.c_str()));
	}
	//下线
	else
	{
		m_DisplayGrid.RemoveColumn(CString(strDeviceName.c_str()));
	}
}

LRESULT CDlgGridContainer::NotifyGridClickRButton(WPARAM wp,LPARAM lp)
{
	CPoint pt;
	pt.x = (int)wp;
	pt.y = (int)lp;
	CBCGPContextMenuManager* pContexMenuManager = theApp.GetContextMenuManager();
	if (nullptr != pContexMenuManager)
	{
		pContexMenuManager->ShowPopupMenu(IDR_MENU_GRID,pt.x,pt.y,this,TRUE);
	}
	return 0L;
}

void CDlgGridContainer::RefreshGrid(void)
{
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}
	m_DisplayGrid.Refresh();
}


void CDlgGridContainer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//描绘边框
	CWnd* pGridPanel = GetParent();
	if(nullptr != pGridPanel)
	{
		CDlgTabPanel* pTabPanel = dynamic_cast<CDlgTabPanel*>(pGridPanel->GetParent());
		if(nullptr != pTabPanel)
		{
			CRect rc;
			GetClientRect(rc);
			//if(this == pTabPanel->GetActiveDlg())//当前窗口激活
			if (TRUE == m_bActiveFlag)
			{
				CPen BoradrPen;
				BoradrPen.CreatePen(PS_SOLID,5,ActiveColor);
				CPen* pOldPen = dc.SelectObject(&BoradrPen);
				
				dc.MoveTo(rc.left,rc.top);
				dc.LineTo(rc.right,rc.top);

				dc.MoveTo(rc.right,rc.top);
				dc.LineTo(rc.right,rc.bottom);

				dc.MoveTo(rc.right,rc.bottom);
				dc.LineTo(rc.left,rc.bottom);

				dc.MoveTo(rc.left,rc.bottom);
				dc.LineTo(rc.left,rc.top);

				dc.SelectObject(pOldPen);
				BoradrPen.DeleteObject();
			}
			else
			{
				CPen BoradrPen;
				BoradrPen.CreatePen(PS_SOLID,5,UnActiveColor);
				CPen* pOldPen = dc.SelectObject(&BoradrPen);
				dc.MoveTo(rc.left,rc.top);
				dc.LineTo(rc.right,rc.top);

				dc.MoveTo(rc.right,rc.top);
				dc.LineTo(rc.right,rc.bottom);

				dc.MoveTo(rc.right,rc.bottom);
				dc.LineTo(rc.left,rc.bottom);

				dc.MoveTo(rc.left,rc.bottom);
				dc.LineTo(rc.left,rc.top);

				dc.SelectObject(pOldPen);
				BoradrPen.DeleteObject();
			}
		}
	}
}

void CDlgGridContainer::YieldDataProc( void )
{
	//模拟生产数据
	CSensorData* pData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(1);

	if (nullptr == pData)
	{
		return;
	}
	boost::mt19937 rng(time(nullptr));
	boost::random::uniform_real_distribution<float> Ui(100.0f,110.0f);
	while (m_bLoop)
	{
		pData->AddSensorData(Ui(rng));	
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	}
}


void CDlgGridContainer::OnMenuGridOption()
{
	//弹出选项对话框
	CDlgGridOpt Dlg(this);

	if (IDOK == Dlg.DoModal())
	{

	}
}

LRESULT CDlgGridContainer::NotifyActive( WPARAM wp,LPARAM lp )
{
	int nActiveFlag = (int)wp;

	if (nActiveFlag)
	{
		m_bActiveFlag = TRUE;
	}
	else
	{
		m_bActiveFlag = FALSE;
	}
	return 0L;
}
