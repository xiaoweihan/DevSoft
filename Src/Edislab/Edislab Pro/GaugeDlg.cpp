// GaugeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "GaugeDlg.h"
#include "DlgDeviceSet.h"
#include "math.h"
#include "GlobalDataManager.h"
#include "Global.h"
#include "DlgTabPanel.h"
#include "Msg.h"
// GaugeDlg dialog
#pragma warning(disable:4267)
IMPLEMENT_DYNAMIC(GaugeDlg, CBaseDialog)

GaugeDlg::GaugeDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(GaugeDlg::IDD, pParent)
{
	enableWarning = false;
	minRange = 0;
	maxRange = 100;
	maxWarningValue = DBL_MAX;//0X7FFFFFFF;
	minWarningValue = -DBL_MAX;//-9999999999;
	//显示数据列的ID号
	dataColumnID = -1;
	std::vector<GROUPDATA> allData = CGlobalDataManager::CreateInstance().getAllData();
	for(UINT i=0; i<allData.size(); ++i)
	{
		for(UINT c=0; c<allData[i].vecColData.size(); ++c)
		{
			dataColumnID = allData[i].vecColData[c].nColumnID;
			break;
		}
		if(dataColumnID!=-1)
		{
			break;
		}
	}
}

GaugeDlg::~GaugeDlg()
{
	//if(m_pNumeric)
	//{
	//	delete m_pNumeric;
	//	m_pNumeric = NULL;
	//}
}

void GaugeDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAUGE, m_wndGauge);
}


BEGIN_MESSAGE_MAP(GaugeDlg, CBaseDialog)
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_GAUGE_SET, &GaugeDlg::OnGaugeSet)
	ON_COMMAND(ID_GAUGE_DELETE, &GaugeDlg::OnGaugeDelete)
	ON_COMMAND(ID_GAUGE_ZERO, &GaugeDlg::OnGaugeZero)
	ON_UPDATE_COMMAND_UI(ID_GAUGE_ZERO, &GaugeDlg::OnUpdateGaugeZero)
	ON_COMMAND(ID_GAUGE_CORRECT, &GaugeDlg::OnGaugeCorrect)
	ON_UPDATE_COMMAND_UI(ID_GAUGE_CORRECT, &GaugeDlg::OnUpdateGaugeCorrect)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void GaugeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CBaseDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	TRACE("[GaugeDlg] OnActivate!\r\n");
}

BOOL GaugeDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	srand((unsigned)time(NULL));

	m_wndGauge.GetGauge()->SetValue(0);
	updateData(NULL);
	Invalidate(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void GaugeDlg::OnUpdateGauge() 
{
	CBCGPCircularGaugeImpl* pGauge = m_wndGauge.GetGauge();
	ASSERT_VALID(pGauge);
	CRect rc;
	GetClientRect(rc);
	double minE = min(rc.Width(), rc.Height());
	//pGauge->SetFrameSize(minE/30);
	//pGauge->SetCapSize(minE/12);

	pGauge->RemoveAllColoredRanges();

	//if (m_bRange)
	{
		double width1 = 1.;
		double width2 = minE/20;
		double opacity = .5;

		CBCGPBrush brFrame;	// empty, no frame
		CBCGPBrush brGreen(CBCGPColor::White, CBCGPColor(CBCGPColor::Green, opacity), CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT);
		CBCGPBrush brYellow(CBCGPColor::Gold);
		CBCGPBrush brRed(CBCGPColor(CBCGPColor::White, opacity), CBCGPColor::DarkRed, CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT);

		int nScale = 0;
		//警告
		if(enableWarning&&(m_wndGauge.GetGauge()->GetValue()<minWarningValue||m_wndGauge.GetGauge()->GetValue()>maxWarningValue))
		{
			pGauge->ModifyPointer(0, CBCGPCircularGaugePointer(
				CBCGPBrush(CBCGPColor(CBCGPColor::Red), CBCGPColor(CBCGPColor::White), CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT), 
				CBCGPBrush(CBCGPColor(CBCGPColor::Red))));
		}else
		{
			pGauge->ModifyPointer(0, CBCGPCircularGaugePointer(
				CBCGPBrush(CBCGPColor(CBCGPColor::BurlyWood), CBCGPColor(CBCGPColor::White), CBCGPBrush::BCGP_GRADIENT_DIAGONAL_LEFT), 
				CBCGPBrush(CBCGPColor(CBCGPColor::BurlyWood))));
		}
		
		pGauge->AddColoredRange(minRange, minRange+(maxRange-minRange)/3, brGreen, brFrame, nScale, width1, width2);
		pGauge->AddColoredRange(minRange+(maxRange-minRange)/3, minRange+(maxRange-minRange)*2/3, brYellow, brFrame, nScale, width2);
		pGauge->AddColoredRange( minRange+(maxRange-minRange)*2/3, maxRange, brRed, brFrame, nScale, width2, width1);
	}

	pGauge->SetTextLabelFormat(_T("")/*_T("%.0f")*/);

	pGauge->SetTickMarkSize(minE/30, TRUE);
	pGauge->SetTickMarkSize(minE/60 , FALSE);

	pGauge->RemoveAllSubGauges();
	int fontSize = min(rc.Width(), rc.Height())/10;
	CString valueStr;
	valueStr.Format(_T("%.2f"), m_wndGauge.GetGauge()->GetValue());
	valueStr += m_strUnit;
	m_pValueGauge = new CBCGPTextGaugeImpl(valueStr, CBCGPColor::LawnGreen);
	CBCGPTextFormat TextFormat = m_pValueGauge->GetTextFormat();
	TextFormat.SetFontSize(float(fontSize));
	m_pValueGauge->SetTextFormat(TextFormat);

	m_pNameGauge = new CBCGPTextGaugeImpl(m_strTitle, CBCGPColor::DarkGray);
	TextFormat = m_pValueGauge->GetTextFormat();
	TextFormat.SetFontSize(float(fontSize));
	m_pNameGauge->SetTextFormat(TextFormat);
	//m_pValueGauge->SetText(m_strText);
	pGauge->AddSubGauge(m_pValueGauge, 
		CBCGPGaugeImpl::BCGP_SUB_GAUGE_TOP, CBCGPSize(0, 0), CBCGPPoint(0, 0));
	pGauge->AddSubGauge(m_pNameGauge, 
		CBCGPGaugeImpl::BCGP_SUB_GAUGE_BOTTOM, CBCGPSize(0, 0), CBCGPPoint(0, 0));
	//if (m_bIcon)
	{
		//pGauge->AddSubGauge(new CBCGPImageGaugeImpl(IDB_IMAGE1), CBCGPGaugeImpl::BCGP_SUB_GAUGE_BOTTOM);
	}

	m_wndGauge.RedrawWindow();
}
// GaugeDlg message handlers



void GaugeDlg::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_wndGauge.GetSafeHwnd())
	{
		m_wndGauge.MoveWindow(5, 5, cx-10, cy-10);
	}
	Invalidate(TRUE);
}


void GaugeDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CBaseDialog::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
	Invalidate(TRUE);
}
void GaugeDlg::setRange(double min, double max)
{
	minRange = min;
	maxRange = max;
}
void GaugeDlg::getRange(double& min, double& max)
{
	min = minRange;
	max = maxRange;
}
void GaugeDlg::setWarningValue(double min, double max)
{
	minWarningValue = min;
	maxWarningValue = max;
}
void GaugeDlg::getWarningValue(double& min, double& max)
{
	min = minWarningValue;
	max = maxWarningValue;
}
void GaugeDlg::setShowDataColumn(int ColumnID)
{
	dataColumnID = ColumnID;
}
int GaugeDlg::getShowDataColumn()
{
	return dataColumnID;
}
void GaugeDlg::setValue(double value)
{
	m_wndGauge.GetGauge()->SetValue(value);
	Invalidate(TRUE);
}
double GaugeDlg::getValue()
{
	return m_wndGauge.GetGauge()->GetValue();
}
void GaugeDlg::setEnableWarning(bool warning)
{
	enableWarning = warning;
}
bool GaugeDlg::isEnableWarning()
{
	return enableWarning;
}
//保存数据
int GaugeDlg::saveData()
{
	//保存当前窗口的数据
	return 0;
}
//读取数据
int GaugeDlg::readData()
{
	//读取当前窗口的数据
	return 0;
}


void GaugeDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu   menu;   //定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_MENU_GAUGE); //装载自定义的右键菜单 
	CMenu   *pContextMenu=menu.GetSubMenu(0); //获取第一个弹出菜单，所以第一个菜单必须有子菜单 
	if(pContextMenu)
	{
		//CPoint point1;//定义一个用于确定光标位置的位置  
		//GetCursorPos(&point1);//获取当前光标的位置，以便使得菜单可以跟随光标  
		int res = pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON /*| TPM_RETURNCMD | TPM_NONOTIFY*/,point.x,point.y,this);
		//switch(res)
		//{
		//case ID_GAUGE_SET:
		//	OnGaugeSet();
		//	break;
		//case ID_GAUGE_DELETE:
		//	OnGaugeDelete();
		//	break;
		//case ID_GAUGE_ZERO:
		//	OnGaugeZero();
		//	break;
		//case ID_GAUGE_CORRECT:
		//	OnGaugeCorrect();
		//	break;
		//}
	}
}


void GaugeDlg::OnGaugeSet()
{
	// TODO: Add your command handler code here
	DlgDeviceSet dlgSet(this);
	//设置初始值
	dlgSet.setDataColumnID(dataColumnID);
	dlgSet.setWarningState(enableWarning);
	if(enableWarning)
	{
		dlgSet.setWarningType(maxWarningValue<DBL_MAX?0:1);
		dlgSet.setWarningValue(maxWarningValue<DBL_MAX?maxWarningValue:minWarningValue);
	}
	if(IDOK==dlgSet.DoModal())
	{
		//获取值
		dataColumnID = dlgSet.getDataColumnID();
		enableWarning = dlgSet.getWarningState();
		if(enableWarning)
		{
			int warningType = dlgSet.getWarningType();
			if(0==warningType)//max
			{
				maxWarningValue = dlgSet.getWarningValue();
				minWarningValue = -DBL_MAX;
			}else//min
			{
				minWarningValue = dlgSet.getWarningValue();
				maxWarningValue = DBL_MAX;
			}
		}else
		{
			minWarningValue = -DBL_MAX;
			maxWarningValue = DBL_MAX;
		}
		//更新
		updateData(NULL);
	}
}


void GaugeDlg::OnGaugeDelete()
{
	// TODO: Add your command handler code here
	//删除当前窗口
}


void GaugeDlg::OnGaugeZero()
{
	// TODO: Add your command handler code here
}


void GaugeDlg::OnUpdateGaugeZero(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void GaugeDlg::OnGaugeCorrect()
{
	// TODO: Add your command handler code here
}


void GaugeDlg::OnUpdateGaugeCorrect(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void GaugeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CBaseDialog::OnPaint() for painting messages
	OnUpdateGauge();
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
void GaugeDlg::updateData(CGlobalDataManager* dbMgr)
{
	//更新当前值
	//添加数据列名称及ID
	std::vector<GROUPDATA> allData = CGlobalDataManager::CreateInstance().getAllData();
	for(UINT i=0; i<allData.size(); ++i)
	{
		for(UINT c=0; c<allData[i].vecColData.size(); ++c)
		{
			if(dataColumnID==allData[i].vecColData[c].nColumnID)
			{
				m_strTitle = allData[i].vecColData[c].strColumnName;
				m_strUnit = allData[i].vecColData[c].strColumnUnit;
				OnUpdateGauge();
				if(allData[i].vecColData[c].data.size()>0)
				{
					for(UINT v=allData[i].vecColData[c].data.size()-1; v>=0; --v)
					{
						if(allData[i].vecColData[c].data[v].IsEmpty())
						{
							continue;
						}
						setValue(_ttof(allData[i].vecColData[c].data[v]));
						break;
					}
				}
				break;
			}
		}
	}
	Invalidate(TRUE);
}


void GaugeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBaseDialog::OnLButtonDown(nFlags, point);
}


BOOL GaugeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_LBUTTONDOWN == pMsg->message||WM_RBUTTONDOWN==pMsg->message)
	{
		CWnd* pWnd = AfxGetMainWnd();
		if (NULL != pWnd)
		{
			pWnd->PostMessage(WM_NOTIFY_ACTIVE_WND_TYPE,1,0);
		}
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
