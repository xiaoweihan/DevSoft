// DlgSensorChoose.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgSensorChoose.h"
#include <boost/foreach.hpp>
#include "Global.h"
// CDlgSensorChoose 对话框

IMPLEMENT_DYNAMIC(CDlgSensorChoose, CBaseDialog)

CDlgSensorChoose::CDlgSensorChoose(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSensorChoose::IDD, pParent)
{

}

CDlgSensorChoose::~CDlgSensorChoose()
{
}

void CDlgSensorChoose::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_SENSOR_TYPE, m_CmbSensorType);
	DDX_Control(pDX, IDC_CMB_RANGE,       m_CmbRange);
	DDX_Control(pDX, IDC_BTN_ADD,         m_BtnAdd);
	DDX_Control(pDX, IDC_BTN_DELETE,      m_BtnDel);
	DDX_Control(pDX, IDC_BTN_DELETE_ALL,  m_BtnDelAll);
	DDX_Control(pDX, IDC_BTN_CHOOSE_CONNECTED, m_BtnChooseCon);
	DDX_Control(pDX, IDC_SENSOR_LIST,           m_ListSensor);
	DDX_Control(pDX, IDC_SELECTED_SENSOR_LIST,           m_ListChoosedSensor);
	DDX_Control(pDX, IDC_STATIC_RESOLUTION_RATIO, m_StaticResolutionRatio);
	DDX_Control(pDX, IDC_CHECK_AUTO_RECOGNIZE, m_CheckAutoChoose);
}


BEGIN_MESSAGE_MAP(CDlgSensorChoose, CDialog)
	ON_BN_CLICKED(IDC_CHECK_AUTO_RECOGNIZE, &CDlgSensorChoose::OnBnClickedCheckAutoRecognize)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgSensorChoose::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CDlgSensorChoose::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALL, &CDlgSensorChoose::OnBnClickedBtnDeleteAll)
	ON_BN_CLICKED(IDC_BTN_CHOOSE_CONNECTED, &CDlgSensorChoose::OnBnClickedBtnChooseConnected)
	ON_BN_CLICKED(ID_BTN_OK, &CDlgSensorChoose::OnBnClickedBtnOk)
END_MESSAGE_MAP()


// CDlgSensorChoose 消息处理程序
void CDlgSensorChoose::InitCtrls()
{

	//begin add by xiaowei.han at 2017/04/23 0:18:43
	if (m_BtnChooseCon.GetSafeHwnd())
	{
		if (g_bAutoSelect)
		{
			m_BtnChooseCon.SetCheck(BST_CHECKED);
		}
		else
		{
			m_BtnChooseCon.SetCheck(BST_CHECKED);
		}
		
	}
	if (m_CmbSensorType.GetSafeHwnd())
	{
		CString strCategoryArray[] = {_T("所有"),_T("物理"),_T("化学"),_T("生物")};

		BOOST_FOREACH(auto& Element,strCategoryArray)
		{
			m_CmbSensorType.AddString(Element);
		}

		m_CmbSensorType.SetCurSel(0);
	}
	//end add by xiaowei.han at 2017/04/23 0:18:43

}



void CDlgSensorChoose::OnBnClickedCheckAutoRecognize()
{
	// TODO: 在此添加控件通知处理程序代码
	if(BST_CHECKED == m_CheckAutoChoose.GetCheck())
	{
		m_BtnAdd.EnableWindow(false);
		m_BtnDel.EnableWindow(false);
		m_BtnDelAll.EnableWindow(false);
		m_BtnChooseCon.EnableWindow(false);
	}
	else
	{
		m_BtnAdd.EnableWindow(true);
		m_BtnDel.EnableWindow(true);
		m_BtnDelAll.EnableWindow(true);
		m_BtnChooseCon.EnableWindow(true);
	}

	// 添加传感器识别部分代码


}


BOOL CDlgSensorChoose::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitCtrls();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSensorChoose::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSensorChoose::OnBnClickedBtnDelete()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSensorChoose::OnBnClickedBtnDeleteAll()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSensorChoose::OnBnClickedBtnChooseConnected()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgSensorChoose::OnBnClickedBtnOk()
{
	// TODO: 在此添加控件通知处理程序代码
}
