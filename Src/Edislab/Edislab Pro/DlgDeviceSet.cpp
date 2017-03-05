// DlgDeviceSet.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DlgDeviceSet.h"
#include "afxdialogex.h"
#include "GlobalDataManager.h"
#pragma warning(push)
#pragma warning(disable:4800)
#pragma warning(disable:4018)
// DlgDeviceSet dialog

IMPLEMENT_DYNAMIC(DlgDeviceSet, CDialog)

DlgDeviceSet::DlgDeviceSet(CWnd* pParent /*=NULL*/)
	: CDialog(DlgDeviceSet::IDD, pParent)
	, m_warningValue(0)
	, m_bWarning(FALSE)
{
	m_bWarning = false;
	m_warningValue = 10;
	m_nWarningType = 0;
}

DlgDeviceSet::~DlgDeviceSet()
{
}

void DlgDeviceSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WARN_VALUE, m_edtValue);
	DDX_Text(pDX, IDC_EDIT_WARN_VALUE, m_warningValue);
	DDX_Check(pDX, IDC_CHECK_WARNING, m_bWarning);
	DDX_Control(pDX, IDC_COMBO_WARNING, m_combWarningTYpe);
	DDX_Control(pDX, IDC_COMBO_DATA, m_combDataID);
}


BEGIN_MESSAGE_MAP(DlgDeviceSet, CDialog)
//	ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_CHECK_WARNING, &DlgDeviceSet::OnBnHotItemChangeCheckWarning)
	ON_BN_CLICKED(IDC_CHECK_WARNING, &DlgDeviceSet::OnBnClickedCheckWarning)
	ON_BN_CLICKED(IDOK, &DlgDeviceSet::OnBnClickedOk)
END_MESSAGE_MAP()


// DlgDeviceSet message handlers
//获取要显示的数据列ID
int DlgDeviceSet::getDataColumnID()
{
	return m_nDataID;
}
void DlgDeviceSet::setDataColumnID(int ID)
{
	m_nDataID = ID;
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

//void DlgDeviceSet::OnBnHotItemChangeCheckWarning(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// This feature requires Internet Explorer 6 or greater.
//	// The symbol _WIN32_IE must be >= 0x0600.
//	LPNMBCHOTITEM pHotItem = reinterpret_cast<LPNMBCHOTITEM>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}


BOOL DlgDeviceSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(false);
	m_combWarningTYpe.EnableWindow(m_bWarning);
	m_edtValue.EnableWindow(m_bWarning);
	int index = m_combWarningTYpe.AddString(_T("超过"));
	m_combWarningTYpe.SetItemData(index, 0);
	index = m_combWarningTYpe.AddString(_T("降到"));
	m_combWarningTYpe.SetItemData(index, 1);
	m_combWarningTYpe.SetCurSel(m_nWarningType);

	//添加数据列名称及ID
	std::vector<GROUPDATA> allData = CGlobalDataManager::CreateInstance().getAllData();
	for(int i=0; i<allData.size(); ++i)
	{
		for(int c=0; c<allData[i].vecColData.size(); ++c)
		{
			int index = m_combDataID.AddString(allData[i].vecColData[c].strColumnName);
			m_combDataID.SetItemData(index, allData[i].vecColData[c].nColumnID);
		}
	}
	m_combDataID.SetCurSel(0);
	for(int i=0; i<m_combDataID.GetCount(); ++i)
	{
		if(m_nDataID==m_combDataID.GetItemData(i))
		{
			m_combDataID.SetCurSel(i);
			break;
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
	int nID = -1;
	int index = m_combDataID.GetCurSel();
	if(index>=0)
	{
		nID = m_combDataID.GetItemData(index);
	}
	m_nDataID = nID;
	//报警类型
	index = m_combWarningTYpe.GetCurSel();
	m_nWarningType = m_combWarningTYpe.GetItemData(index);
	//
	CDialog::OnOK();
}
#pragma warning(pop)