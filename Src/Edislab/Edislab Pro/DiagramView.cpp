// DiagramView.cpp : 实现文件
//

#include "stdafx.h"
#include "Edislab Pro.h"
#include "DiagramView.h"


// CDiagramView

IMPLEMENT_DYNCREATE(CDiagramView, CBCGPFormView)

CDiagramView::CDiagramView()
	: CBCGPFormView(CDiagramView::IDD)
{

}

CDiagramView::~CDiagramView()
{
}

void CDiagramView::DoDataExchange(CDataExchange* pDX)
{
	CBCGPFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDiagramView, CBCGPFormView)
END_MESSAGE_MAP()


// CDiagramView 诊断

#ifdef _DEBUG
void CDiagramView::AssertValid() const
{
	CBCGPFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDiagramView::Dump(CDumpContext& dc) const
{
	CBCGPFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDiagramView 消息处理程序
