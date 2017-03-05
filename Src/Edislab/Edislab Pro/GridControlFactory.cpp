/**************************************************************************************************************************************************
*描述:Grid控件工厂(多视图Grid组合)
*作者:
*日期:2015.8
**************************************************************************************************************************************************/

#include "stdafx.h"
#include "GridControlFactory.h"
#include "CustomLabGridCtrl.h"

/*************************************************************************************************
函数名称:GridCallback
功能描述:grid回调函数
输入参数:
输出参数:
返回值: 
***************************************************************************************************/
static BOOL CALLBACK GridCallback (BCGPGRID_DISPINFO* pdi, LPARAM lp)
{
	ASSERT (pdi != NULL);

	CCustomLabGridCtrl* pGridCtrl = (CCustomLabGridCtrl*) lp;

	int nRow = pdi->item.nRow;	// Row of an item
	int nCol = pdi->item.nCol;	// Column of an item

	//if(nCol>= 0&&nCol<100)
		//pdi->item.varValue = (pGridCtrl->m_GridDataManager->GetGridData())[nCol][nRow];

	//刷新数据
	if(nCol>= 0)
	{
		CString strTemp;
		GLOBAL_DATA.GetItemData(nCol,nRow,strTemp);
		pdi->item.varValue = strTemp;
	}

	return TRUE;
}

/*************************************************************************************************
函数名称:CGridControlFactory
功能描述:构造函数
输入参数:
输出参数:无
返回值: 无
***************************************************************************************************/
CGridControlFactory::CGridControlFactory(void)
{
	m_ViewManager = NULL;

	m_ViewManager = new CGridViewManager();
}

/*************************************************************************************************
函数名称:CGridControlFactory
功能描述:析构函数
输入参数:
输出参数:无
返回值: 无
***************************************************************************************************/
CGridControlFactory::~CGridControlFactory(void)
{

}

/*************************************************************************************************
函数名称:CreateGridCtrl
功能描述:为窗口添加Grid控件
输入参数:_Parent : 父窗口句柄
输出参数:无
返回值: 无
***************************************************************************************************/
CWnd* CGridControlFactory::AddGridCtrl(CWnd* _Parent)
{
	if (NULL == _Parent)
		return NULL;

	CCustomLabGridCtrl* pGrid = NULL;
	pGrid = new CCustomLabGridCtrl();

	if (NULL == pGrid)
		return NULL;

	pGrid->SetViewManager(m_ViewManager);

	pGrid->SetCallBack(GridCallback);

	pGrid->Create (WS_VISIBLE |WS_CHILD, CRect(0,0,0,0), _Parent, 1);
	pGrid->ShowWindow(TRUE);

	m_ViewManager->AddGridView(pGrid);

	return pGrid;
}

//移除窗口Grid控件
void CGridControlFactory::RemoveGridCtrl(CWnd* pWnd)
{
	//判断有效性
	if(NULL != pWnd)
	{
		m_ViewManager->RemoveGridView((CCustomLabGridCtrl*)pWnd);
	}
}
