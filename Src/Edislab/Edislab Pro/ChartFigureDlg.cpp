// ChartFigureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "ChartFigureDlg.h"
#include "afxdialogex.h"
#include "DlgChartSet.h"
#include "GlobalDataManager.h"
#include "DlgTabPanel.h"
#include "Msg.h"
// ChartFigureDlg 对话框
IMPLEMENT_DYNAMIC(ChartFigureDlg, CBaseDialog)

ChartFigureDlg::ChartFigureDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(ChartFigureDlg::IDD, pParent)
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
	CDialog::OnPaint();
	if (m_charxy)
		m_charxy->paintEvent();
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
			CRgn rgn;
			rgn.CreateRectRgnIndirect(rc);
			dc.SelectClipRgn(&rgn);
			if(this==pTabPanel->GetActiveDlg())//当前窗口激活
			{
				CPen BoradrPen;
				BoradrPen.CreatePen(PS_SOLID,5,ActiveColor);
				CPen* pOldPen = dc.SelectObject(&BoradrPen);
				CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
				CBrush *pOldBrush = dc.SelectObject(pBrush);  
				dc.Rectangle(&rc);
				dc.SelectObject(pOldPen);
				dc.SelectObject(pOldBrush);
				BoradrPen.DeleteObject();
			}else
			{
				CPen BoradrPen;
				BoradrPen.CreatePen(PS_SOLID,5,UnActiveColor);
				CPen* pOldPen = dc.SelectObject(&BoradrPen);
				CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
				CBrush *pOldBrush = dc.SelectObject(pBrush);
				dc.Rectangle(&rc);
				dc.SelectObject(pOldPen);
				dc.SelectObject(pOldBrush);
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
	// TODO: Add your message handler code here
	CMenu   menu;   //定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_MENU_CHART); //装载自定义的右键菜单 
	CMenu   *pContextMenu=menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单 
	if(pContextMenu)
	{
		//CPoint point1;//定义一个用于确定光标位置的位置  
		//GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标  
		pContextMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point.x,point.y,this); 
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
