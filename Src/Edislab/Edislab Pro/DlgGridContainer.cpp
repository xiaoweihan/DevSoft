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
#include "SensorData.h"
#include "SensorDataManager.h"
#include "Msg.h"
const int TIMER_ID = 1;
const int TIMER_GAP = 3000;
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
				CString strColumnName = pGridCtrl->GetColumnName(nCol);

				int nSensorID = CGridDisplayColumnInfo::CreateInstance().QuerySensorIDByColumnName(strColumnName);

				if (nSensorID < 0)
				{
					pdi->item.varValue = nRow + 1;
				}
				else
				{
					//根据传感器ID获取传感器数据
					CSensorData* pData = CSensorDataManager::CreateInstance().GetSensorDataBySensorID(nSensorID);
					if (nullptr != pData)
					{
						pdi->item.varValue = pData->GetSensorData(nRow);
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
     m_bLoop(false)
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
END_MESSAGE_MAP()


// CDlgGridContainer 消息处理程序


BOOL CDlgGridContainer::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	EnableVisualManagerStyle(TRUE,TRUE);
	CSensorDataManager::CreateInstance().AddSensorData(1);

	if (!m_pYieldDataThread)
	{
		m_bLoop = true;

		m_pYieldDataThread = boost::make_shared<boost::thread>(boost::bind(&CDlgGridContainer::YieldDataProc,this));
	}


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
			m_DisplayGrid.SetDisplayVirtualRows(60);
			m_DisplayGrid.SetCallBack(GridCallback);
			m_DisplayGrid.Create(WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,CCustomGrid::s_GridID++);
			//m_DisplayGrid.FillData();
		}	
	}
	SetTimer(TIMER_ID,TIMER_GAP,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgGridContainer::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (NULL != m_DisplayGrid.GetSafeHwnd())
	{
		m_DisplayGrid.MoveWindow(0,0,cx,cy);
	}
}


BOOL CDlgGridContainer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message == WM_KEYDOWN)
	{
		if (VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam)
		{
			return TRUE;
		}
	}

	if (WM_LBUTTONDOWN == pMsg->message|| WM_RBUTTONDOWN == pMsg->message)
	{
		CWnd* pWnd = AfxGetMainWnd();
		if (NULL != pWnd)
		{
			pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,0,0);
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

void CDlgGridContainer::RefreshGrid(void)
{
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}
#if 0
	static int nIndex = 0;
	CBCGPGridRow* pRow = nullptr;
	CBCGPGridItem* pItem = nullptr;
	for (int i = 0; i < m_DisplayGrid.GetRowCount(); ++i)
	{
		pRow = m_DisplayGrid.GetRow(i);
		
		if (nullptr != pRow)
		{
			pItem = pRow->GetItem(1);

			CString strIndex;
			strIndex.Format(_T("%d"),nIndex++);
			if (nullptr != pItem)
			{
				pItem->SetValue((_variant_t)strIndex);
			}
		}

	}
	m_DisplayGrid.AdjustLayout();
#endif

	m_DisplayGrid.Refresh();
	
}


void CDlgGridContainer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBCGPDialog::OnPaint()
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
