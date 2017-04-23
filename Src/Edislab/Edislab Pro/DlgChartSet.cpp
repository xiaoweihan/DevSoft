// DlgChartSet.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DlgChartSet.h"
#include "afxdialogex.h"
#include "GlobalDataManager.h"
#pragma warning(push)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
// DlgChartSet dialog

IMPLEMENT_DYNAMIC(DlgChartSet, CDialog)

DlgChartSet::DlgChartSet(CWnd* pParent /*=NULL*/)
	: CDialog(DlgChartSet::IDD, pParent)
{
	m_nXID = -1;
	m_eChartType = E_CHART_LINE;
	m_eLineStyle = E_LINE_LINE;
	m_eMoveStyle = E_Y_SCROLL;
	m_treeY.m_bVisualManagerStyle = TRUE;

}

DlgChartSet::~DlgChartSet()
{
}

void DlgChartSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_XAXIS, m_combX);
	DDX_Control(pDX, IDC_TREE_YAXIS, m_treeY);
	DDX_Control(pDX, IDC_COMBO_LAB, m_combMoveStyle);
	DDX_Control(pDX, IDC_COMBO_FIGURE_TYPE, m_combChartType);
	DDX_Control(pDX, IDC_COMBO_LINE_TYPE, m_combLineStyle);
}


BEGIN_MESSAGE_MAP(DlgChartSet, CDialog)
	ON_BN_CLICKED(IDOK, &DlgChartSet::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgChartSet message handlers


BOOL DlgChartSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//添加数据列名称及ID
	std::vector<GROUPDATA> allData = CGlobalDataManager::CreateInstance().getAllData();
	//X轴
	for(int i=0; i<allData.size(); ++i)
	{
		for(int c=0; c<allData[i].vecColData.size(); ++c)
		{
			int index = m_combX.AddString(allData[i].vecColData[c].strColumnName);
			m_combX.SetItemData(index, allData[i].vecColData[c].nColumnID);
		}
	}
	m_combX.SetCurSel(0);
	for(int i=0; i<m_combX.GetCount(); ++i)
	{
		if(m_nXID==m_combX.GetItemData(i))
		{
			m_combX.SetCurSel(i);
			break;
		}
	}
	//Y轴
	//m_treeY.SetImageList(NULL, TVSIL_NORMAL);
	m_treeY.ModifyStyle( TVS_CHECKBOXES, 0 );
	m_treeY.ModifyStyle( 0, TVS_CHECKBOXES );
	for(int i=0; i<allData.size(); ++i)
	{
		HTREEITEM hGroup;
		hGroup = m_treeY.InsertItem(allData[i].strGroupName);
		m_treeY.SetItemData(hGroup, allData[i].nGroupID);
		for(int c=0; c<allData[i].vecColData.size(); ++c)
		{
			HTREEITEM hCol = m_treeY.InsertItem(allData[i].vecColData[c].strColumnName, hGroup);
			m_treeY.SetItemData(hCol, allData[i].vecColData[c].nColumnID);
			if(m_setShowID.find(allData[i].vecColData[c].nColumnID)!=m_setShowID.end())
			{
				m_treeY.SetCheck(hCol, TRUE);//选择
			}
		}
		m_treeY.Expand(hGroup, TVE_EXPAND);
	}
	HTREEITEM hGroup = m_treeY.GetRootItem();
	while(hGroup)
	{
		HTREEITEM hCol = m_treeY.GetChildItem(hGroup);
		while(hCol)
		{
			int id = static_cast<int>(m_treeY.GetItemData(hCol));
			if(m_setShowID.find(id)!=m_setShowID.end())
			{
				m_treeY.SetCheck(hCol, TRUE);//选择
			}
			hCol = m_treeY.GetNextSiblingItem(hCol);
		}
		hGroup = m_treeY.GetNextSiblingItem(hGroup);
	}
	//实验时
	int index = m_combMoveStyle.AddString(_T("横轴翻页"));
	m_combMoveStyle.SetItemData(index, E_Y_SCROLL);
	index = m_combMoveStyle.AddString(_T("横轴全显"));
	m_combMoveStyle.SetItemData(index, E_Y_SHOWALL);
	index = m_combMoveStyle.AddString(_T("手动"));
	m_combMoveStyle.SetItemData(index, E_Y_HANDLE);
	for(int i=0; i<m_combMoveStyle.GetCount(); ++i)
	{
		if(m_eMoveStyle==static_cast<int>(m_combMoveStyle.GetItemData(i)))
		{
			m_combMoveStyle.SetCurSel(i);
			break;
		}
	}
	//显示类型
	index = m_combChartType.AddString(_T("线状图"));
	m_combChartType.SetItemData(index, E_CHART_LINE);
	for(int i=0; i<m_combChartType.GetCount(); ++i)
	{
		if(m_eChartType==static_cast<int>(m_combChartType.GetItemData(i)))
		{
			m_combChartType.SetCurSel(i);
			break;
		}
	}
	//线型
	index = m_combLineStyle.AddString(_T("线"));
	m_combLineStyle.SetItemData(index, E_LINE_LINE);
	index = m_combLineStyle.AddString(_T("点"));
	m_combLineStyle.SetItemData(index, E_LINE_DOT);
	index = m_combLineStyle.AddString(_T("点线"));
	m_combLineStyle.SetItemData(index, E_LINE_DOT_LINE);
	for(int i=0; i<m_combLineStyle.GetCount(); ++i)
	{
		if(m_eLineStyle==static_cast<int>(m_combLineStyle.GetItemData(i)))
		{
			m_combLineStyle.SetCurSel(i);
			break;
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void DlgChartSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//更新数据
	//XID
	int nID = -1;
	int index = m_combX.GetCurSel();
	if(index>=0)
	{
		nID = m_combX.GetItemData(index);
	}
	m_nXID = nID;
	//实验时
	index = m_combMoveStyle.GetCurSel();
	m_eMoveStyle = E_Y_SCROLL;
	if(index>=0)
	{
		m_eMoveStyle = MoveStyle(static_cast<int>(m_combMoveStyle.GetItemData(index)));
	}
	//显示类型
	index = m_combChartType.GetCurSel();
	m_eChartType = E_CHART_LINE;
	if(index>=0)
	{
		m_eChartType = ChartType(static_cast<int>(m_combChartType.GetItemData(index)));
	}
	//线型
	index = m_combLineStyle.GetCurSel();
	m_eLineStyle = E_LINE_LINE;
	if(index>=0)
	{
		m_eLineStyle = LineStyle(static_cast<int>(m_combLineStyle.GetItemData(index)));
	}
	//Y轴
	m_setShowID.clear();
	HTREEITEM hGroup = m_treeY.GetRootItem();
	while(hGroup)
	{
		HTREEITEM hCol = m_treeY.GetChildItem(hGroup);
		while(hCol)
		{
			if(m_treeY.GetCheck(hCol))
			{
				m_setShowID.insert(static_cast<int>(m_treeY.GetItemData(hCol)));
			}
			hCol = m_treeY.GetNextSiblingItem(hCol);
		}
		hGroup = m_treeY.GetNextSiblingItem(hGroup);
	}
	CDialog::OnOK();
}
#pragma warning(pop)