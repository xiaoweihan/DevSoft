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
#include "GridColumnGroupManager.h"
#include "SensorIDGenerator.h"
#include "SensorData.h"
#include "SensorDataManager.h"
#include "SensorConfig.h"
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
				if (0 == nCol)
				{
					CString strTempColumnName = pGridCtrl->GetColumnName(nCol);
					//名称相等才显示
					if (strTempColumnName == _T("t(s)时间"))
					{
						//采样周期获取
						const SENSOR_RECORD_INFO& SampleInfo = CSensorConfig::CreateInstance().GetSensorRecordInfo();
						double fPeriod = 1.0 / (SampleInfo.fFrequency);
						CString strContent;
						strContent.Format(_T("%.6f"),fPeriod * nRow);
						
						pdi->item.varValue = strContent;
					}
				}
				else
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
	ON_COMMAND(ID_MENU_GRID_COPY, &CDlgGridContainer::OnMenuGridCopy)
	ON_COMMAND(ID_MENU_GRID_PASTE, &CDlgGridContainer::OnMenuGridPaste)
	ON_COMMAND(ID_MENU_GRID_DEL_CELL, &CDlgGridContainer::OnMenuGridDelCell)
	ON_COMMAND(ID_MENU_GRID_FIRST_ROW, &CDlgGridContainer::OnMenuGridFirstRow)
	ON_COMMAND(ID_MENU_GRID_LAST_ROW, &CDlgGridContainer::OnMenuGridLastRow)
END_MESSAGE_MAP()


// CDlgGridContainer 消息处理程序


BOOL CDlgGridContainer::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	EnableVisualManagerStyle(TRUE,TRUE);
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		std::vector<COLUMN_GROUP_INFO> ColumnGroupArray;
		//获取要显示的列信息
		CGridColumnGroupManager::CreateInstance().GetGridDisplayInfo(ColumnGroupArray);
		//显示信息为空则返回
		if (ColumnGroupArray.empty())
		{
			COLUMN_GROUP_INFO TempGroupInfo;
			TempGroupInfo.strGroupName = _T("当前");
			COLUMN_INFO TempColumnInfo;
			TempColumnInfo.strColumnName = _T("X");
			TempGroupInfo.ColumnArray.push_back(TempColumnInfo);
			TempColumnInfo.Reset();
			TempColumnInfo.strColumnName = _T("Y");
			TempGroupInfo.ColumnArray.push_back(TempColumnInfo);
			ColumnGroupArray.push_back(TempGroupInfo);
		}

		const SENSOR_RECORD_INFO& SampleInfo = CSensorConfig::CreateInstance().GetSensorRecordInfo();

		//设置点数
		int nTotalRows = (int)(SampleInfo.fFrequency * SampleInfo.fLimitTime);
		m_DisplayGrid.SetHeaderInfoArray(ColumnGroupArray);
		nTotalRows = (std::max)(nTotalRows,60);
		m_DisplayGrid.SetDisplayVirtualRows(nTotalRows);
		m_DisplayGrid.SetCallBack(GridCallback);
		m_DisplayGrid.Create(WS_VISIBLE | WS_CHILD,CRect(0,0,0,0),this,CCustomGrid::s_GridID++);
	}
	//SetTimer(TIMER_ID,TIMER_GAP,NULL);
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
	if (m_DisplayGrid.GetSafeHwnd() == NULL || strDeviceName.empty())
	{
		return;
	}
	//获取当前需要显示的分组信息
	std::vector<COLUMN_GROUP_INFO> ColumnGroupArray;
	CGridColumnGroupManager::CreateInstance().GetGridDisplayInfo(ColumnGroupArray);
	if (ColumnGroupArray.empty())
	{
		COLUMN_GROUP_INFO TempGroupInfo;
		TempGroupInfo.strGroupName = _T("当前");
		COLUMN_INFO TempColumnInfo;
		TempColumnInfo.strColumnName = _T("X");
		TempGroupInfo.ColumnArray.push_back(TempColumnInfo);
		TempColumnInfo.Reset();
		TempColumnInfo.strColumnName = _T("Y");
		TempGroupInfo.ColumnArray.push_back(TempColumnInfo);
		ColumnGroupArray.push_back(TempGroupInfo);
	}
	m_DisplayGrid.DynamicSetHeaderInfoArray(ColumnGroupArray);

#if 0
	//上线
	if (nOnFlag)
	{
		//m_DisplayGrid.AddColumnInfo(_T("当前"),CString(strDeviceName.c_str()));
		


	}
	//下线
	else
	{
		//m_DisplayGrid.RemoveColumn(CString(strDeviceName.c_str()));
	}
#endif
}

void CDlgGridContainer::NotifyGridChangeRows(int nRows)
{
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}
	m_DisplayGrid.SetVirtualRows((std::max)(nRows,60));

	m_DisplayGrid.AdjustLayout();
}

void CDlgGridContainer::NotifyControlsStartRefresh()
{
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}
	SetTimer(TIMER_ID,TIMER_GAP,NULL);
}

void CDlgGridContainer::NotifyControlsStopRefresh()
{
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}
	KillTimer(TIMER_ID);
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

	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}

	//获取分组的显示信息
	std::vector<SHOW_COLUMN_GROUP_INFO> ShowColumnArray;
	m_DisplayGrid.GetColumnGroupDisplayInfo(ShowColumnArray);
	//弹出选项对话框
	CDlgGridOpt Dlg(this);
	Dlg.SetDisplayInfo(ShowColumnArray);
	if (IDOK == Dlg.DoModal())
	{
		//获取显示信息
		ShowColumnArray.clear();
		//获取配置
		Dlg.GetDisplayInfo(ShowColumnArray);

		//设置显示
		m_DisplayGrid.SetColumnGroupDisplayInfo(ShowColumnArray);
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


void CDlgGridContainer::OnMenuGridCopy()
{
	// TODO: 在此添加命令处理程序代码
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}

	m_DisplayGrid.Copy();
}


void CDlgGridContainer::OnMenuGridPaste()
{
	// TODO: 在此添加命令处理程序代码
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}

	m_DisplayGrid.Paste();
}


void CDlgGridContainer::OnMenuGridDelCell()
{
	// TODO: 在此添加命令处理程序代码
	if (NULL == m_DisplayGrid.GetSafeHwnd())
	{
		return;
	}

	m_DisplayGrid.Clear(FALSE);
}


void CDlgGridContainer::OnMenuGridFirstRow()
{
	// TODO: 在此添加命令处理程序代码
	if (NULL != m_DisplayGrid.GetSafeHwnd())
	{
		int nTotalRow = m_DisplayGrid.GetTotalRowCount();
		if (nTotalRow > 0)
		{
			CBCGPGridRow *pFirstRow = m_DisplayGrid.GetRow(0);
			if (nullptr != pFirstRow)
			{
				m_DisplayGrid.EnsureVisible(pFirstRow);
				m_DisplayGrid.SetCurSel(pFirstRow);
			}
		}
	}
}


void CDlgGridContainer::OnMenuGridLastRow()
{
	// TODO: 在此添加命令处理程序代码
	if (NULL != m_DisplayGrid.GetSafeHwnd())
	{
		int nTotalRow = m_DisplayGrid.GetTotalRowCount();
		if (nTotalRow > 0)
		{
			CBCGPGridRow *pLastRow = m_DisplayGrid.GetRow(nTotalRow - 1);

			if (nullptr != pLastRow)
			{
				m_DisplayGrid.EnsureVisible(pLastRow);
				m_DisplayGrid.SetCurSel(pLastRow);
			}
		}
	}
}
