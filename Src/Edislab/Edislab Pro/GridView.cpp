// GridView.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "GridView.h"


// CGridView

IMPLEMENT_DYNCREATE(CGridView, CBCGPFormView)

CGridView::CGridView()
	: CBCGPFormView(CGridView::IDD)
{

}

CGridView::~CGridView()
{
}

void CGridView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGridView, CBCGPFormView)
END_MESSAGE_MAP()


// CGridView 诊断

#ifdef _DEBUG
void CGridView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGridView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGridView 消息处理程序
