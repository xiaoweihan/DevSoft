// DlgGridOpt.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgGridOpt.h"


// CDlgGridOpt 对话框

IMPLEMENT_DYNAMIC(CDlgGridOpt, CBaseDialog)

CDlgGridOpt::CDlgGridOpt(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgGridOpt::IDD, pParent)
{
	m_GridDisplayArray.clear();
}

CDlgGridOpt::~CDlgGridOpt()
{
}

void CDlgGridOpt::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_DISPLAY_TREE, m_GridDisplayItemTree);
}


BEGIN_MESSAGE_MAP(CDlgGridOpt, CBaseDialog)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CDlgGridOpt::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgGridOpt::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDlgGridOpt 消息处理程序


BOOL CDlgGridOpt::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/*******************************************************************
*函数名称:InitDisplay
*功能描述:初始化对话框显示
*输入参数:
*输出参数:
*返回值:
*作者:xiaowei.han
*日期:2017/06/04 15:04:56
*******************************************************************/
void CDlgGridOpt::InitDisplay(void)
{
	if (NULL == m_GridDisplayItemTree.GetSafeHwnd())
	{
		return;
	}
}


void CDlgGridOpt::OnBnClickedBtnApply()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}


void CDlgGridOpt::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CDlgGridOpt::SetDisplayInfo(const std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray)
{
	m_GridDisplayArray = GridDisplayArray;
}

void CDlgGridOpt::GetDisplayInfo(std::vector<SHOW_COLUMN_GROUP_INFO>& GridDisplayArray)
{
	GridDisplayArray.clear();

	GridDisplayArray = m_GridDisplayArray;
}
