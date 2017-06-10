// ChartFigureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "ChartFigureDlg.h"
#include "DlgChartSet.h"
#include "GlobalDataManager.h"
#include "DlgTabPanel.h"
#include "Msg.h"
// ChartFigureDlg 对话框
IMPLEMENT_DYNAMIC(ChartFigureDlg, CBaseDialog)
#define TIMER_CHART_EVENT 1000011
#define TIMER_CHART 300
ChartFigureDlg::ChartFigureDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(ChartFigureDlg::IDD, pParent),
	m_bActiveFlag(FALSE)
{
	m_charxy = NULL;
}

ChartFigureDlg::~ChartFigureDlg()
{
	if (m_charxy != NULL)
	{
		delete m_charxy;
		m_charxy = NULL;
	}
}

void ChartFigureDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ChartFigureDlg, CBaseDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_ACTIVATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CHART_SET, &ChartFigureDlg::OnChartSet)
	ON_COMMAND(ID_CHART_DEL, &ChartFigureDlg::OnChartDel)
	ON_COMMAND(ID_CHART_ZOOM_IN, &ChartFigureDlg::OnChartZoomIn)
	ON_COMMAND(ID_CHART_ZOOM_OUT, &ChartFigureDlg::OnChartZoomOut)
	ON_COMMAND(ID_CHART_SHOW_ALL, &ChartFigureDlg::OnChartShowAll)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SET_DLG_ACTIVE,&ChartFigureDlg::NotifyActive)
END_MESSAGE_MAP()

BOOL ChartFigureDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDC* pDC = GetDC();
	m_charxy = new ChartXY(pDC->m_hDC);

	//ChartXYData* line = new ChartXYData();
	//std::vector<CMeDPoint> linePts;
	//linePts.push_back(CMeDPoint(0, 1));
	//linePts.push_back(CMeDPoint(2, 1));
	//linePts.push_back(CMeDPoint(3, 10));
	//linePts.push_back(CMeDPoint(4, 7));
	//line->setXYData(linePts);
	//m_charxy->addChartData(line);
	CRect rect;
	GetClientRect(rect);
	CSize size(rect.Width(), rect.Height());
	m_charxy->resize(size);
	SetTimer(TIMER_CHART_EVENT, TIMER_CHART, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void ChartFigureDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBaseDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	TRACE("[GaugeDlg] OnActivate!\r\n");
}
// ChartFigureDlg 消息处理程序
void ChartFigureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	if (m_charxy)
	{
		m_charxy->mousePressEvent(point);
		//Invalidate();
	}

	CWnd* pWnd = AfxGetMainWnd();
	if (NULL != pWnd)
	{
		pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,2,0);
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void ChartFigureDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_charxy)
	{
		m_charxy->mouseReleaseEvent(point);
		//Invalidate();
	}
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}


BOOL ChartFigureDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_charxy)
	{
		CPoint pts = pt;
		ScreenToClient(&pts);
		m_charxy->wheelEvent(pts, zDelta);
		Invalidate();
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void ChartFigureDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_charxy)
	{
		m_charxy->mouseMoveEvent(point);
		//Invalidate();
	}
	CWnd::OnMouseMove(nFlags, point);
}


void ChartFigureDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CWnd::OnPaint()
	//CDialog::OnPaint();
	if (m_charxy)
	{
		m_charxy->paintEvent();
	}
	//描绘边框
	CWnd* parent = GetParent();
	if(parent)
	{
		parent = parent->GetParent();
		CDlgTabPanel* pTabPanel = dynamic_cast<CDlgTabPanel*>(parent);
		if(pTabPanel)
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
			}else
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


void ChartFigureDlg::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_charxy)
	{
		m_charxy->resize(CSize(cx, cy));
		Invalidate();
	}
}


void ChartFigureDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CWnd::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
	Invalidate();
}

void ChartFigureDlg::setChartMgr(ChartManager* mgr)
{
	if (m_charxy)
	{
		m_charxy->setChartMgr(mgr);
	}
}
const ChartManager* ChartFigureDlg::getChartMgr()
{
	if (m_charxy)
	{
		return m_charxy->getChartMgr();
	}
	return NULL;
}
void ChartFigureDlg::updateData(CGlobalDataManager* dbMgr) const
{
	if (m_charxy)
	{
		return m_charxy->updateData(dbMgr);
	}
}
//保存数据
int ChartFigureDlg::saveData()const
{
	//保存当前图表的数据
	return 0;
}
//读取数据
int ChartFigureDlg::readData()
{
	//读取当前图表的数据
	return 0;
}


void ChartFigureDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CBCGPContextMenuManager* pContexMenuManager = theApp.GetContextMenuManager();
	if (nullptr != pContexMenuManager)
	{
		pContexMenuManager->ShowPopupMenu(IDR_MENU_CHART,point.x,point.y,pWnd,TRUE);
	}	
}


void ChartFigureDlg::OnChartSet()
{
	// TODO: Add your command handler code here
	DlgChartSet dlgSet(this);
	//初始化当前值
	dlgSet.m_nXID = m_charxy->getXID();
	dlgSet.m_eLineStyle = m_charxy->getLineStyle();
	dlgSet.m_eMoveStyle = m_charxy->getMoveStyle();
	dlgSet.m_eChartType = m_charxy->getChartType();
	std::map<int, bool> mapV = m_charxy->getMapVisible();
	dlgSet.m_setShowID.clear();
	for(std::map<int, bool>::iterator itr = mapV.begin(); itr!=mapV.end(); ++itr)
	{
		if(itr->second)
		{
			dlgSet.m_setShowID.insert(itr->first);
		}
	}
	if(IDOK==dlgSet.DoModal())
	{
		std::map<int, bool> mapV;
		SET<int> setV = dlgSet.m_setShowID;
		for(SET<int>::iterator itr = setV.begin(); itr!=setV.end(); ++itr)
		{
			mapV[*itr] = true;
		}
		m_charxy->setMapVisible(mapV);

		m_charxy->setXID(dlgSet.m_nXID);
		m_charxy->setMoveStyle(dlgSet.m_eMoveStyle);
		m_charxy->setChartType(dlgSet.m_eChartType);
		m_charxy->setLineStyle(dlgSet.m_eLineStyle);
		m_charxy->refreshData();
		Invalidate();
	}
}


void ChartFigureDlg::OnChartDel()
{
	// TODO: Add your command handler code here
	int a;
	a = 0;
}


void ChartFigureDlg::OnChartZoomIn()
{
	// TODO: Add your command handler code here
}


void ChartFigureDlg::OnChartZoomOut()
{
	// TODO: Add your command handler code here
}


void ChartFigureDlg::OnChartShowAll()
{
	// TODO: Add your command handler code here
}


BOOL ChartFigureDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			CWnd* pDiagramPanel = GetParent();
			if (nullptr != pDiagramPanel)
			{
				CDlgTabPanel* pTabWnd = dynamic_cast<CDlgTabPanel*>(pDiagramPanel->GetParent());

				if (nullptr != pTabWnd)
				{
					pTabWnd->SetActive(CDlgTabPanel::DIAGRAM_INDEX,this);
				}	
			}
		}
		break;
	default:
		break;
	}
	//end add by hanxiaowei


	if (WM_LBUTTONDOWN == pMsg->message || WM_RBUTTONDOWN == pMsg->message)
	{
		CWnd* pWnd = AfxGetMainWnd();
		if (NULL != pWnd)
		{
			pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,2,0);
		}
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}


void ChartFigureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (TIMER_CHART_EVENT == nIDEvent)
	{
		updateData(NULL);
	}
	CBaseDialog::OnTimer(nIDEvent);
}

LRESULT ChartFigureDlg::NotifyActive( WPARAM wp,LPARAM lp )
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
