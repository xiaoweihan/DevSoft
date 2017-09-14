// DlgDeviceSet.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include <boost/checked_delete.hpp>
#include "DlgDeviceSet.h"
//#include "GlobalDataManager.h"
#include "GridColumnGroupManager.h"
//#include "SensorIDGenerator.h"
#include "SensorManager.h"
#include "SensorData.h"
#include "SensorDataManager.h"
#include "Utility.h"
#pragma warning(push)
#pragma warning(disable:4800)
#pragma warning(disable:4018)
#pragma warning(disable:4244)
// DlgDeviceSet dialog

IMPLEMENT_DYNAMIC(DlgDeviceSet, CBaseDialog)

DlgDeviceSet::DlgDeviceSet(CWnd* pParent /*=NULL*/)
	: CBaseDialog(DlgDeviceSet::IDD, pParent)
	, m_warningValue(0)
	, m_bWarning(FALSE)
{
	m_bWarning = false;
	m_warningValue = 10;
	m_nWarningType = 0;
	m_ptrDataArray.clear();
}

DlgDeviceSet::~DlgDeviceSet()
{
	m_ptrDataArray.clear();
}

void DlgDeviceSet::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WARN_VALUE, m_edtValue);
	DDX_Text(pDX, IDC_EDIT_WARN_VALUE, m_warningValue);
	DDX_Check(pDX, IDC_CHECK_WARNING, m_bWarning);
	DDX_Control(pDX, IDC_COMBO_WARNING, m_combWarningTYpe);
	DDX_Control(pDX, IDC_COMBO_DATA, m_combDataID);
}


BEGIN_MESSAGE_MAP(DlgDeviceSet, CBaseDialog)
//	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_CHECK_WARNING, &DlgDeviceSet::OnBnHotItemChangeCheckWarning)
	ON_BN_CLICKED(IDC_CHECK_WARNING, &DlgDeviceSet::OnBnClickedCheckWarning)
	ON_BN_CLICKED(IDOK, &DlgDeviceSet::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// DlgDeviceSet message handlers
//获取要显示的数据列ID
SENSOR_TYPE_KEY DlgDeviceSet::getDataColumnID()
{
	return m_SensorKeyID;
}
void DlgDeviceSet::setDataColumnID(SENSOR_TYPE_KEY SensorKeyID)
{
	m_SensorKeyID = SensorKeyID;
}
//警告启用?
bool DlgDeviceSet::getWarningState()
{
	return m_bWarning;
}
void DlgDeviceSet::setWarningState(bool warning)
{
	m_bWarning = warning;
}
//最大最小0:超过 1:降到
int DlgDeviceSet::getWarningType()
{
	return m_nWarningType;
}
void DlgDeviceSet::setWarningType(int type)
{
	m_nWarningType = type;
}
//警告值
double DlgDeviceSet::getWarningValue()
{
	return m_warningValue;
}
void DlgDeviceSet::setWarningValue(double val)
{
	m_warningValue = val;
}

BOOL DlgDeviceSet::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	UpdateData(false);
	m_combWarningTYpe.EnableWindow(m_bWarning);
	m_edtValue.EnableWindow(m_bWarning);
	int index = m_combWarningTYpe.AddString(_T("超过"));
	m_combWarningTYpe.SetItemData(index, 0);
	index = m_combWarningTYpe.AddString(_T("降到"));
	m_combWarningTYpe.SetItemData(index, 1);
	m_combWarningTYpe.SetCurSel(m_nWarningType);
	std::vector<SENSOR_TYPE_INFO_ELEMENT> SensorArray;
	CSensorManager::CreateInstance().GetSensorList(SensorArray);
	for(int i = 0; i < (int)SensorArray.size(); ++i)
	{
		int index = m_combDataID.AddString(CString(SensorArray[i].strSensorName.c_str()));
		//int nSensorID = CSensorIDGenerator::CreateInstance().QuerySensorTypeIDByName(SensorNameArray[i]);
		LP_SENSOR_TYPE_KEY pData = new SENSOR_TYPE_KEY(SensorArray[i].nSensorID,SensorArray[i].nSensorSerialID);

		if (nullptr != pData)
		{
			m_combDataID.SetItemData(index,(DWORD_PTR)pData);
			m_ptrDataArray.push_back(pData);
		}
		
	}
	m_combDataID.SetCurSel(0);
	for(int i = 0; i < m_combDataID.GetCount(); ++i)
	{
		LP_SENSOR_TYPE_KEY pData = (LP_SENSOR_TYPE_KEY)m_combDataID.GetItemData(i);
		if (nullptr != pData)
		{
			if(*pData == m_SensorKeyID)
			{
				m_combDataID.SetCurSel(i);
				break;
			}
		}	
	}
	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void DlgDeviceSet::OnBnClickedCheckWarning()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_combWarningTYpe.EnableWindow(m_bWarning);
	m_edtValue.EnableWindow(m_bWarning);
}


void DlgDeviceSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	//数据ID
	int index = m_combDataID.GetCurSel();
	if(index >= 0)
	{
		LP_SENSOR_TYPE_KEY pData = (LP_SENSOR_TYPE_KEY)m_combDataID.GetItemData(index);

		if (nullptr != pData)
		{
			m_SensorKeyID = *pData;
		}
	}
	//报警类型
	index = m_combWarningTYpe.GetCurSel();
	m_nWarningType = m_combWarningTYpe.GetItemData(index);
	PostMessage(WM_CLOSE,0,0);
}
#pragma warning(pop)

void DlgDeviceSet::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CBaseDialog::OnClose();
}
