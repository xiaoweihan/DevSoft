// DlgChartSet.cpp : implementation file
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgChartSet.h"
//#include "GlobalDataManager.h"
#include "GridColumnGroupManager.h"
#include "SensorManager.h"
#include "SensorData.h"
#include "SensorDataManager.h"
#include "Utility.h"
#pragma warning(push)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
// DlgChartSet dialog

IMPLEMENT_DYNAMIC(DlgChartSet, CBaseDialog)

DlgChartSet::DlgChartSet(CWnd* pParent /*=NULL*/)
	: CBaseDialog(DlgChartSet::IDD, pParent)
{
	//m_nXID = -1;
	m_eChartType = E_CHART_LINE;
	m_eLineStyle = E_LINE_LINE;
	m_eMoveStyle = E_X_HANDLE;
	m_treeY.m_bVisualManagerStyle = TRUE;
	m_DataArray.clear();

}

DlgChartSet::~DlgChartSet()
{
	m_DataArray.clear();
}

void DlgChartSet::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_XAXIS, m_combX);
	DDX_Control(pDX, IDC_TREE_YAXIS, m_treeY);
	DDX_Control(pDX, IDC_COMBO_LAB, m_combMoveStyle);
	DDX_Control(pDX, IDC_COMBO_FIGURE_TYPE, m_combChartType);
	DDX_Control(pDX, IDC_COMBO_LINE_TYPE, m_combLineStyle);
}


BEGIN_MESSAGE_MAP(DlgChartSet, CBaseDialog)
	ON_BN_CLICKED(IDOK, &DlgChartSet::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgChartSet message handlers


BOOL DlgChartSet::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	std::vector<SENSOR_TYPE_INFO_ELEMENT> SensorArray;
	CSensorManager::CreateInstance().GetSensorList(SensorArray);
	for(int i = 0; i < (int)SensorArray.size(); ++i)
	{
		int index = m_combX.AddString(CString(SensorArray[i].strSensorName.c_str()));
		//int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(SensorNameArray[i]);
		LP_SENSOR_TYPE_KEY pData = new SENSOR_TYPE_KEY(SensorArray[i].nSensorID,SensorArray[i].nSensorSerialID);
		
		if (nullptr != pData)
		{
			m_combX.SetItemData(index,(DWORD_PTR)pData);
			m_DataArray.push_back(pData);
		}
		
	}
	m_combX.SetCurSel(0);
	for(int i = 0; i < m_combX.GetCount(); ++i)
	{
		LP_SENSOR_TYPE_KEY pData = (LP_SENSOR_TYPE_KEY)m_combX.GetItemData(i);
		if (nullptr != pData)
		{
			if(m_XKeyID == *pData)
			{
				m_combX.SetCurSel(i);
				break;
			}
		}	
	}
	//Y轴
	//m_treeY.SetImageList(NULL, TVSIL_NORMAL);
	m_treeY.ModifyStyle(TVS_CHECKBOXES,0);
	m_treeY.ModifyStyle(0,TVS_CHECKBOXES);
	std::vector<COLUMN_GROUP_INFO> ColumnGroupArray;
	CGridColumnGroupManager::CreateInstance().GetGridDisplayInfo(ColumnGroupArray);
	for(int i = 0; i < (int)ColumnGroupArray.size(); ++i)
	{
		HTREEITEM hGroup = NULL;
		hGroup = m_treeY.InsertItem(ColumnGroupArray[i].strGroupName);
		m_treeY.SetItemData(hGroup, 0);
		for(int g=0; g < ColumnGroupArray[i].ColumnArray.size(); ++g)
		{
			//获取传感器名称
			CString strTempName = ColumnGroupArray[i].ColumnArray[g].strColumnName;
			std::string strColumnName = Utility::WideChar2MultiByte(strTempName.GetBuffer(0));
			//根据名称获取SensorID
			SENSOR_TYPE_KEY SensorKeyID;
			CSensorManager::CreateInstance().QuerySensorIDByName(strColumnName,SensorKeyID);
			//int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(strColumnName);
			HTREEITEM hCol = m_treeY.InsertItem(strTempName,hGroup);
			LP_SENSOR_TYPE_KEY pData = new SENSOR_TYPE_KEY(SensorKeyID.nSensorID,SensorKeyID.nSensorSerialID);
			if (nullptr != pData)
			{
				m_treeY.SetItemData(hCol,(DWORD_PTR)pData);
				if(m_setShowID.find(SensorKeyID) != m_setShowID.end())
				{
					m_treeY.SetCheck(hCol,TRUE);//选择
				}
			}		
		}
		m_treeY.Expand(hGroup, TVE_EXPAND);
	}
	//end ldh 0610
	HTREEITEM hGroup = m_treeY.GetRootItem();
	while(hGroup)
	{
		HTREEITEM hCol = m_treeY.GetChildItem(hGroup);
		while(hCol)
		{
			LP_SENSOR_TYPE_KEY pSensorKeyID = LP_SENSOR_TYPE_KEY(m_treeY.GetItemData(hCol));
			if(m_setShowID.find(*pSensorKeyID) != m_setShowID.end())
			{
				m_treeY.SetCheck(hCol, TRUE);//选择
			}
			hCol = m_treeY.GetNextSiblingItem(hCol);
		}
		hGroup = m_treeY.GetNextSiblingItem(hGroup);
	}
	//实验时
	int index = m_combMoveStyle.AddString(_T("横轴翻页"));
	m_combMoveStyle.SetItemData(index, E_X_SCROLL);
	index = m_combMoveStyle.AddString(_T("横轴全显"));
	m_combMoveStyle.SetItemData(index, E_X_SHOWALL);
	index = m_combMoveStyle.AddString(_T("手动"));
	m_combMoveStyle.SetItemData(index, E_X_HANDLE);
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
	SENSOR_TYPE_KEY nID;
	int index = m_combX.GetCurSel();
	if(index >= 0)
	{
		LP_SENSOR_TYPE_KEY pData = (LP_SENSOR_TYPE_KEY)m_combX.GetItemData(index);
		if (nullptr != pData)
		{
			nID = * pData;
		}
	}
	m_XKeyID = nID;
	//实验时
	index = m_combMoveStyle.GetCurSel();
	m_eMoveStyle = E_X_SCROLL;
	if(index >= 0)
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
	if(index >= 0)
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
				m_setShowID.insert(*(LP_SENSOR_TYPE_KEY)(m_treeY.GetItemData(hCol)));
			}
			hCol = m_treeY.GetNextSiblingItem(hCol);
		}
		hGroup = m_treeY.GetNextSiblingItem(hGroup);
	}
	CBaseDialog::OnOK();
}
#pragma warning(pop)