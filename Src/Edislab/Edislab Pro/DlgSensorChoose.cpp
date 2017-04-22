// DlgSensorChoose.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgSensorChoose.h"
#include "afxdialogex.h"


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
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_SENSOR_TYPE, m_CmbSensorType);
	DDX_Control(pDX, IDC_CMB_RANGE,       m_CmbRange);
	DDX_Control(pDX, IDC_BTN_ADD,         m_BtnAdd);
	DDX_Control(pDX, IDC_BTN_DELETE,      m_BtnDel);
	DDX_Control(pDX, IDC_BTN_DELETE_ALL,  m_BtnDelAll);
	DDX_Control(pDX, IDC_BTN_CHOOSE_CONNECTED, m_BtnChooseCon);
	DDX_Control(pDX, IDC_LIST2,           m_ListSensor);
	DDX_Control(pDX, IDC_LIST3,           m_ListChoosedSensor);
	DDX_Control(pDX, IDC_STATIC_RESOLUTION_RATIO, m_StaticResolutionRatio);
}


BEGIN_MESSAGE_MAP(CDlgSensorChoose, CDialog)
	ON_BN_CLICKED(IDC_CHECK_AUTO_RECOGNIZE, &CDlgSensorChoose::OnBnClickedCheckAutoRecognize)
END_MESSAGE_MAP()


// CDlgSensorChoose 消息处理程序
void CDlgSensorChoose::InitCtrls()
{

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
