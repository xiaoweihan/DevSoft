// DlgPageNameConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgPageNameConfig.h"
#include "Utility.h"
// CDlgPageNameConfig 对话框

IMPLEMENT_DYNAMIC(CDlgPageNameConfig, CBaseDialog)

CDlgPageNameConfig::CDlgPageNameConfig( const CString& strPageName,CWnd* pParent /*= NULL*/ )
	: CBaseDialog(CDlgPageNameConfig::IDD, pParent),
	m_strPageName(strPageName)
{

}


CDlgPageNameConfig::~CDlgPageNameConfig()
{
}

void CDlgPageNameConfig::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PAGE_NAME, m_EditPageName);
}


BEGIN_MESSAGE_MAP(CDlgPageNameConfig, CBaseDialog)
	ON_BN_CLICKED(IDC_BTN_OK, &CDlgPageNameConfig::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgPageNameConfig::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDlgPageNameConfig 消息处理程序


BOOL CDlgPageNameConfig::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgPageNameConfig::InitDisplay( void )
{
	if (NULL != m_EditPageName.GetSafeHwnd())
	{
		if (!m_strPageName.IsEmpty())
		{
			m_EditPageName.SetWindowText(m_strPageName);
		}
	}
}


void CDlgPageNameConfig::OnBnClickedBtnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL == m_EditPageName.GetSafeHwnd())
	{
		return;
	}

	m_EditPageName.GetWindowText(m_strPageName);

	if (m_strPageName.IsEmpty())
	{
		Utility::AfxBCGPMessageBox(_T("页面名称不能为空!"),MB_OK);
		return;
	}
	OnOK();
}


void CDlgPageNameConfig::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

CString CDlgPageNameConfig::GetPageName( void ) const
{
	return m_strPageName;
}
