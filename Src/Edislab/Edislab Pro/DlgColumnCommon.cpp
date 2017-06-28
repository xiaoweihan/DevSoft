// DlgColumnCommon.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgColumnCommon.h"



// CDlgColumnCommon 对话框

IMPLEMENT_DYNAMIC(CDlgColumnCommon, CBaseDialog)

CDlgColumnCommon::CDlgColumnCommon(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgColumnCommon::IDD, pParent)
{

}

CDlgColumnCommon::~CDlgColumnCommon()
{
}

void CDlgColumnCommon::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgColumnCommon, CDialog)
END_MESSAGE_MAP()


// CDlgColumnCommon 消息处理程序
