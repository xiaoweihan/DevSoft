// DlgAddColumn.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgAddColumn.h"
// CDlgAddColumn 对话框

IMPLEMENT_DYNAMIC(CDlgAddColumn, CBaseDialog)

CDlgAddColumn::CDlgAddColumn(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAddColumn::IDD, pParent)
{

}

CDlgAddColumn::~CDlgAddColumn()
{
}

void CDlgAddColumn::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAddColumn, CBaseDialog)
END_MESSAGE_MAP()


// CDlgAddColumn 消息处理程序


BOOL CDlgAddColumn::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
