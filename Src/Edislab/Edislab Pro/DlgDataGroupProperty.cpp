// DlgDataGroupProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgDataGroupProperty.h"
#include "Utility.h"
#include "GridColumnGroupManager.h"
// CDlgDataGroupProperty 对话框

IMPLEMENT_DYNAMIC(CDlgDataGroupProperty, CBaseDialog)

CDlgDataGroupProperty::CDlgDataGroupProperty(const CString& strGroupName,CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgDataGroupProperty::IDD, pParent),
	m_strGroupName(strGroupName)
{

}

CDlgDataGroupProperty::~CDlgDataGroupProperty()
{
}

void CDlgDataGroupProperty::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDataGroupProperty, CBaseDialog)
	ON_BN_CLICKED(ID_BTN_OK, &CDlgDataGroupProperty::OnBnClickedBtnOk)
	ON_BN_CLICKED(ID_BTN_CANCEL, &CDlgDataGroupProperty::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDlgDataGroupProperty 消息处理程序


BOOL CDlgDataGroupProperty::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitDisplay();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgDataGroupProperty::InitDisplay()
{
	CWnd* pWnd = GetDlgItem(IDC_DATA_GROUP_NAME);
	if (nullptr != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		pWnd->SetWindowText(m_strGroupName);
	}

	CTime CurrentTime = CTime::GetCurrentTime();

	CString strTime = CurrentTime.Format(_T("%Y-%m-%d %H:%M:%S"));

	pWnd = GetDlgItem(IDC_DATA_YIELD_TIME);

	if (nullptr != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		pWnd->SetWindowText(strTime);
	}
}


void CDlgDataGroupProperty::OnBnClickedBtnOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pWnd = GetDlgItem(IDC_DATA_GROUP_NAME);
	if (nullptr != pWnd && NULL != pWnd->GetSafeHwnd())
	{
		CString strGroupName;
		pWnd->GetWindowText(strGroupName);

		if (strGroupName.IsEmpty())
		{
			Utility::AfxBCGPMessageBox(_T("数组组名不能为空!"),MB_OK | MB_ICONERROR);
			return;
		}

		//如果尚未做某种改变
		if (strGroupName == m_strGroupName)
		{
			OnOK();
			return;
		}

		if (CGridColumnGroupManager::CreateInstance().IsHeaderNameExist(strGroupName))
		{
			Utility::AfxBCGPMessageBox(_T("该数据组已经存在,请输入另外一个名字!"),MB_OK | MB_ICONERROR);
			return;
		}
		m_strGroupName = strGroupName;
		OnOK();
	}

}


void CDlgDataGroupProperty::OnBnClickedBtnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

CString CDlgDataGroupProperty::GetGroupName(void)
{
	return m_strGroupName;
}


BOOL CDlgDataGroupProperty::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_RETURN == pMsg->wParam)
		{
			OnBnClickedBtnOk();
			return TRUE;
		}
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}
