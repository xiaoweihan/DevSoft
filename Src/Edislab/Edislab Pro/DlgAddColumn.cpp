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

	InitDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgAddColumn::InitDisplay( void )
{
	CWnd* pWnd = GetDlgItem(IDC_TAB_POS);
	if (nullptr != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		pWnd->ShowWindow(SW_HIDE);
		//获取位置
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		if (NULL == m_TabWnd)
		{
			if (FALSE == m_TabWnd.Create(CBCGPTabWnd::STYLE_3D,rc,this,CBaseTabWnd::s_nTabBaseID++
				,CBCGPBaseTabWnd::LOCATION_TOP))
			{
				return;
			}
		}
	}
}
