// DlgColumnDisplay.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DlgColumnDisplay.h"



// CDlgColumnDisplay 对话框

IMPLEMENT_DYNAMIC(CDlgColumnDisplay, CBaseDialog)

CDlgColumnDisplay::CDlgColumnDisplay(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgColumnDisplay::IDD, pParent)
{

}

CDlgColumnDisplay::~CDlgColumnDisplay()
{
}

void CDlgColumnDisplay::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgColumnDisplay, CDialog)
END_MESSAGE_MAP()


// CDlgColumnDisplay 消息处理程序
